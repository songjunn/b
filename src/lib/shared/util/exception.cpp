//#pragma once
#define	WIN32_LEAN_AND_MEAN

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <ImageHlp.h>
#include <direct.h>
#else
#ifdef __linux__
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#endif
#endif

#include <time.h>
#include "exception.h"
#include "extendedtrace.h"

/*
Copyright 2001 Bruce Dawson, Cygnus Software
For more information, visit http://www.cygnus-software.com/papers/
or e-mail: comments@cygnus-software.com

This code may be redistributed as long as this notice is kept intact.
*/

/*
This source file contains the exception handler for recording error
information after crashes. See the Jan 1999 Game Developer Magazine
for information on how to hook it in.
*/

const int NumCodeBytes = 16;	// Number of code bytes to record.
const int MaxStackDump = 2048;	// Maximum number of DWORDS in stack dumps.
const int StackColumns = 8;		// Number of columns in stack dump.

#define	ONEK			1024
#define	SIXTYFOURK		(64*ONEK)
#define	ONEM			(ONEK*ONEK)
#define	ONEG			(ONEK*ONEK*ONEK)

// hprintf behaves similarly to printf, with a few vital differences.
// It uses wvsprintf to do the formatting, which is a system routine,
// thus avoiding C run time interactions. For similar reasons it
// uses WriteFile rather than fwrite.
// The one limitation that this imposes is that wvsprintf, and
// therefore hprintf, cannot handle floating point numbers.

static void hprintf(HANDLE LogFile, const char* Format, ...)
{
	char buffer[2000];	// wvsprintf never prints more than one K.

	va_list arglist;
	va_start( arglist, Format);
#ifdef _WIN32
	wvsprintf(buffer, Format, arglist);
#else
#ifdef __linux__
	vsprintf(buffer, Format, arglist);
#endif
#endif
	va_end( arglist);


#ifdef _WIN32
	DWORD NumBytes;
	WriteFile(LogFile, buffer, lstrlen(buffer), &NumBytes, 0);
#else
	write(LogFile, buffer, strlen(buffer));
#endif
}

// Print the specified FILETIME to output in a human readable format,
// without using the C run time.

#ifdef _WIN32
static void PrintTime(char *output, FILETIME TimeToPrint)
{
	WORD Date, Time;
	if (FileTimeToLocalFileTime(&TimeToPrint, &TimeToPrint) &&
				FileTimeToDosDateTime(&TimeToPrint, &Date, &Time))
	{
		// What a silly way to print out the file date/time. Oh well,
		// it works, and I'm not aware of a cleaner way to do it.
		wsprintf(output, "%d/%d/%d %02d:%02d:%02d",
					(Date / 32) & 15, Date & 31, (Date / 512) + 1980,
					(Time / 2048), (Time / 32) & 63, (Time & 31) * 2);
	}
	else
		output[0] = 0;
}

// Print information about a code module (DLL or EXE) such as its size,
// location, time stamp, etc.

static void ShowModuleInfo(HANDLE LogFile, HINSTANCE ModuleHandle)
{
	char ModName[MAX_PATH];
	__try
	{
		if (GetModuleFileName(ModuleHandle, ModName, sizeof(ModName)) > 0)
		{
			// If GetModuleFileName returns greater than zero then this must
			// be a valid code module address. Therefore we can try to walk
			// our way through its structures to find the link time stamp.
			IMAGE_DOS_HEADER *DosHeader = (IMAGE_DOS_HEADER*)ModuleHandle;
		    if (IMAGE_DOS_SIGNATURE != DosHeader->e_magic)
	    	    return;
			IMAGE_NT_HEADERS *NTHeader = (IMAGE_NT_HEADERS*)((char *)DosHeader
						+ DosHeader->e_lfanew);
		    if (IMAGE_NT_SIGNATURE != NTHeader->Signature)
	    	    return;
			// Open the code module file so that we can get its file date
			// and size.
			HANDLE ModuleFile = CreateFile(ModName, GENERIC_READ,
						FILE_SHARE_READ, 0, OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL, 0);
			char TimeBuffer[100] = "";
			DWORD FileSize = 0;
			if (ModuleFile != INVALID_HANDLE_VALUE)
			{
				FileSize = GetFileSize(ModuleFile, 0);
				FILETIME	LastWriteTime;
				if (GetFileTime(ModuleFile, 0, 0, &LastWriteTime))
				{
					wsprintf(TimeBuffer, " - file date is ");
					PrintTime(TimeBuffer + lstrlen(TimeBuffer), LastWriteTime);
				}
				CloseHandle(ModuleFile);
			}
			hprintf(LogFile, "%s, loaded at 0x%08x - %d bytes - %08x%s\r\n",
						ModName, ModuleHandle, FileSize,
						NTHeader->FileHeader.TimeDateStamp, TimeBuffer);
		}
	}
	// Handle any exceptions by continuing from this point.
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}
#else
#ifdef __linux__
static void PrintTime(char *output, FILETIME TimeToPrint)
{
	
}
static void ShowModuleInfo(HANDLE LogFile, HINSTANCE ModuleHandle)
{

}
#endif
#endif

// Scan memory looking for code modules (DLLs or EXEs). VirtualQuery is used
// to find all the blocks of address space that were reserved or committed,
// and ShowModuleInfo will display module information if they are code
// modules.

