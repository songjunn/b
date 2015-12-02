//========= Copyright ?1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: Real-Time Hierarchical Profiling
//
// $NoKeywords: $
//=============================================================================

#ifdef WIN32
#pragma warning(disable:4267)
#endif
#include <map>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include "vprof.h"
#include "console.h"
#ifdef __linux__
#include <stdarg.h>
#endif


// NOTE: Explicitly and intentionally using STL in here to not generate any
// cyclical dependencies between the low-level debug library and the higher
// level data structures (toml 01-27-03)
using namespace std;

//-----------------------------------------------------------------------------
static int g_nDebugOutLine = 0;

//-------------------------------------

CVProfNode *CVProfNode::GetSubNode( const char *pszName )
{
	// Try to find this sub node
	CVProfNode * child = m_pChild;
	while ( child )
	{
		if ( child->m_pszName == pszName )
		{
			return child;
		}
		child = child->m_pSibling;
	}

	// We didn't find it, so add it
	CVProfNode * node = new CVProfNode( pszName, this );
	node->m_pSibling = m_pChild;
	m_pChild = node;
	return node;
}

//-------------------------------------

void CVProfNode::Pause()
{
	if ( m_nRecursions > 0 )
	{
		m_Timer.End();
		m_CurFrameTime += m_Timer.GetDuration();
	}
	if ( m_pChild )
	{
		m_pChild->Pause();
	}
	if ( m_pSibling )
	{
		m_pSibling->Pause();
	}
}

//-------------------------------------

void CVProfNode::Resume()
{
	if ( m_nRecursions > 0 )
	{
		m_Timer.Start();
	}
	if ( m_pChild )
	{
		m_pChild->Resume();
	}
	if ( m_pSibling )
	{
		m_pSibling->Resume();
	}
}

//-------------------------------------

void CVProfNode::Reset()
{
	m_nPrevFrameCalls = 0;
	m_PrevFrameTime.Init();

	m_nCurFrameCalls = 0;
	m_CurFrameTime.Init();

	m_nTotalCalls = 0;
	m_TotalTime.Init();

	m_PeakTime.Init();

	if ( m_pChild )
	{
		m_pChild->Reset();
	}
	if ( m_pSibling )
	{
		m_pSibling->Reset();
	}
}


//-------------------------------------

void CVProfNode::MarkFrame()
{
	m_nPrevFrameCalls = m_nCurFrameCalls;
	m_PrevFrameTime = m_CurFrameTime;

	m_nTotalCalls += m_nCurFrameCalls;
	m_TotalTime += m_CurFrameTime;

	if ( m_PeakTime.IsLessThan( m_CurFrameTime ) )
		m_PeakTime = m_CurFrameTime;

	m_CurFrameTime.Init();
	m_nCurFrameCalls = 0;

	if ( m_pChild )
	{
		m_pChild->MarkFrame();
	}
	if ( m_pSibling )
	{
		m_pSibling->MarkFrame();
	}
}

//-------------------------------------

void CVProfNode::ResetPeak()
{
	m_PeakTime.Init();

	if ( m_pChild )
	{
		m_pChild->ResetPeak();
	}
	if ( m_pSibling )
	{
		m_pSibling->ResetPeak();
	}
}

//-----------------------------------------------------------------------------

struct TimeSums_t
{
	const char *pszProfileScope;
	unsigned	calls;
	double 		time;
	double 		timeLessChildren;
	double		peak;
};

static bool TimeCompare( const TimeSums_t &lhs, const TimeSums_t &rhs )
{
	return ( lhs.time > rhs.time );
}

static bool TimeLessChildrenCompare( const TimeSums_t &lhs, const TimeSums_t &rhs )
{
	return ( lhs.timeLessChildren > rhs.timeLessChildren );
}

static bool PeakCompare( const TimeSums_t &lhs, const TimeSums_t &rhs )
{
	return ( lhs.peak > rhs.peak );
}

