//========= Copyright ?1996-2003, Valve LLC, All rights reserved. ============
//
// Purpose: Real-Time Hierarchical Profiling
//
// $NoKeywords: $
//=============================================================================

#ifndef SHARED_VPROF_H
#define SHARED_VPROF_H

#include "fasttimer.h"

#define VPROF_ENABLED

//-----------------------------------------------------------------------------
//
// Profiling instrumentation macros
//

#ifdef VPROF_ENABLED
#define	VPROF( name )		CVProfScope VProf_(name);
#else
#define	VPROF( name )		((void)0)
#endif


//////////////////////////////////////////////////////////////////////////
//
#define MonitorMsg	printf
#define Msg			printf

//////////////////////////////////////////////////////////////////////////
//
// A node in the call graph hierarchy
//

class CVProfNode
{

public:
	CVProfNode( );
	CVProfNode( const char * pszName, CVProfNode *pParent );
	~CVProfNode( );

	void Init( const char * pszName, CVProfNode *pParent );

	CVProfNode *GetSubNode( const char *pszName );
	CVProfNode *GetParent();
	CVProfNode *GetSibling();
	CVProfNode *GetPrevsibling();
	CVProfNode *GetChild();

	void MarkFrame();
	void ResetPeak();

	void Pause();
	void Resume();
	void Reset();

	void EnterScope();
	bool ExitScope();

	const char *GetName();

	int	GetCurCalls();
	double GetCurTime();
	int GetPrevCalls();
	double GetPrevTime();
	int	GetTotalCalls();
	double GetTotalTime();
	double GetPeakTime();

	double GetCurTimeLessChildren();
	double GetPrevTimeLessChildren();
	double GetTotalTimeLessChildren();

private:
	const char *m_pszName;
	CFastTimer	m_Timer;

	int			m_nRecursions;

	unsigned	m_nCurFrameCalls;
	CCycleCount	m_CurFrameTime;

	unsigned	m_nPrevFrameCalls;
	CCycleCount	m_PrevFrameTime;

	unsigned	m_nTotalCalls;
	CCycleCount	m_TotalTime;

	CCycleCount	m_PeakTime;

	CVProfNode *m_pParent;
	CVProfNode *m_pChild;
	CVProfNode *m_pSibling;

};

//-----------------------------------------------------------------------------
//
// Coordinator and root node of the profile hierarchy tree
//

enum VProfReportType_t
{
	VPRT_SUMMARY						= 1 << 0,
	VPRT_HIERARCHY						= 1 << 1,
	VPRT_LIST_BY_TIME					= 1 << 2,
	VPRT_LIST_BY_TIME_LESS_CHILDREN		= 1 << 3,
	VPRT_LIST_BY_AVG_TIME				= 1 << 4,
	VPRT_LIST_BY_AVG_TIME_LESS_CHILDREN	= 1 << 5,
	VPRT_LIST_BY_PEAK_TIME				= 1 << 6,
	VPRT_LIST_BY_PEAK_OVER_AVERAGE		= 1 << 7,
	VPRT_FULL = VPRT_SUMMARY | VPRT_HIERARCHY |
				VPRT_LIST_BY_TIME | VPRT_LIST_BY_TIME_LESS_CHILDREN |
				VPRT_LIST_BY_AVG_TIME |	VPRT_LIST_BY_AVG_TIME_LESS_CHILDREN |
				VPRT_LIST_BY_PEAK_TIME | VPRT_LIST_BY_PEAK_OVER_AVERAGE,
};

class CVProfile
{
public:
	CVProfile();
	~CVProfile();

	void Start();
	void Stop();

	void EnterScope( const char *pszName );
	void ExitScope();

	void MarkFrame();
	void ResetPeaks();

	void Pause();
	void Resume();
	void Reset();

	bool IsEnabled() const;

	bool AtRoot() const;

	int NumFramesSampled()	{ return m_nFrames; }
	double GetPeakFrameTime();
	double GetTotalTimeSampled();
	double GetTimeLastFrame();

	CVProfNode *GetRoot();
	CVProfNode *FindNode( CVProfNode *pStartNode, const char *pszNode );

	void OutputReport( VProfReportType_t type = VPRT_FULL, const char *pszStartNode = NULL, int budgetGroupID = -1 );
	//char *GetReportServerMsg( );