#ifdef _WIN32
static void RecordModuleList(HANDLE LogFile)
{
	hprintf(LogFile, "\r\n"
					 "\tModule list: names, addresses, sizes, time stamps "
			"and file times:\r\n");
	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	const size_t PageSize = SystemInfo.dwPageSize;
	// Set NumPages to the number of pages in the 4GByte address space,
	// while being careful to avoid overflowing ints.
	const size_t NumPages = 4 * size_t(ONEG / PageSize);
	size_t pageNum = 0;
	void *LastAllocationBase = 0;
	while (pageNum < NumPages)
	{
		MEMORY_BASIC_INFORMATION	MemInfo;
		if (VirtualQuery((void *)(pageNum * PageSize), &MemInfo,
					sizeof(MemInfo)))
		{
			if (MemInfo.RegionSize > 0)
			{
				// Adjust the page number to skip over this block of memory.
				pageNum += MemInfo.RegionSize / PageSize;
				if (MemInfo.State == MEM_COMMIT && MemInfo.AllocationBase >
							LastAllocationBase)
				{
					// Look for new blocks of committed memory, and try
					// recording their module names - this will fail
					// gracefully if they aren't code modules.
					LastAllocationBase = MemInfo.AllocationBase;
					ShowModuleInfo(LogFile, (HINSTANCE)LastAllocationBase);
				}
			}
			else
				pageNum += SIXTYFOURK / PageSize;
		}
		else
			pageNum += SIXTYFOURK / PageSize;
		// If VirtualQuery fails we advance by 64K because that is the
		// granularity of address space doled out by VirtualAlloc().
	}
}

// Record information about the user's system, such as processor type, amount
// of memory, etc.

static void RecordSystemInformation(HANDLE LogFile)
{
	FILETIME	CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	char TimeBuffer[100];
	PrintTime(TimeBuffer, CurrentTime);
	hprintf(LogFile, "Error occurred at %s.\r\n", TimeBuffer);
	char	ModuleName[MAX_PATH];
	if (GetModuleFileName(0, ModuleName, sizeof(ModuleName)) <= 0)
		lstrcpy(ModuleName, "Unknown");
	char	UserName[200];
	DWORD UserNameSize = sizeof(UserName);
	if (!GetUserName(UserName, &UserNameSize))
		lstrcpy(UserName, "Unknown");
	hprintf(LogFile, "%s, run by %s.\r\n", ModuleName, UserName);

	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	hprintf(LogFile, "%d processor(s), type %d.\r\n",
				SystemInfo.dwNumberOfProcessors, SystemInfo.dwProcessorType);

	MEMORYSTATUS	MemInfo;
	MemInfo.dwLength = sizeof(MemInfo);
	GlobalMemoryStatus(&MemInfo);
	// Print out the amount of physical memory, rounded up.
	hprintf(LogFile, "%d MBytes physical memory.\r\n", (MemInfo.dwTotalPhys +
				ONEM - 1) / ONEM);
}

// Translate the exception code into something human readable.

static const char *GetExceptionDescription(DWORD ExceptionCode)
{
	struct ExceptionNames
	{
		DWORD	ExceptionCode;
		const char*	ExceptionName;
	};

	ExceptionNames ExceptionMap[] =
	{
		{0x40010005, "a Control-C"},
		{0x40010008, "a Control-Break"},
		{0x80000002, "a Datatype Misalignment"},
		{0x80000003, "a Breakpoint"},
		{0xc0000005, "an Access Violation"},
		{0xc0000006, "an In Page Error"},
		{0xc0000017, "a No Memory"},
		{0xc000001d, "an Illegal Instruction"},
		{0xc0000025, "a Noncontinuable Exception"},
		{0xc0000026, "an Invalid Disposition"},
		{0xc000008c, "a Array Bounds Exceeded"},
		{0xc000008d, "a Float Denormal Operand"},
		{0xc000008e, "a Float Divide by Zero"},
		{0xc000008f, "a Float Inexact Result"},
		{0xc0000090, "a Float Invalid Operation"},
		{0xc0000091, "a Float Overflow"},
		{0xc0000092, "a Float Stack Check"},
		{0xc0000093, "a Float Underflow"},
		{0xc0000094, "an Integer Divide by Zero"},
		{0xc0000095, "an Integer Overflow"},
		{0xc0000096, "a Privileged Instruction"},
		{0xc00000fD, "a Stack Overflow"},
		{0xc0000142, "a DLL Initialization Failed"},
		{0xe06d7363, "a Microsoft C++ Exception"},
	};

	for (int i = 0; i < sizeof(ExceptionMap) / sizeof(ExceptionMap[0]); i++)
		if (ExceptionCode == ExceptionMap[i].ExceptionCode)
			return ExceptionMap[i].ExceptionName;

	return "Unknown exception type";
}

static char* GetFilePart(char *source)
{
	char *result = strrchr(source, '\\');
	if (result)
		result++;
	else
		result = source;
	return result;
}

static void SetReadOnly( LPCSTR szName, BOOL bReadOnly )
{
	
	DWORD dwAttribute = 0;
	dwAttribute = ::GetFileAttributes( szName );
	if( dwAttribute != 0xffffffff )
	{
		if( bReadOnly )
			dwAttribute |= FILE_ATTRIBUTE_READONLY;
		else
			dwAttribute &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes( szName, dwAttribute );
	}
}

static char s_specasc[] = { '!', '@', '#', '$', '%',
							'^', '&', '*', '(', ')',
							'_', '-', '+', '=', ',',
							'<', '>', '?', '/', '[',
							']', '|', '.', '\'','\\',
							':', ';', '\"' };

static bool IsSpecAscChar( char c )
{
	bool bOk = false;
	for ( int i = 0; i < sizeof(s_specasc); i++ )
	{
		if ( s_specasc[i] == c )
		{
			bOk = true;
			break;
		}
	}

	return bOk;
}

static bool IsAscii( char c )
{
	 if ( ( c >='a' && c <='z' ) ||
						   ( c >='A' && c <='Z' ) ||
						   ( c >='0' && c <='9' ) )
	 {
		 return true;
	 }

	return IsSpecAscChar( c );
}