static bool AverageTimeCompare( const TimeSums_t &lhs, const TimeSums_t &rhs )
{
	double avgLhs = ( lhs.calls ) ? lhs.time / (double)lhs.calls : 0.0;
	double avgRhs = ( rhs.calls ) ? rhs.time / (double)rhs.calls : 0.0;
	return ( avgLhs > avgRhs );
}

static bool AverageTimeLessChildrenCompare( const TimeSums_t &lhs, const TimeSums_t &rhs )
{
	double avgLhs = ( lhs.calls ) ? lhs.timeLessChildren / (double)lhs.calls : 0.0;
	double avgRhs = ( rhs.calls ) ? rhs.timeLessChildren / (double)rhs.calls : 0.0;
	return ( avgLhs > avgRhs );

}
static bool PeakOverAverageCompare( const TimeSums_t &lhs, const TimeSums_t &rhs )
{
	double avgLhs = ( lhs.calls ) ? lhs.timeLessChildren / (double)lhs.calls : 0.0;
	double avgRhs = ( rhs.calls ) ? rhs.timeLessChildren / (double)rhs.calls : 0.0;

	double lhsPoA = ( avgLhs != 0 ) ? lhs.peak / avgLhs : 0.0;
	double rhsPoA = ( avgRhs != 0 ) ? rhs.peak / avgRhs : 0.0;

	return ( lhsPoA > rhsPoA );
}

map<CVProfNode *, double> 	g_TimesLessChildren;
map<const char *, unsigned> g_TimeSumsMap;
vector<TimeSums_t> 			g_TimeSums;

//-------------------------------------

void CVProfile::SumTimes( CVProfNode *pNode, int budgetGroupID )
{
	if ( !pNode )
		return; // this generally only happens on a failed FindNode()

	if ( GetRoot() != pNode )
	{
		//if ( budgetGroupID == -1 || pNode->GetBudgetGroupID() == budgetGroupID )
		{
			double timeLessChildren = pNode->GetTotalTimeLessChildren();

			g_TimesLessChildren.insert( make_pair( pNode, timeLessChildren ) );

			map<const char *, unsigned>::iterator iter;
			iter = g_TimeSumsMap.find( pNode->GetName() ); // intenionally using address of string rather than string compare (toml 01-27-03)
			if ( iter == g_TimeSumsMap.end() )
			{
				TimeSums_t timeSums = { pNode->GetName(), pNode->GetTotalCalls(), pNode->GetTotalTime(), timeLessChildren, pNode->GetPeakTime() };
				g_TimeSumsMap.insert( make_pair( pNode->GetName(), g_TimeSums.size() ) );
				g_TimeSums.push_back( timeSums );
			}
			else
			{
				TimeSums_t &timeSums = g_TimeSums[iter->second];
				timeSums.calls += pNode->GetTotalCalls();
				timeSums.time += pNode->GetTotalTime();
				timeSums.timeLessChildren += timeLessChildren;
				if ( pNode->GetPeakTime() > timeSums.peak )
					timeSums.peak = pNode->GetPeakTime();
			}
		}

		if( pNode->GetSibling() )
		{
			SumTimes( pNode->GetSibling(), budgetGroupID );
		}
	}

	if( pNode->GetChild() )
	{
		SumTimes( pNode->GetChild(), budgetGroupID );
	}

}

//-------------------------------------

CVProfNode *CVProfile::FindNode( CVProfNode *pStartNode, const char *pszNode )
{
	if ( strcmp( pStartNode->GetName(), pszNode ) != 0 )
	{
		CVProfNode *pFoundNode = NULL;
		if ( pStartNode->GetSibling() )
		{
			pFoundNode = FindNode( pStartNode->GetSibling(), pszNode );
		}

		if ( !pFoundNode && pStartNode->GetChild() )
		{
			pFoundNode = FindNode( pStartNode->GetChild(), pszNode );
		}

		return pFoundNode;
	}
	return pStartNode;
}

//-------------------------------------