	void Server_DumpNodes(CVProfNode *pNode, int indent);
	void Server_OutputReport( VProfReportType_t type = VPRT_FULL, const char *pszStartNode = NULL, int budgetGroupID = -1 );

private:
	void SumTimes( const char *pszStartNode, int budgetGroupID );
	void SumTimes( CVProfNode *pNode, int budgetGroupID );
	void DumpNodes( CVProfNode *pNode, int indent );

	int 		m_enabled;
	bool		m_fAtRoot; // tracked for efficiency of the "not profiling" case
	CVProfNode *m_pCurNode;
	CVProfNode	m_Root;
	int			m_nFrames;
	int			m_pausedEnabledDepth;

public:
	static CVProfile* s_VProfCurrentProfile;

};


//-----------------------------------------------------------------------------
// Singleton interface
//-----------------------------------------------------------------------------
inline void SetVProfCurrentProfile(CVProfile* pvpro){CVProfile::s_VProfCurrentProfile = pvpro;}
CVProfile &VProfCurrentProfile( );

//-----------------------------------------------------------------------------

class CVProfScope
{
public:
	CVProfScope( const char * pszName );
	~CVProfScope();
};

//-----------------------------------------------------------------------------
//
// CVProfNode, inline methods
//
inline CVProfNode::CVProfNode( )
{
	m_nCurFrameCalls = 0;
	m_nPrevFrameCalls = 0;
	m_nRecursions = 0;
	m_pParent = NULL;
	m_pChild = NULL;
	m_pSibling = NULL;
}

inline CVProfNode::CVProfNode( const char * pszName, CVProfNode *pParent )
{
	Init( pszName, pParent );
}

inline void CVProfNode::Init( const char * pszName, CVProfNode *pParent )
{
	m_pszName = pszName;
	m_nCurFrameCalls = 0;
	m_nPrevFrameCalls = 0;
	m_nRecursions = 0;
	m_pParent = pParent;
	m_pChild = NULL;
	m_pSibling = NULL;

	Reset();
}

//-------------------------------------

inline CVProfNode::~CVProfNode()
{
	if( m_pChild )
		delete m_pChild;
	if( m_pSibling )
		delete m_pSibling;
}

//-------------------------------------

inline CVProfNode *CVProfNode::GetParent()
{
	assert( m_pParent );
	return m_pParent;
}

//-------------------------------------

inline CVProfNode *CVProfNode::GetSibling()
{
	return m_pSibling;
}

//-------------------------------------
// Hacky way to the previous sibling, only used from vprof panel at the moment,
// so it didn't seem like it was worth the memory waste to add the reverse
// link per node.

inline CVProfNode *CVProfNode::GetPrevsibling()
{
	CVProfNode* p = GetParent();

	if(!p)
		return NULL;

	CVProfNode* s;
	for( s = p->GetChild();
	     s && ( s->GetSibling() != this );
		 s = s->GetSibling() )
		;

	return s;
}

//-------------------------------------

inline CVProfNode *CVProfNode::GetChild()
{
	return m_pChild;
}

//-------------------------------------

inline void CVProfNode::EnterScope()
{
	m_nCurFrameCalls++;
	if ( m_nRecursions++ == 0 )
	{
		m_Timer.Start();
	}
}

//-------------------------------------

inline bool CVProfNode::ExitScope()
{
	m_nRecursions = m_nRecursions - 1;
	if ( m_nRecursions == 0 && m_nCurFrameCalls != 0 )
	{
		m_Timer.End();
		m_CurFrameTime += m_Timer.GetDuration();
	}
	return ( m_nRecursions == 0 );
}

//-------------------------------------

inline const char *CVProfNode::GetName()
{
	assert( m_pszName );
	return m_pszName;
}

//-------------------------------------

inline int	CVProfNode::GetTotalCalls()
{
	return m_nTotalCalls;
}

//-------------------------------------

inline double CVProfNode::GetTotalTime()
{
	return m_TotalTime.GetMillisecondsF();
}

//-------------------------------------

inline int	CVProfNode::GetCurCalls()
{
	return m_nCurFrameCalls;
}

//-------------------------------------

inline double CVProfNode::GetCurTime()
{
	return m_CurFrameTime.GetMillisecondsF();
}

//-------------------------------------

inline int CVProfNode::GetPrevCalls()
{
	return m_nPrevFrameCalls;
}

//-------------------------------------

inline double CVProfNode::GetPrevTime()
{
	return m_PrevFrameTime.GetMillisecondsF();
}

//-------------------------------------