static HANDLE s_logFile = NULL;
void WriteStackDetails( PCONTEXT pContext, bool bWriteVariables, HANDLE logFile );  // true if local/params should be output

static int RecordExceptionInfo(PEXCEPTION_POINTERS data, const char *Message)
{
	static int BeenHere;
	if (BeenHere)	// Going recursive! That must mean this routine crashed!
		return EXCEPTION_CONTINUE_SEARCH;
	BeenHere = true;

	char	ModuleName[MAX_PATH];
	char	FileName[MAX_PATH] = "Unknown";
	// Create a filename to record the error information to.
	// Storing it in the executable directory works well.
	if (GetModuleFileName(0, ModuleName, sizeof(ModuleName)) <= 0)
		ModuleName[0] = 0;
	char *FilePart = GetFilePart(ModuleName);

	// Extract the file name portion and remove it's file extension. We'll
	// use that name shortly.
	lstrcpy(FileName, FilePart);
	char *lastperiod = strrchr(FileName, '.');
	if (lastperiod)
		lastperiod[0] = 0;
	// Replace the executable filename with our error log file name.
	//strcat( FilePart , "\\log\\errorlog.txt");
	lstrcpy(FilePart, "\\log\\errorlog.txt");

	SetReadOnly( ModuleName, FALSE );
	HANDLE LogFile = CreateFile(ModuleName, GENERIC_WRITE, 0, 0,
				OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, 0);
	if (LogFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString("Error creating exception report");
		return EXCEPTION_CONTINUE_SEARCH;
	}
	// Append to the error log.
	SetFilePointer(LogFile, 0, 0, FILE_END);
	// Print out some blank lines to separate this error log from any previous ones.
	hprintf(LogFile, "\r\n\r\n\r\n\r\n");
	PEXCEPTION_RECORD	Exception = data->ExceptionRecord;
	PCONTEXT			Context = data->ContextRecord;

	char	CrashModulePathName[MAX_PATH];
	char	*CrashModuleFileName = "Unknown";
	MEMORY_BASIC_INFORMATION	MemInfo;
	// VirtualQuery can be used to get the allocation base associated with a
	// code address, which is the same as the ModuleHandle. This can be used
	// to get the filename of the module that the crash happened in.
#ifdef _X64
	if (VirtualQuery((void*)Context->Rip, &MemInfo, sizeof(MemInfo)) &&
#else
	if (VirtualQuery((void*)Context->Eip, &MemInfo, sizeof(MemInfo)) &&
#endif
				GetModuleFileName((HINSTANCE)MemInfo.AllocationBase,
							CrashModulePathName,
				sizeof(CrashModulePathName)) > 0)
		CrashModuleFileName = GetFilePart(CrashModulePathName);

	// Print out the beginning of the error log in a Win95 error window
	// compatible format.
	hprintf(LogFile, "%s caused %s in module %s at %04x:%08x.\r\n",
				FileName, GetExceptionDescription(Exception->ExceptionCode),
#ifdef _X64
				CrashModuleFileName, Context->SegCs, Context->Rip);
#else
				CrashModuleFileName, Context->SegCs, Context->Eip);
#endif
				
	hprintf(LogFile, "Exception handler called in %s.\r\n", Message);
	RecordSystemInformation(LogFile);
	// If the exception was an access violation, print out some additional
	// information, to the error log and the debugger.
	if (Exception->ExceptionCode == STATUS_ACCESS_VIOLATION &&
				Exception->NumberParameters >= 2)
	{
		char DebugMessage[1000];
		const char* readwrite = "Read from";
		if (Exception->ExceptionInformation[0])
			readwrite = "Write to";
		wsprintf(DebugMessage, "%s location %08x caused an access violation.\r\n",
					readwrite, Exception->ExceptionInformation[1]);
		#ifdef	_DEBUG
		// The VisualC++ debugger doesn't actually tell you whether a read
		// or a write caused the access violation, nor does it tell what
		// address was being read or written. So I fixed that.
		OutputDebugString("Exception handler: ");
		OutputDebugString(DebugMessage);
		#endif
		hprintf(LogFile, "%s", DebugMessage);
	}
	// Print out the register values in a Win95 error window compatible format.
	hprintf(LogFile, "\r\n");
	hprintf(LogFile, "Registers:\r\n");
#ifdef _X64
	hprintf(LogFile, "RAX=%16x CS=%04x RIP=%16x EFLGS=%08x\r\n",
				Context->Rax, Context->SegCs, Context->Rip, Context->EFlags);
	hprintf(LogFile, "RBX=%16x SS=%04x RSP=%16x RBP=%16x\r\n",
				Context->Rbx, Context->SegSs, Context->Rsp, Context->Rbp);
	hprintf(LogFile, "RCX=%16x DS=%04x RSI=%16x FS=%04x\r\n",
				Context->Rcx, Context->SegDs, Context->Rsi, Context->SegFs);
	hprintf(LogFile, "RDX=%16x ES=%04x RDI=%16x GS=%04x\r\n",
				Context->Rdx, Context->SegEs, Context->Rdi, Context->SegGs);
#else
	hprintf(LogFile, "EAX=%08x CS=%04x EIP=%08x EFLGS=%08x\r\n",
				Context->Eax, Context->SegCs, Context->Eip, Context->EFlags);
	hprintf(LogFile, "EBX=%08x SS=%04x ESP=%08x EBP=%08x\r\n",
				Context->Ebx, Context->SegSs, Context->Esp, Context->Ebp);
	hprintf(LogFile, "ECX=%08x DS=%04x ESI=%08x FS=%04x\r\n",
				Context->Ecx, Context->SegDs, Context->Esi, Context->SegFs);
	hprintf(LogFile, "EDX=%08x ES=%04x EDI=%08x GS=%04x\r\n",
				Context->Edx, Context->SegEs, Context->Edi, Context->SegGs);
#endif
	hprintf(LogFile, "Bytes at CS:EIP:\r\n");

	// Print out the bytes of code at the instruction pointer. Since the
	// crash may have been caused by an instruction pointer that was bad,
	// this code needs to be wrapped in an exception handler, in case there
	// is no memory to read. If the dereferencing of code[] fails, the
	// exception handler will print '??'.
#ifdef _X64
	unsigned char *code = (unsigned char*)Context->Rip;
#else
	unsigned char *code = (unsigned char*)Context->Eip;
#endif

	for (int codebyte = 0; codebyte < NumCodeBytes; codebyte++)
	{
		__try
		{
			hprintf(LogFile, "%02x ", code[codebyte]);

		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			hprintf(LogFile, "?? ");
		}
	}

	// Time to print part or all of the stack to the error log. This allows
	// us to figure out the call stack, parameters, local variables, etc.
	hprintf(LogFile, "\r\n"
					 "Stack dump:\r\n");
	__try
	{
		// Esp contains the bottom of the stack, or at least the bottom of
		// the currently used area.
//		DWORD* pStack = (DWORD *)Context->Esp;
		DWORD* pStackTop = NULL;
#ifdef _X64
	DWORD* pStack = (DWORD *)Context->Rsp;
	assert(0);
#else
		DWORD* pStack = (DWORD *)Context->Esp;
		__asm
		{
			// Load the top (highest address) of the stack from the
			// thread information block. It will be found there in
			// Win9x and Windows NT.
			mov	eax, fs:[4]
			mov pStackTop, eax
		}
#endif
		if (pStackTop > pStack + MaxStackDump)
			pStackTop = pStack + MaxStackDump;
		int Count = 0;
		// Too many calls to WriteFile can take a long time, causing
		// confusing delays when programs crash. Therefore I implemented
		// simple buffering for the stack dumping code instead of calling
		// hprintf directly.
		char	buffer[1000] = "";
		const int safetyzone = 50;
		char*	nearend = buffer + sizeof(buffer) - safetyzone;
		char*	output = buffer;
		int nLineCount = 0;
		while (pStack + 1 <= pStackTop)
		{
			if ((Count % StackColumns) == 0)
			{
				output += wsprintf(output, "%08x: ", pStack);
				nLineCount = 0;
			}
			
			++Count;
			++nLineCount;
			
			char *Suffix = " ";
			if ((Count % StackColumns) == 0 || pStack + 2 > pStackTop)
				Suffix = "\r\n";

			if ((Count % StackColumns) == 0 || pStack + 2 > pStackTop) // 结尾是ascii
			{	
				output += wsprintf(output, "%08x ; ", *pStack);
				memcpy( output, (pStack - nLineCount + 1), 4 * nLineCount );
				for ( int itmp = 0; itmp < (4 *nLineCount); itmp++ )
				{
					
					if ( !IsAscii(output[itmp] ) )
						output[itmp] = '.';
					
				}
				output += (4 * nLineCount);
				output += wsprintf( output, "%s",Suffix );
			}
			else
			{
				output += wsprintf(output, "%08x%s", *pStack, Suffix );
			}

			pStack++;
			// Check for when the buffer is almost full, and flush it to disk.
			if (output > nearend)
			{
				hprintf(LogFile, "%s", buffer);
				buffer[0] = 0;
				output = buffer;
			}
		}
		// Print out any final characters from the cache.
		hprintf(LogFile, "%s", buffer);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		hprintf(LogFile, "Exception encountered during stack dump.\r\n");
	}

	//by jinsheng  输出 stack walk 参数
	WriteStackDetails( Context, true, LogFile);

	RecordModuleList(LogFile);

	CloseHandle(LogFile);
	// Return the magic value which tells Win32 that this handler didn't
	// actually handle the exception - so that things will proceed as per
	// normal.
	return EXCEPTION_CONTINUE_SEARCH;
}