void CVProfile::SumTimes( const char *pszStartNode, int budgetGroupID )
{
	if ( GetRoot()->GetChild() )
	{
		CVProfNode *pStartNode;
		if ( pszStartNode == NULL )
			pStartNode = GetRoot();
		else
		{
			// This only works for nodes that show up only once in tree
			pStartNode = FindNode( GetRoot(), pszStartNode );
		}
		SumTimes( pStartNode, budgetGroupID );
	}

}

//-------------------------------------

void CVProfile::DumpNodes( CVProfNode *pNode, int indent )
{
	if ( !pNode )
		return; // this generally only happens on a failed FindNode()

	bool fIsRoot = ( pNode == GetRoot() );

	if ( !fIsRoot )
	{
		char szIndent[100];
		szIndent[0] = 0;
		for ( int i = 0; i < indent; i++ )
			//Msg( "  " );
			strcat( szIndent, "    " );//by jinsheng
		map<CVProfNode *, double>::iterator iterTimeLessChildren = g_TimesLessChildren.find( pNode );

		Msg("%s%s: Sum[%dc, func+child %.3f, func %.3f] Avg[ func+child %.3f, func %.3f], Peak[%.3f]\n",
			         szIndent,
					 pNode->GetName(),
					 pNode->GetTotalCalls(), pNode->GetTotalTime(), iterTimeLessChildren->second,
					 ( pNode->GetTotalCalls() > 0 ) ? pNode->GetTotalTime() / (double)pNode->GetTotalCalls() : 0,
					 ( pNode->GetTotalCalls() > 0 ) ? iterTimeLessChildren->second / (double)pNode->GetTotalCalls() : 0,
					 pNode->GetPeakTime()  );

	}

	if( pNode->GetChild() )
	{
		DumpNodes( pNode->GetChild(), indent + 1 );
	}

	if( !fIsRoot && pNode->GetSibling() )
	{
		DumpNodes( pNode->GetSibling(), indent );
	}
}

//-------------------------------------

static void DumpSorted( const char *pszHeading, double totalTime, bool (*pfnSort)( const TimeSums_t &, const TimeSums_t & ) )
{
	unsigned i;
	vector<TimeSums_t> sortedSums;
	sortedSums = g_TimeSums;
#ifdef kkk
	sort( sortedSums.begin(), sortedSums.end(), pfnSort );
#endif
	MonitorMsg( "%s\n", pszHeading);
    MonitorMsg( "Calls        Time+Child   Pct     Time          Pct    Avg+Child    Avg          Peak       Scope                                   \n");
    MonitorMsg( "---------- ------------ ------ ------------ ------ ------------ ------------ ------------ ----------------------------------------\n");
    for ( i = 0; i < sortedSums.size(); i++ )
    {
		double avg = ( sortedSums[i].calls ) ? sortedSums[i].time / (double)sortedSums[i].calls : 0.0;
		double avgLessChildren = ( sortedSums[i].calls ) ? sortedSums[i].timeLessChildren / (double)sortedSums[i].calls : 0.0;

        MonitorMsg( "%10d %12.3f %6.2f %12.3f %6.2f %12.3f %12.3f %12.3f %s\n",
             sortedSums[i].calls,
			 sortedSums[i].time,
			 ( sortedSums[i].time / totalTime ) * 100.0,
			 sortedSums[i].timeLessChildren,
			 ( sortedSums[i].timeLessChildren / totalTime ) * 100.0,
			 avg,
			 avgLessChildren,
			 sortedSums[i].peak,
			 sortedSums[i].pszProfileScope);
	}
}