inline double CVProfNode::GetPeakTime()
{
	return m_PeakTime.GetMillisecondsF();
}

//-------------------------------------

inline double CVProfNode::GetTotalTimeLessChildren()
{
	double result = GetTotalTime();
	CVProfNode *pChild = GetChild();
	while ( pChild )
	{
		result -= pChild->GetTotalTime();
		pChild = pChild->GetSibling();
	}
	return result;
}

//-------------------------------------

inline double CVProfNode::GetCurTimeLessChildren()
{
	double result = GetCurTime();
	CVProfNode *pChild = GetChild();
	while ( pChild )
	{
		result -= pChild->GetCurTime();
		pChild = pChild->GetSibling();
	}
	return result;
}


//-----------------------------------------------------------------------------
inline double CVProfNode::GetPrevTimeLessChildren()
{
	double result = GetPrevTime();
	CVProfNode *pChild = GetChild();
	while ( pChild )
	{
		result -= pChild->GetPrevTime();
		pChild = pChild->GetSibling();
	}
	return result;
}

//-----------------------------------------------------------------------------
//
// CVProfile, inline methods
//

//-------------------------------------

inline bool CVProfile::IsEnabled() const
{
	return ( m_enabled != 0 );
}

//-------------------------------------

inline bool CVProfile::AtRoot() const
{
	return m_fAtRoot;
}

//-------------------------------------

inline void CVProfile::Start()
{
	if ( ++m_enabled == 1 )
		m_Root.EnterScope();
}

//-------------------------------------

inline void CVProfile::Stop()
{
	if ( --m_enabled == 0 )
		m_Root.ExitScope();
}

//-------------------------------------

inline void CVProfile::EnterScope( const char *pszName )
{
	if ( m_enabled != 0 || !m_fAtRoot ) // if became disabled, need to unwind back to root before stopping
	{
		// Only account for vprof stuff on the primary thread.
		//if( !Plat_IsPrimaryThread() )
		//	return;

		if ( pszName != m_pCurNode->GetName() )
		{
			m_pCurNode = m_pCurNode->GetSubNode( pszName );
		}

		m_pCurNode->EnterScope();
		m_fAtRoot = false;
	}
}

//-------------------------------------

inline void CVProfile::ExitScope()
{
	if ( !m_fAtRoot || m_enabled != 0 )
	{
		// Only account for vprof stuff on the primary thread.
		//if( !Plat_IsPrimaryThread() )
		//	return;

		// ExitScope will indicate whether we should back up to our parent (we may
		// be profiling a recursive function)
		if (m_pCurNode->ExitScope())
		{
			m_pCurNode = m_pCurNode->GetParent();
		}
		m_fAtRoot = ( m_pCurNode == &m_Root );
	}
}

//-------------------------------------

inline void CVProfile::Pause()
{
	m_pausedEnabledDepth = m_enabled;
	m_enabled = 0;
	if ( !AtRoot() )
		m_Root.Pause();
}

//-------------------------------------

inline void CVProfile::Resume()
{
	m_enabled = m_pausedEnabledDepth;
	if ( !AtRoot() )
		m_Root.Resume();
}

//-------------------------------------

inline void CVProfile::Reset()
{
	m_Root.Reset();
	m_nFrames = 0;
}

//-------------------------------------

inline void CVProfile::ResetPeaks()
{
	m_Root.ResetPeak();
}

//-------------------------------------

inline void CVProfile::MarkFrame()
{
	if ( m_enabled )
	{
		++m_nFrames;
		m_Root.ExitScope();
		m_Root.MarkFrame();
		m_Root.EnterScope();
	}
}

//-------------------------------------

inline double CVProfile::GetTotalTimeSampled()
{
	return m_Root.GetTotalTime();
}

//-------------------------------------

inline double CVProfile::GetPeakFrameTime()
{
	return m_Root.GetPeakTime();
}

//-------------------------------------

inline double CVProfile::GetTimeLastFrame()
{
	return m_Root.GetCurTime();
}

//-------------------------------------

inline CVProfNode *CVProfile::GetRoot()
{
	return &m_Root;
}

//-----------------------------------------------------------------------------

inline CVProfScope::CVProfScope( const char * pszName )
{
	VProfCurrentProfile( ).EnterScope( pszName );
}

//-------------------------------------

inline CVProfScope::~CVProfScope()
{
	VProfCurrentProfile( ).ExitScope();
}

#endif	//SHARED_VPROF_H