BOOL DumpMini(PEXCEPTION_POINTERS ExceptionInfo)
{
#ifdef UNICODE
	#define CreateFile  CreateFileW
#else
	#define CreateFile   CreateFileA
#endif

	//CTime time = CTime::GetCurrentTime();
	//CString strtime = time.Format(TEXT("%m.%d.%Y--%H.%M.%S"));

	struct tm* local;
	time_t curTime;
	time(&curTime);
	local = localtime(&curTime);
	char strtime[256] = {0};
	sprintf(strtime, "%d.%d.%d--%d.%d.%d",local->tm_year+1900, local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

	char filename[256] = {0};
	//Plat_GetExePath( filename, 256 );
	strcat(filename,"\\Log\\");
	mkdir( filename );

	strcat(filename,"Except_");
	strcat(filename,strtime);
	strcat(filename,".dmp");

	BOOL bDump = 0;
	HANDLE t_FileHandle = CreateFile( filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( t_FileHandle != INVALID_HANDLE_VALUE)
	{
		//[DLL_IMPORT("DbgHelp.dll")]
		// 设置输出信息
		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId();
		eInfo.ExceptionPointers = ExceptionInfo;
		eInfo.ClientPointers = FALSE;

		// 准备进程相关参数
		HANDLE hProc = GetCurrentProcess();
		DWORD nProc = GetCurrentProcessId();
		PMINIDUMP_EXCEPTION_INFORMATION pMDI = ExceptionInfo ? &eInfo : NULL;
		bDump = MiniDumpWriteDump(hProc,nProc,t_FileHandle,MiniDumpNormal,pMDI,NULL,NULL);
		CloseHandle(t_FileHandle);
	}
	return bDump;
}

BOOL DumpFull()
{
#ifdef UNICODE
	#define CreateFile  CreateFileW
#else
	#define CreateFile   CreateFileA
#endif

	struct tm* local;
	time_t curTime;
	time(&curTime);
	local = localtime(&curTime);
	char strtime[256] = {0};
	sprintf(strtime, "%d.%d.%d--%d.%d.%d",local->tm_year+1900, local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

	char filename[256] = {0};
	//Plat_GetExePath( filename, 256 );
	strcat(filename,"\\Log\\");
	mkdir( filename );

	strcat(filename,"Except_Full_");
	strcat(filename,strtime);
	strcat(filename,".dmp");

	BOOL bDump = 0;
	HANDLE t_FileHandle = CreateFile( filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( t_FileHandle != INVALID_HANDLE_VALUE)
	{
		// 准备进程相关参数
		HANDLE hProc = GetCurrentProcess();
		DWORD nProc = GetCurrentProcessId();
		bDump = MiniDumpWriteDump(
			hProc,
			nProc,
			t_FileHandle,
			(MINIDUMP_TYPE)(MiniDumpWithThreadInfo|MiniDumpWithFullMemoryInfo|MiniDumpWithUnloadedModules|MiniDumpWithHandleData|MiniDumpWithFullMemory),
			0,
			0,
			0);
		CloseHandle(t_FileHandle);
	}
	return bDump;
}

int HandleException(PEXCEPTION_POINTERS data, const char *Message)
{
	DumpMini(data);
	RecordExceptionInfo(data, Message);

	return EXCEPTION_EXECUTE_HANDLER;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
enum BasicType  // Stolen from CVCONST.H in the DIA 2.0 SDK
{
	btNoType = 0,
	btVoid = 1,
	btChar = 2,
	btWChar = 3,
	btInt = 6,
	btUInt = 7,
	btFloat = 8,
	btBCD = 9,
	btBool = 10,
	btLong = 13,
	btULong = 14,
	btCurrency = 25,
	btDate = 26,
	btVariant = 27,
	btComplex = 28,
	btBit = 29,
	btBSTR = 30,
	btHresult = 31
};


BOOL CALLBACK EnumerateSymbolsCallback(PSYMBOL_INFO  pSymInfo,
									   ULONG         SymbolSize,
									   PVOID         UserContext );

bool FormatSymbolValue(PSYMBOL_INFO pSym,
					   STACKFRAME * sf,
					   char * pszBuffer,
					   unsigned cbBuffer );


char * DumpTypeIndex(char * pszCurrBuffer,
					 DWORD64 modBase,
					 DWORD dwTypeIndex,
					 unsigned nestingLevel,
					 DWORD_PTR offset,
					 bool & bHandled );
BasicType 
GetBasicType( DWORD typeIndex, DWORD64 modBase );

char * FormatOutputValue(   char * pszCurrBuffer,
						 BasicType basicType,
						 DWORD64 length,
						 PVOID pAddress );



//=============================================================================
// Given a linear address, locates the module, section, and offset containing  
// that address.                                                               
//                                                                             
// Note: the szModule paramater buffer is an output buffer of length specified 
// by the len parameter (in characters!)                                       
//=============================================================================
BOOL GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset )
{
	MEMORY_BASIC_INFORMATION mbi;

	if ( !VirtualQuery( addr, &mbi, sizeof(mbi) ) )
		return FALSE;

	DWORD hMod = (DWORD)mbi.AllocationBase;

	if ( !GetModuleFileName( (HMODULE)hMod, szModule, len ) )
		return FALSE;

	// Point to the DOS header in memory
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;

	if (!pDosHdr)
	{
		return FALSE;
	}
	// From the DOS header, find the NT (PE) header
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);

	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );

	DWORD rva = (DWORD)addr - hMod; // RVA is offset from module load address

	// Iterate through the section table, looking for the one that encompasses
	// the linear address.
	for (   unsigned i = 0;
		i < pNtHdr->FileHeader.NumberOfSections;
		i++, pSection++ )
	{
		DWORD sectionStart = pSection->VirtualAddress;
		DWORD sectionEnd = sectionStart
			+ max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

		// Is the address in this section???
		if ( (rva >= sectionStart) && (rva <= sectionEnd) )
		{
			// Yes, address is in the section.  Calculate section and offset,
			// and store in the "section" & "offset" params, which were
			// passed by reference.
			section = i+1;
			offset = rva - sectionStart;
			return TRUE;
		}
	}

	return FALSE;   // Should never get here!
}

//============================================================
// Walks the stack, and writes the results to the report file 
//============================================================
void WriteStackDetails( PCONTEXT pContext, bool bWriteVariables, HANDLE logFile )  // true if local/params should be output
{

	s_logFile = logFile;
    hprintf(s_logFile, _T("\r\nCall stack:\r\n") );

    hprintf(s_logFile, _T("Address   Frame     Function            SourceFile\r\n") );

    DWORD dwMachineType = 0;
    // Could use SymSetOptions here to add the SYMOPT_DEFERRED_LOADS flag

    STACKFRAME sf;
    memset( &sf, 0, sizeof(sf) );

    #ifdef _M_IX86
    // Initialize the STACKFRAME structure for the first call.  This is only
    // necessary for Intel CPUs, and isn't mentioned in the documentation.
    sf.AddrPC.Offset       = pContext->Eip;
    sf.AddrPC.Mode         = AddrModeFlat;
    sf.AddrStack.Offset    = pContext->Esp;
    sf.AddrStack.Mode      = AddrModeFlat;
    sf.AddrFrame.Offset    = pContext->Ebp;
    sf.AddrFrame.Mode      = AddrModeFlat;

    dwMachineType = IMAGE_FILE_MACHINE_I386;
    #endif

	char filename[256] = {0};
	//Plat_GetExePath( filename, 256 );
	EXTENDEDTRACEINITIALIZE( filename );

    while ( 1 )
    {
        // Get the next stack frame
        if ( ! StackWalk(  dwMachineType,
                            GetCurrentProcess( ),
                            GetCurrentThread(),
                            &sf,
                            pContext,
                            0,
                            SymFunctionTableAccess,
                            SymGetModuleBase,
                            0 ) )
            break;

        if ( 0 == sf.AddrFrame.Offset ) // Basic sanity check to make sure
            break;                      // the frame is OK.  Bail if not.

        hprintf(s_logFile, _T("%08X  %08X  "), sf.AddrPC.Offset, sf.AddrFrame.Offset );

        // Get the name of the function for this stack frame entry
        uint8 symbolBuffer[ sizeof(SYMBOL_INFO) + 1024 ];
        PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbolBuffer;
        pSymbol->SizeOfStruct = sizeof(symbolBuffer);
        pSymbol->MaxNameLen = 1024;
                        
        DWORD64 symDisplacement = 0;    // Displacement of the input address,
                                        // relative to the start of the symbol

        if ( SymFromAddr( GetCurrentProcess(),sf.AddrPC.Offset,&symDisplacement,pSymbol))
        {
            hprintf(s_logFile, _T("%hs+%I64X"), pSymbol->Name, symDisplacement );
            
        }
        else    // No symbol found.  Print out the logical address instead.
        {
            TCHAR szModule[MAX_PATH] = _T("");
            DWORD section = 0, offset = 0;

			if (!GetLogicalAddress(  (PVOID)sf.AddrPC.Offset,szModule, sizeof(szModule), section, offset ))
            {
				return ;
            }
 
            hprintf(s_logFile, _T("%04X:%08X %s"), section, offset, szModule );
			continue; 
        }

        // Get the source line for this stack frame entry
        IMAGEHLP_LINE lineInfo = { sizeof(IMAGEHLP_LINE) };
        DWORD dwLineDisplacement;
        if ( SymGetLineFromAddr( GetCurrentProcess(), sf.AddrPC.Offset,
                                &dwLineDisplacement, &lineInfo ) )
        {
            hprintf(s_logFile,_T("  %s line %u"),lineInfo.FileName,lineInfo.LineNumber); 
        }

        hprintf(s_logFile, _T("\r\n") );

        // Write out the variables, if desired
        if ( bWriteVariables )
        {
            // Use SymSetContext to get just the locals/params for this frame
            IMAGEHLP_STACK_FRAME imagehlpStackFrame;
            imagehlpStackFrame.InstructionOffset = sf.AddrPC.Offset;
            SymSetContext( GetCurrentProcess(), &imagehlpStackFrame, 0 );

            // Enumerate the locals/parameters
            SymEnumSymbols( GetCurrentProcess(), 0, 0, EnumerateSymbolsCallback, &sf );

            hprintf(s_logFile, _T("\r\n") );
        }
    }
	EXTENDEDTRACEUNINITIALIZE( );

}

//////////////////////////////////////////////////////////////////////////////
// The function invoked by SymEnumSymbols
//////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnumerateSymbolsCallback(
    PSYMBOL_INFO  pSymInfo,
    ULONG         SymbolSize,
    PVOID         UserContext )
{

    char szBuffer[2048];

    __try
    {
        if ( FormatSymbolValue( pSymInfo, (STACKFRAME*)UserContext,
                                szBuffer, sizeof(szBuffer) ) )  
            hprintf(s_logFile, _T("\t%s\r\n"), szBuffer );
    }
    __except( 1 )
    {
        hprintf(s_logFile, _T("punting on symbol %s\r\n"), pSymInfo->Name );
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// Given a SYMBOL_INFO representing a particular variable, displays its
// contents.  If it's a user defined type, display the members and their
// values.
//////////////////////////////////////////////////////////////////////////////
bool FormatSymbolValue(PSYMBOL_INFO pSym, STACKFRAME * sf, char * pszBuffer, unsigned cbBuffer )
{
    char * pszCurrBuffer = pszBuffer;

    // Indicate if the variable is a local or parameter
    if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_PARAMETER )
        pszCurrBuffer += sprintf( pszCurrBuffer, "Parameter " );
    else if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_LOCAL )
        pszCurrBuffer += sprintf( pszCurrBuffer, "Local " );

    // If it's a function, don't do anything.
    if ( pSym->Tag == 5 )   // SymTagFunction from CVCONST.H from the DIA SDK
        return false;

    // Emit the variable name
    pszCurrBuffer += sprintf( pszCurrBuffer, "\'%s\'", pSym->Name );

    DWORD_PTR pVariable = 0;    // Will point to the variable's data in memory

    if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGRELATIVE )
    {
        // if ( pSym->Register == 8 )   // EBP is the value 8 (in DBGHELP 5.1)
        {                               //  This may change!!!
            pVariable = sf->AddrFrame.Offset;
            pVariable += (DWORD_PTR)pSym->Address;
			pVariable += 4;
        }
        // else
        //  return false;
    }
    else if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGISTER )
    {
        return false;   // Don't try to report register variable
    }
    else
    {
        pVariable = (DWORD_PTR)pSym->Address;   // It must be a global variable
    }

    // Determine if the variable is a user defined type (UDT).  IF so, bHandled
    // will return true.
    bool bHandled;
    pszCurrBuffer = DumpTypeIndex(pszCurrBuffer,pSym->ModBase, pSym->TypeIndex,
                                    0, pVariable, bHandled );

    if ( !bHandled )
    {
		
        // The symbol wasn't a UDT, so do basic, stupid formatting of the
        // variable.  Based on the size, we're assuming it's a char, WORD, or
        // DWORD.
        BasicType basicType = GetBasicType( pSym->TypeIndex, pSym->ModBase );
        
        pszCurrBuffer = FormatOutputValue(pszCurrBuffer, basicType, pSym->Size,
                                            (PVOID)pVariable ); 

    }

    return true;
}