void CVProfile::OutputReport( VProfReportType_t type, const char *pszStartNode, int budgetGroupID )
{
	//MonitorMsg( "******** BEGIN VPROF CLIENT REPORT ********\n");
	//MonitorMsg( "  (note: this report exceeds the output capacity of MSVC debug window. Use console window or console log.) \n");
	if ( NumFramesSampled() == 0 || GetTotalTimeSampled() == 0){
	//	MonitorMsg( "No samples\n" );
	}
	else
	{
		if ( type & VPRT_SUMMARY )
		{
			MonitorMsg( "-- Summary --\n" );
			MonitorMsg( "%d frames sampled for %.2f seconds\n", NumFramesSampled(), GetTotalTimeSampled() / 1000.0 );
			MonitorMsg( "Average %.2f fps, %.2f ms per frame\n", 1000.0 / ( GetTotalTimeSampled() / NumFramesSampled() ), GetTotalTimeSampled() / NumFramesSampled() );
			MonitorMsg( "Peak %.2f ms frame\n", GetPeakFrameTime() );

			double timeAccountedFor = 100.0 - ( m_Root.GetTotalTimeLessChildren() / m_Root.GetTotalTime() );
			MonitorMsg( "%.0f pct of time accounted for\n", min( 100.0, timeAccountedFor ) );
			MonitorMsg( "\n" );
		}

		if ( pszStartNode == NULL )
		{
			pszStartNode = GetRoot()->GetName();
		}

		SumTimes( pszStartNode, budgetGroupID );

		// Dump the hierarchy
		if ( type & VPRT_HIERARCHY )
		{
		//	MonitorMsg( "-- Hierarchical Call Graph --\n");
		//	DumpNodes( (pszStartNode == NULL ) ? GetRoot() : FindNode( GetRoot(), pszStartNode ), 0 );
		//	MonitorMsg( "\n" );
		}

		if ( type & VPRT_LIST_BY_TIME )
		{
			DumpSorted( "-- Profile scopes sorted by time (including children) --", GetTotalTimeSampled(), TimeCompare );
			MonitorMsg( "\n" );
		}
		if ( type & VPRT_LIST_BY_TIME_LESS_CHILDREN )
		{
		//	DumpSorted( "-- Profile scopes sorted by time (without children) --", GetTotalTimeSampled(), TimeLessChildrenCompare );
		//	MonitorMsg( "\n" );
		}
		if ( type & VPRT_LIST_BY_AVG_TIME )
		{
		//	DumpSorted( "-- Profile scopes sorted by average time (including children) --", GetTotalTimeSampled(), AverageTimeCompare );
		//	MonitorMsg( "\n" );
		}
		if ( type & VPRT_LIST_BY_AVG_TIME_LESS_CHILDREN )
		{
		//	DumpSorted( "-- Profile scopes sorted by average time (without children) --", GetTotalTimeSampled(), AverageTimeLessChildrenCompare );
		//	MonitorMsg( "\n" );
		}
		if ( type & VPRT_LIST_BY_PEAK_TIME )
		{
		//	DumpSorted( "-- Profile scopes sorted by peak --", GetTotalTimeSampled(), PeakCompare );
		//	MonitorMsg( "\n" );
		}
		if ( type & VPRT_LIST_BY_PEAK_OVER_AVERAGE )
		{
		//	DumpSorted( "-- Profile scopes sorted by peak over average (including children) --", GetTotalTimeSampled(), PeakOverAverageCompare );
		//	MonitorMsg( "\n" );
		}

		// TODO: Functions by time less children
		// TODO: Functions by time averages
		// TODO: Functions by peak
		// TODO: Peak deviation from average
		g_TimesLessChildren.clear();
		g_TimeSumsMap.clear();
		g_TimeSums.clear();
	}
	MonitorMsg( "******** END VPROF REPORT ********\n");

}

//////////////////////////////////////////////////////////////////////////
//about Server