//////////////////////////////////////////////////////////////////////////////
// If it's a user defined type (UDT), recurse through its members until we're
// at fundamental types.  When he hit fundamental types, return
// bHandled = false, so that FormatSymbolValue() will format them.
//////////////////////////////////////////////////////////////////////////////
char * DumpTypeIndex(char * pszCurrBuffer, DWORD64 modBase, DWORD dwTypeIndex, unsigned nestingLevel, DWORD_PTR offset, bool & bHandled)
{
	bHandled = false;

	// Get the name of the symbol.  This will either be a Type name (if a UDT),
	// or the structure member name.
	WCHAR * pwszTypeName;
	if ( SymGetTypeInfo( GetCurrentProcess(), modBase, dwTypeIndex, TI_GET_SYMNAME,
		&pwszTypeName ) )
	{
		pszCurrBuffer += sprintf( pszCurrBuffer, " %ls", pwszTypeName );
		LocalFree( pwszTypeName );
	}

	// Determine how many children this type has.
	DWORD dwChildrenCount = 0;
	SymGetTypeInfo( GetCurrentProcess(), modBase, dwTypeIndex, TI_GET_CHILDRENCOUNT,
		&dwChildrenCount );

	if ( !dwChildrenCount )     // If no children, we're done
		return pszCurrBuffer;

	// Prepare to get an array of "TypeIds", representing each of the children.
	// SymGetTypeInfo(TI_FINDCHILDREN) expects more memory than just a
	// TI_FINDCHILDREN_PARAMS struct has.  Use derivation to accomplish this.
	struct FINDCHILDREN : TI_FINDCHILDREN_PARAMS
	{
		ULONG   MoreChildIds[1024];
		FINDCHILDREN(){Count = sizeof(MoreChildIds) / sizeof(MoreChildIds[0]);}
	} children;

	children.Count = dwChildrenCount;
	children.Start= 0;

	// Get the array of TypeIds, one for each child type
	if ( !SymGetTypeInfo( GetCurrentProcess(), modBase, dwTypeIndex, TI_FINDCHILDREN,
		&children ) )
	{
		return pszCurrBuffer;
	}

	// Append a line feed
	pszCurrBuffer += sprintf( pszCurrBuffer, "\r\n" );

	// Iterate through each of the children
	for ( unsigned i = 0; i < dwChildrenCount; i++ )
	{
		// Add appropriate indentation level (since this routine is recursive)
		for ( unsigned j = 0; j <= nestingLevel+1; j++ )
			pszCurrBuffer += sprintf( pszCurrBuffer, "\t" );

		// Recurse for each of the child types
		bool bHandled2;
		pszCurrBuffer = DumpTypeIndex( pszCurrBuffer, modBase,
			children.ChildId[i], nestingLevel+1,
			offset, bHandled2 );

		// If the child wasn't a UDT, format it appropriately
		if ( !bHandled2 )
		{
			// Get the offset of the child member, relative to its parent
			DWORD dwMemberOffset;
			SymGetTypeInfo( GetCurrentProcess(), modBase, children.ChildId[i],
				TI_GET_OFFSET, &dwMemberOffset );

			// Get the real "TypeId" of the child.  We need this for the
			// SymGetTypeInfo( TI_GET_TYPEID ) call below.
			DWORD typeId;
			SymGetTypeInfo( GetCurrentProcess(), modBase, children.ChildId[i],
				TI_GET_TYPEID, &typeId );

			// Get the size of the child member
			ULONG64 length;
			SymGetTypeInfo(GetCurrentProcess(), modBase, typeId, TI_GET_LENGTH,&length);

			// Calculate the address of the member
			DWORD_PTR dwFinalOffset = offset + dwMemberOffset;

			BasicType basicType = GetBasicType(children.ChildId[i], modBase );

			pszCurrBuffer = FormatOutputValue( pszCurrBuffer, basicType,
				length, (PVOID)dwFinalOffset ); 

			pszCurrBuffer += sprintf( pszCurrBuffer, "\r\n" );
		}

	}

	bHandled = true;
	return pszCurrBuffer;
}

char * FormatOutputValue(char * pszCurrBuffer, BasicType basicType, DWORD64 length, PVOID pAddress)
{
	// Format appropriately (assuming it's a 1, 2, or 4 bytes (!!!)
	if ( length == 1 )
		pszCurrBuffer += sprintf( pszCurrBuffer, " = %X", *(PBYTE)pAddress );
	else if ( length == 2 )
		pszCurrBuffer += sprintf( pszCurrBuffer, " = %X", *(PWORD)pAddress );
	else if ( length == 4 )
	{
		if ( basicType == btFloat )
		{
			pszCurrBuffer += sprintf(pszCurrBuffer," = %f", *(PFLOAT)pAddress);
		}
		else if ( basicType == btChar )
		{
			if ( !IsBadStringPtr( *(PSTR*)pAddress, 32) )
			{
				pszCurrBuffer += sprintf( pszCurrBuffer, " = \"%.31s\"",
					*(PDWORD)pAddress );
			}
			else
				pszCurrBuffer += sprintf( pszCurrBuffer, " = %X",
				*(PDWORD)pAddress );
		}
		else
			pszCurrBuffer += sprintf(pszCurrBuffer," = %X", *(PDWORD)pAddress);
	}
	else if ( length == 8 )
	{
		if ( basicType == btFloat )
		{
			pszCurrBuffer += sprintf( pszCurrBuffer, " = %lf",
				*(double *)pAddress );
		}
		else
			pszCurrBuffer += sprintf( pszCurrBuffer, " = %I64X",
			*(DWORD64*)pAddress );
	}

	return pszCurrBuffer;
}