void CVProfile::Server_DumpNodes( CVProfNode *pNode, int indent )
{
	if ( !pNode )
		return; // this generally only happens on a failed FindNode()

	bool fIsRoot = ( pNode == GetRoot() );

	if ( !fIsRoot )
	{
		char szIndent[100];
		szIndent[0] = 0;
		//for ( int i = 0; i < indent; i++ )
			//Print( g_nDebugOutLine++,"  " );
		map<CVProfNode *, double>::iterator iterTimeLessChildren = g_TimesLessChildren.find( pNode );

		Print( g_nDebugOutLine++,"%s%s: Sum[%dc, func+child %.3f, func %.3f] Avg[ func+child %.3f, func %.3f], Peak[%.3f]\n",
			szIndent,
			pNode->GetName(),
			pNode->GetTotalCalls(), pNode->GetTotalTime(), iterTimeLessChildren->second,
			( pNode->GetTotalCalls() > 0 ) ? pNode->GetTotalTime() / (double)pNode->GetTotalCalls() : 0,
			( pNode->GetTotalCalls() > 0 ) ? iterTimeLessChildren->second / (double)pNode->GetTotalCalls() : 0,
			pNode->GetPeakTime()  );

	}

	if( pNode->GetChild() )
	{
		Server_DumpNodes( pNode->GetChild(), indent + 1 );
	}

	if( !fIsRoot && pNode->GetSibling() )
	{
		Server_DumpNodes( pNode->GetSibling(), indent );
	}
}