BasicType GetBasicType( DWORD typeIndex, DWORD64 modBase )
{
	BasicType basicType;
	if ( SymGetTypeInfo( GetCurrentProcess(), modBase, typeIndex,
		TI_GET_BASETYPE, &basicType ) )
	{
		return basicType;
	}

	// Get the real "TypeId" of the child.  We need this for the
	// SymGetTypeInfo( TI_GET_TYPEID ) call below.
	DWORD typeId;
	if (SymGetTypeInfo(GetCurrentProcess(),modBase, typeIndex, TI_GET_TYPEID, &typeId))
	{
		if ( SymGetTypeInfo( GetCurrentProcess(), modBase, typeId, TI_GET_BASETYPE,
			&basicType ) )
		{
			return basicType;
		}
	}

	return btNoType;
}
#else
#ifdef __linux__
static void RecordModuleList(HANDLE LogFile)
{
}
static void RecordSystemInformation(HANDLE LogFile)
{
}

// Translate the exception code into something human readable.

static const char *GetExceptionDescription(DWORD ExceptionCode)
{

	return "Unknown exception type";
}

static char* GetFilePart(char *source)
{
	return NULL;
}

static void SetReadOnly( LPCSTR szName, BOOL bReadOnly )
{
}

static char s_specasc[] = { '!', '@', '#', '$', '%',
							'^', '&', '*', '(', ')',
							'_', '-', '+', '=', ',',
							'<', '>', '?', '/', '[',
							']', '|', '.', '\'','\\',
							':', ';', '\"' };

static bool IsSpecAscChar( char c )
{
	bool bOk = false;
	for ( int i = 0; i < (int)sizeof(s_specasc); i++ )
	{
		if ( s_specasc[i] == c )
		{
			bOk = true;
			break;
		}
	}

	return bOk;
}

//static bool IsAscii( char c )
//{
//	return false;
//}

//static HANDLE s_logFile = NULL;
void WriteStackDetails( PCONTEXT pContext, bool bWriteVariables, HANDLE logFile );  // true if local/params should be output

//static int RecordExceptionInfo(PEXCEPTION_POINTERS data, const char *Message)
//{
//	return 0;
//}

BOOL DumpMini(PEXCEPTION_POINTERS ExceptionInfo)
{
	return false;
}

BOOL DumpFull()
{
	return false;
}

int HandleException(PEXCEPTION_POINTERS data, const char *Message)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
enum BasicType  // Stolen from CVCONST.H in the DIA 2.0 SDK
{
	btNoType = 0,
	btVoid = 1,
	btChar = 2,
	btWChar = 3,
	btInt = 6,
	btUInt = 7,
	btFloat = 8,
	btBCD = 9,
	btBool = 10,
	btLong = 13,
	btULong = 14,
	btCurrency = 25,
	btDate = 26,
	btVariant = 27,
	btComplex = 28,
	btBit = 29,
	btBSTR = 30,
	btHresult = 31
};


BOOL CALLBACK EnumerateSymbolsCallback(PSYMBOL_INFO  pSymInfo,
									   ULONG         SymbolSize,
									   PVOID         UserContext );

bool FormatSymbolValue(PSYMBOL_INFO pSym,
					   STACKFRAME * sf,
					   char * pszBuffer,
					   unsigned cbBuffer );


char * DumpTypeIndex(char * pszCurrBuffer,
					 DWORD64 modBase,
					 DWORD dwTypeIndex,
					 unsigned nestingLevel,
					 DWORD_PTR offset,
					 bool & bHandled );
BasicType 
GetBasicType( DWORD typeIndex, DWORD64 modBase );

char * FormatOutputValue(   char * pszCurrBuffer,
						 BasicType basicType,
						 DWORD64 length,
						 PVOID pAddress );



//=============================================================================
// Given a linear address, locates the module, section, and offset containing  
// that address.                                                               
//                                                                             
// Note: the szModule paramater buffer is an output buffer of length specified 
// by the len parameter (in characters!)                                       
//=============================================================================
BOOL GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset )
{
	return false;
}

//============================================================
// Walks the stack, and writes the results to the report file 
//============================================================
void WriteStackDetails( PCONTEXT pContext, bool bWriteVariables, HANDLE logFile )  // true if local/params should be output
{

}

//////////////////////////////////////////////////////////////////////////////
// The function invoked by SymEnumSymbols
//////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnumerateSymbolsCallback(
    PSYMBOL_INFO  pSymInfo,
    ULONG         SymbolSize,
    PVOID         UserContext )
{

	return false;
}

//////////////////////////////////////////////////////////////////////////////
// Given a SYMBOL_INFO representing a particular variable, displays its
// contents.  If it's a user defined type, display the members and their
// values.
//////////////////////////////////////////////////////////////////////////////
bool FormatSymbolValue(PSYMBOL_INFO pSym, STACKFRAME * sf, char * pszBuffer, unsigned cbBuffer )
{
	return false;
}


//////////////////////////////////////////////////////////////////////////////
// If it's a user defined type (UDT), recurse through its members until we're
// at fundamental types.  When he hit fundamental types, return
// bHandled = false, so that FormatSymbolValue() will format them.
//////////////////////////////////////////////////////////////////////////////
char * DumpTypeIndex(char * pszCurrBuffer, DWORD64 modBase, DWORD dwTypeIndex, unsigned nestingLevel, DWORD_PTR offset, bool & bHandled)
{
	return NULL;
}

char * FormatOutputValue(char * pszCurrBuffer, BasicType basicType, DWORD64 length, PVOID pAddress)
{
	return NULL;
}

BasicType GetBasicType( DWORD typeIndex, DWORD64 modBase )
{
	return btNoType;
}
#endif
#endif