static void Server_DumpSorted( const char *pszHeading, double totalTime, bool (*pfnSort)( const TimeSums_t &, const TimeSums_t & ) )
{
	unsigned i;
	vector<TimeSums_t> sortedSums;
	sortedSums = g_TimeSums;
#ifdef kkk
	sort( sortedSums.begin(), sortedSums.end(), pfnSort );
#endif
	Print( g_nDebugOutLine++,"%s\n", pszHeading);
	Print( g_nDebugOutLine++,"            Scope              Calls      Time+Child   Pct+Child     Time       Pct    Avg+Child     Avg         Peak   \n");
	Print( g_nDebugOutLine++,"   ----------------------- ------------- ------------ ----------- ----------- ------- ----------- ----------- ----------\n");
	for ( i = 0; i < sortedSums.size(); i++ )
	{
		double avg = ( sortedSums[i].calls ) ? sortedSums[i].time / (double)sortedSums[i].calls : 0.0;
		double avgLessChildren = ( sortedSums[i].calls ) ? sortedSums[i].timeLessChildren / (double)sortedSums[i].calls : 0.0;

#ifdef __linux__
		Print( g_nDebugOutLine++,"   %20s    %10d    %9.3f        %3.2f    %9.3f     %3.2f     %6.3f      %6.3f     %7.3f\n",
#else
		Print( g_nDebugOutLine++,"   %20s	%10d	%9.3f        %3.2f    %9.3f      %3.2f     %6.3f      %6.3f     %7.3f\n",
#endif
			sortedSums[i].pszProfileScope,
			sortedSums[i].calls,
			sortedSums[i].time,
			( sortedSums[i].time / totalTime ) * 100.0,
			sortedSums[i].timeLessChildren,
			( sortedSums[i].timeLessChildren / totalTime ) * 100.0,
			avg,
			avgLessChildren,
			sortedSums[i].peak );
	}

}

void CVProfile::Server_OutputReport( VProfReportType_t type, const char *pszStartNode, int budgetGroupID )
{
	g_nDebugOutLine = 0;
	//Print(g_nDebugOutLine++, "************************ BEGIN VPROF REPORT *************************\n");
	//Print(g_nDebugOutLine++, "  (note: this report exceeds the output capacity of MSVC debug window. Use console window or console log.) \n");

	if ( NumFramesSampled() == 0 || GetTotalTimeSampled() == 0)
	{
		Print(g_nDebugOutLine++, "No samples\n" );
	}
	else
	{
		if ( type & VPRT_SUMMARY )
		{
			Print( g_nDebugOutLine++, "-- Summary --												\n" );
			Print( g_nDebugOutLine++,"%d frames sampled for %.2f seconds							\n", NumFramesSampled(), GetTotalTimeSampled() / 1000.0 );
			Print( g_nDebugOutLine++,"Average %.2f fps, %.2f ms per frame							\n", 1000.0 / ( GetTotalTimeSampled() / NumFramesSampled() ), GetTotalTimeSampled() / NumFramesSampled() );
			Print( g_nDebugOutLine++,"Peak %.2f ms frame											\n", GetPeakFrameTime() );

			double timeAccountedFor = 100.0 - ( m_Root.GetTotalTimeLessChildren() / m_Root.GetTotalTime() );
			Print( g_nDebugOutLine++,"%.0f pct of time accounted for\n", min( 100.0, timeAccountedFor ) );
			Print( g_nDebugOutLine++,"																\n" );
		}

		if ( pszStartNode == NULL )
		{
			pszStartNode = GetRoot()->GetName();
		}

		SumTimes( pszStartNode, budgetGroupID );

		// Dump the hierarchy
		if ( type & VPRT_HIERARCHY )
		{
// 			Print( g_nDebugOutLine++,"-- Hierarchical Call Graph --\n");
// 			Server_DumpNodes( (pszStartNode == NULL ) ? GetRoot() : FindNode( GetRoot(), pszStartNode ), 0 );
// 			Print( g_nDebugOutLine++,"\n" );
		}

		if ( type & VPRT_LIST_BY_TIME )
		{
			Server_DumpSorted( "-- Profile scopes sorted by time (including children) --", GetTotalTimeSampled(), TimeCompare );
			Print( g_nDebugOutLine++,"																\n" );
		}
		if ( type & VPRT_LIST_BY_TIME_LESS_CHILDREN )
		{
//			Server_DumpSorted( "-- Profile scopes sorted by time (without children) --", GetTotalTimeSampled(), TimeLessChildrenCompare );
//			Print( g_nDebugOutLine++,"\n" );
		}
		if ( type & VPRT_LIST_BY_AVG_TIME )
		{
//			Server_DumpSorted( "-- Profile scopes sorted by average time (including children) --", GetTotalTimeSampled(), AverageTimeCompare );
//			Print( g_nDebugOutLine++,"\n" );
		}
		if ( type & VPRT_LIST_BY_AVG_TIME_LESS_CHILDREN )
		{
//			Server_DumpSorted( "-- Profile scopes sorted by average time (without children) --", GetTotalTimeSampled(), AverageTimeLessChildrenCompare );
//			Print( g_nDebugOutLine++,"\n" );
		}
		if ( type & VPRT_LIST_BY_PEAK_TIME )
		{
//			Server_DumpSorted( "-- Profile scopes sorted by peak --", GetTotalTimeSampled(), PeakCompare );
//			Print( g_nDebugOutLine++,"\n" );
		}
		if ( type & VPRT_LIST_BY_PEAK_OVER_AVERAGE )
		{
//			Server_DumpSorted( "-- Profile scopes sorted by peak over average (including children) --", GetTotalTimeSampled(), PeakOverAverageCompare );
//			Print( g_nDebugOutLine++,"\n" );
		}

		// TODO: Functions by time less children
		// TODO: Functions by time averages
		// TODO: Functions by peak
		// TODO: Peak deviation from average
		g_TimesLessChildren.clear();
		g_TimeSumsMap.clear();
		g_TimeSums.clear();
	}
//	Print(g_nDebugOutLine++,"********************* END VPROF REPORT *************************\n");

}

//=============================================================================
CVProfile & VProfCurrentProfile( )
{
	return *CVProfile::s_VProfCurrentProfile;
}

//=============================================================================
CVProfile* CVProfile::s_VProfCurrentProfile = NULL;
CVProfile::CVProfile()
: 	m_enabled( 0 ),
	m_fAtRoot( true ),
 	m_nFrames( 0 ),
 	m_pausedEnabledDepth( 0 )
{
	g_nDebugOutLine		= 0;

	m_Root.Init( "Root", NULL );
	m_pCurNode = &m_Root;
}

CVProfile::~CVProfile()
{
	if( s_VProfCurrentProfile )
		delete s_VProfCurrentProfile;
}

