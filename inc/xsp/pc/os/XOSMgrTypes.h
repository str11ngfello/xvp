//*****************************************************************************
//*****************************************************************************
//     _  __         _    ______ 
//    | |/ /        | |  / / __ \
//    |   / ______  | | / / /_/ /
//   /   | /_____/  | |/ / ____/ 
//  /_/|_|          |___/_/       
//
//	Extensible Visualization Platform (X-VP) (c) 
//	Copyright 2000. All Rights Reserved.
// 
//	This source is protected by copyright laws. Unauthorized reproduction or 
//	distribution of this source will result in severe civil and criminal
//	penalties, and will be prosecuted to the maximum extent possible under law.
//	
//	Module Name:	XOSMgrTypes.h
//	Author:			Lynn Duke
//	Creation Date:	6-30-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************
#ifndef __X_OS_MGR_TYPES_H__
#define __X_OS_MGR_TYPES_H__

#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN		
#define NOMINMAX
#undef min
#undef max

#include <windows.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <list>
#include <deque>
#include <iomanip>
#include <math.h>
#include <stdio.h>
#include <string>
#include <assert.h>
#include <tchar.h>
using namespace std;

//#define USE_CUSTOM_LUA_BINDINGS
//#ifdef USE_CUSTOM_LUA_BINDINGS
//#define DECLARE_SCRIPTED_CLASS_REF_ID virtual XS32 nGetClassTableRefID();
//#else
#define DECLARE_SCRIPTED_CLASS_REF_ID
//#endif

#define SCRIPT_EXPORT_CLASS
#define SCRIPT_EXPORT_BASE_CLASS
#define SCRIPT_EXPORT_MEMBER
#define SCRIPT_EXPORT_CONSTRUCTOR
#define SCRIPT_EXPORT_FUNCTION
#define SCRIPT_EXPORT_ENUM




//*****************************************************************************
//Defines for super fast bit list of arbitrary size based on 32-bit ULONGs.
//These macros do no bounds checking and leave memory allocation
//responsibilities up to the user. 
//*****************************************************************************
#define X_SET_ULONG_BIT(num,bit)	(num |= bit);
#define X_CLEAR_ULONG_BIT(num,bit)	(num &= ~bit);
#define X_TEST_ULONG_BIT(num,bit)	(num & bit) 

#define X_CREATE_BIT_ARRAY(bitArrayPointer,numBits) \
	{XNewArray(bitArrayPointer,XU32,1 + (XU32((numBits - 1) >> 5)),XOSMgr::getDefaultHeap());}
	//{bitArrayPointer = new XU32[1 + (XU32((numBits - 1) >> 5))];}


#define X_DESTROY_BIT_ARRAY(bitArrayPointer) \
	{if (bitArrayPointer) \
		delete[] bitArrayPointer;}

#define X_IS_BIT_SET(bitArrayPointer,pos) \
	(bitArrayPointer[XU32(pos >> 5)] & (1 << (pos & 0x0000001F)))

#define X_SET_BIT(bitArrayPointer,pos) \
	{bitArrayPointer[XU32(pos >> 5)] |= 1 << (pos & 0x0000001F);}

#define X_CLEAR_BIT(bitArrayPointer,pos) \
	{bitArrayPointer[XU32(pos >> 5)] &= ~(1 << (pos & 0x0000001F));}

#define X_CLEAR_BIT_ARRAY(bitArrayPointer,numBits) \
	{memset((void*)bitArrayPointer,0,(4 + ((XU32(numBits - 1)) >> 5) * 4));}



//*****************************************************************************
//XSP Configuration
//*****************************************************************************
//Select the XSP.  Only one define should be uncommented or defined in project
//settings.
//#define XSP_WINDOWS
//#define XSP_LINUX
//#define XSP_MAC

//*****************************************************************************
//Build Configuration
//*****************************************************************************
//Select a build configuration.  Only one should be uncommented or defined
//in project settings.
//#define XVP_DEBUG
//#define XVP_RELEASE
//#define XVP_FINAL

//*****************************************************************************
//Editor Configuration
//*****************************************************************************
//Define this to include functionality required by the XVP editor or include
//this define in project settings
//#define XVP_EDITOR



//*****************************************************************************
//Defines for Data Types
//*****************************************************************************

//XU8 should map to a 1 byte unsigned value
#undef XU8
#define XU8 unsigned char

//XS8 should map to a 1 byte signed value
#undef XS8
#define XS8 char

//XU16 should map to a 2 byte unsigned value
#undef XU16
#define XU16 unsigned short

//XS16 should map to a 2 byte signed value
#undef XS16
#define XS16 short

//XU32 should map to a 4 byte unsigned value
#undef XU32
#define XU32 unsigned int

//XS32 should map to a 4 byte signed value
#undef XS32
#define XS32 int

//XS64 should map to an 8 byte integer
#undef XS64
#define XS64 signed long long

//XU64 should map to an 8 byte integer
#undef XU64
#define XU64 unsigned long long

//XF32 should map to a 4 byte real
#undef XF32
#define XF32 float

//XF64 should map to a 8 byte real
#undef XF64
#define XF64 double

//XBOOL should map to a boolean
#undef XBOOL
#define XBOOL bool

//Integer type booleans, FALSE = 0 and TRUE = 1
#undef FALSE
#define FALSE   0
#undef TRUE
#define TRUE    1

//NULL should be 0
#undef NULL
#define NULL    0


#define X_INVALID_HANDLE	0
typedef XU32 XHandle;

#define X_MAX_SHADER_CONTROLLER_NAME_LENGTH		64


//*****************************************************************************
//! Standard status reporting defines for X-VP.
/**
These are the standard X-VP return codes.  Many functions throughout the
SDK use these return codes to indicate their level of success upon completion.
*/
//*****************************************************************************
enum XStatus
{
	X_OK,							/*!< Successful completion. */  
	X_ERR_MEMORY_EXHAUSTED,			/*!< Memory was exhausted during operation. */
	X_ERR_INVALID_PARAMS,			/*!< Invalid function parameters were received. */
	X_ERR_NOT_SUPPORTED,			/*!< Operation not supported in current version. */
	X_ERR_REFERENCES_EXIST,			/*!< References still exist. */
	X_ERR_NO_SUCH_OBJECT,			/*!< Object could not be found. */
	X_ERR_OPERATION_FAILED,			/*!< The requested operation failed. */
	X_ERR_LIMIT_REACHED,			/*!< Reached a limit that stopped processing. */		
	X_ERR_NOT_RUNNING,				/*!< Object has been shutdown/closed/not started. */
	X_ERR_NOT_READY,				/*!< Object not ready for the requested operation. */
	X_ERR_NOT_INITIALIZED,			/*!< Object not initialized. */
	X_ERR_PARTIAL_FAILURE,			/*!< The function partially failed. */
	X_ERR_REDUNDANT_OPERATION,		/*!< The opeation has already been done. */	
	X_ERR_WRONG_NUMBER_OF_PARAMS,	/*!< Wrong number of parameters specified. */
	X_ERR_NO_EFFECT					/*!< Operation had no effect. */
};


class XMaterial;
class XSceneNode;

//*****************************************************************************
//*****************************************************************************
enum X_PATH_TYPE
{
	X_PATH_SYSTEM,
	X_PATH_TEXTURE,
	X_PATH_MOVIE,
	X_PATH_AUDIO,
	X_PATH_UI_TEMPLATE,
	X_PATH_SHADER
};


//*****************************************************************************
//*****************************************************************************
enum 
{
	X_DLL_NONE,
	X_DLL_WARNING,
	X_DLL_ASSERT,
	X_DLL_INFO,
	X_DLL_VERBOSE
};



//*****************************************************************************
//*****************************************************************************
enum X_ASSERT_ACTION_TYPE
{
	X_AAT_IGNORE_SINGLE,
	X_AAT_IGNORE_MULTIPLE,
	X_AAT_IGNORE_ALL,
	X_AAT_BREAK
};



//*****************************************************************************
//X-VP Internal Configuration Defines
//*****************************************************************************
#define X_MAX_PATH_SIZE				512		
#define X_SCENE_ROOT_NAME			"SCENE_ROOT"
#define X_TYPE_DESC_LEN				64
#define X_STATE_DESC_LEN			30



//*****************************************************************************
//Debug Logging Configuration
//*****************************************************************************
#ifndef XVP_FINAL

#define XVP_PROFILER
#define XVP_CONSOLE
#define XVP_XDUL
#define XVP_STATS

#define X_DEBUG(x) x
#define X_DEBUG_LOG_INDENT_INC(increment)		{XOSMgr::incrementDebugIndent(increment);}	
#define X_DEBUG_LOG_INDENT_DEC(decrement)		{XOSMgr::decrementDebugIndent(decrement);}	
#define X_DEBUG_LOG_GET_INDENT					{XOSMgr::getDebugIndent;}	
#define X_DEBUG_LOG_SET_LEVEL(level)			{XOSMgr::setDebugLevel(level);}	
#define X_DEBUG_LOG_GET_LEVEL					{XOSMgr::getDebugLevel;}	
#define X_DEBUG_LOG_LN(string,debugLevel)		{XOSMgr::logDebugString(string,__FILE__,__LINE__,debugLevel,true);}
#define X_DEBUG_LOG_LNF(string,debugLevel,...)	{XS8 buf[4096]; sprintf(buf,string,__VA_ARGS__); XOSMgr::logDebugString(buf,__FILE__,__LINE__,debugLevel,true);}
#define X_DEBUG_LOG(string,debugLevel)			{XOSMgr::logDebugString(string,__FILE__,__LINE__,debugLevel,false);}
#define X_DEBUG_LOGF(string,debugLevel,...)		{XS8 buf[4096]; sprintf(buf,string,__VA_ARGS__); XOSMgr::logDebugString(buf,__FILE__,__LINE__,debugLevel,false);}

#define X_DEBUG_BREAK DebugBreak();


#define X_ASSERTF(condition,formatString,...)if (!(condition)) \
{	\
	static XS32 numAssertsToIgnore = 0; \
	if (numAssertsToIgnore >= 0 && XOSMgr::enableAsserts()) \
	{	\
		XS8 formattedString[4096]; sprintf(formattedString,formatString,__VA_ARGS__); \
		X_ASSERT_ACTION_TYPE action = XOSMgr::getAssertAction(formattedString); \
		if (action == X_AAT_IGNORE_SINGLE) {numAssertsToIgnore = 1; X_DEBUG_LOGF(formattedString,X_DLL_ASSERT,__VA_ARGS__); X_DEBUG_LOG_LN("(ASSERT action: ignore single)",X_DLL_NONE);}	\
		else if (action == X_AAT_IGNORE_MULTIPLE){ numAssertsToIgnore = -1; X_DEBUG_LOGF(formattedString,X_DLL_ASSERT,__VA_ARGS__); X_DEBUG_LOG_LN("(ASSERT action: ignore multiple)",X_DLL_NONE);}	\
		else if (action == X_AAT_IGNORE_ALL) { XOSMgr::enableAsserts(false); X_DEBUG_LOGF(formattedString,X_DLL_ASSERT,__VA_ARGS__); X_DEBUG_LOG_LN("(ASSERT action: ignore all)",X_DLL_NONE);}	\
		else if (action == X_AAT_BREAK) { X_DEBUG_LOGF(formattedString,X_DLL_ASSERT,__VA_ARGS__); X_DEBUG_LOG_LN("(ASSERT action: debug break)",X_DLL_NONE); X_DEBUG_BREAK}	\
		else { X_DEBUG_LOGF(formattedString,X_DLL_ASSERT,__VA_ARGS__); X_DEBUG_LOG_LN("(ASSERT action: unknown)",X_DLL_NONE); X_DEBUG_BREAK }	\
		if (numAssertsToIgnore >= 0) --numAssertsToIgnore; \
	}	\
}

#define X_ASSERTM(condition,outputString)if (!(condition)) \
{	\
	static XS32 numAssertsToIgnore = 0; \
	if (numAssertsToIgnore >= 0 && XOSMgr::enableAsserts()) \
	{	\
		X_ASSERT_ACTION_TYPE action = XOSMgr::getAssertAction(outputString); \
		if (action == X_AAT_IGNORE_SINGLE) {numAssertsToIgnore = 1; X_DEBUG_LOG(outputString,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: ignore single)",X_DLL_NONE);}	\
		else if (action == X_AAT_IGNORE_MULTIPLE){ numAssertsToIgnore = -1; X_DEBUG_LOG(outputString,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: ignore multiple)",X_DLL_NONE);}	\
		else if (action == X_AAT_IGNORE_ALL) { XOSMgr::enableAsserts(false); X_DEBUG_LOG(outputString,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: ignore all)",X_DLL_NONE);}	\
		else if (action == X_AAT_BREAK) { X_DEBUG_LOG(outputString,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: debug break)",X_DLL_NONE); X_DEBUG_BREAK}	\
		else { X_DEBUG_LOG(outputString,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: unknown)",X_DLL_NONE); X_DEBUG_BREAK }	\
		if (numAssertsToIgnore >= 0) --numAssertsToIgnore; \
	}	\
}

#define X_ASSERT(condition) if (!(condition)) \
{	\
	static XS32 numAssertsToIgnore = 0; \
	if (numAssertsToIgnore >= 0 && XOSMgr::enableAsserts()) \
	{	\
		X_ASSERT_ACTION_TYPE action = XOSMgr::getAssertAction(#condition); \
		if (action == X_AAT_IGNORE_SINGLE) {numAssertsToIgnore = 1; X_DEBUG_LOG(#condition,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: ignore single)",X_DLL_NONE);}	\
		else if (action == X_AAT_IGNORE_MULTIPLE){ numAssertsToIgnore = -1; X_DEBUG_LOG(#condition,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: ignore multiple)",X_DLL_NONE);}	\
		else if (action == X_AAT_IGNORE_ALL) { XOSMgr::enableAsserts(false); X_DEBUG_LOG(#condition,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: ignore all)",X_DLL_NONE);}	\
		else if (action == X_AAT_BREAK) { X_DEBUG_LOG(#condition,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: debug break)",X_DLL_NONE); X_DEBUG_BREAK}	\
		else { X_DEBUG_LOG(#condition,X_DLL_ASSERT); X_DEBUG_LOG_LN("(ASSERT action: unknown)",X_DLL_NONE); X_DEBUG_BREAK }	\
		if (numAssertsToIgnore >= 0) --numAssertsToIgnore; \
	}	\
}



#define X_ASSERT_FORCED(outputString) {X_DEBUG_LOG_LN(outputString,X_DLL_ASSERT) X_DEBUG_BREAK exit(0);} 
#define X_WARNING(outputString) {X_DEBUG_LOG_LN(outputString,X_DLL_WARNING)} 
#define X_WARNING_ONCE(outputString)  \
{	\
	static XBOOL once = false; \
	if (!once) \
	{	\
		X_DEBUG_LOG_LN(outputString,X_DLL_WARNING) \
		X_DEBUG_LOG_LN("Previous warning will not be repeated",X_DLL_WARNING)	\
		once = true; \
	} \
}


#define LUA_ASSERT(poLuaState, bTest, szMsg)															\
	/*if(!(bTest))																						\
	{																									\
		XS8 szLine[8];																				\
		_itoa(__LINE__, szLine, 10);																	\
		std::string str(__FILE__);																		\
		str = str.substr(str.find_last_of('\\')+1) + " (" + szLine + ')' + ": \"" + #bTest + '"'		\
			+ '\n' + (XS8*)szMsg + '\n' + '\n';													\
		X_ASSERT_FORCED(str.c_str());											\
	}*/

#else //else debug logging is not enabled

#define X_DEBUG(x)
#define X_DEBUG_LOG_INDENT_INC(increment)		
#define X_DEBUG_LOG_INDENT_DEC(decrement)	
#define X_DEBUG_LOG_GET_INDENT				
#define X_DEBUG_LOG_SET_LEVEL(level)		
#define X_DEBUG_LOG_GET_LEVEL				
#define X_DEBUG_LOG_LN(string,debugLevel)	
#define X_DEBUG_LOG(string,debugLevel)
#define X_ASSERTM(condition,outputString) 
#define X_ASSERT_FORCED(outputString)
#define X_WARNING(outputString)
#define X_WARNING_ONCE(outputString)
#define LUA_ASSERT(poLuaState, bTest, szMsg)
#endif //XVP_FINAL


//*****************************************************************************
//Profiler Configuration
//*****************************************************************************
#ifdef XVP_PROFILER 
#define X_PROFILER(x) x
#define X_START_PROFILE(pName)	{if (XProfiler::isInitialized()) XProfiler::startProfile(pName);}
#define X_STOP_PROFILE()		{if (XProfiler::isInitialized()) XProfiler::stopProfile();}
#else
#define X_PROFILER(x)
#define X_START_PROFILE(pName)	
#define X_STOP_PROFILE()		
#endif



//*****************************************************************************
//Console Configuration
//*****************************************************************************
#ifdef XVP_CONSOLE 
#define X_CONSOLE(x) x
#define X_CMD_RESPONSE_BUF_SIZE		4096
#define X_CONSOLE_MAX_CHARS			4096
#define X_CONSOLE_Z_ORDER			-1000000
#else
#define X_CONSOLE(x)
#endif


//*****************************************************************************
//Debug Utility Library Configuration
//*****************************************************************************
#ifdef XVP_XDUL 
#define X_DUL(x) x
#else
#define X_DUL(x)
#endif



//*****************************************************************************
//Statistics Configuration
//*****************************************************************************
#ifdef XVP_STATS 
#define X_STATS(x) x
#else
#define X_STATS(x)
#endif



//*****************************************************************************
//! File mode flags.
/**
These defines specify different types of file access.
*/
//************************************* ****************************************
enum X_FILE_MODE_FLAGS
{
	X_FMF_READ		= 0x00000001,	/*!< Open for reading only; place the file pointer at the beginning of the file. */
	X_FMF_WRITE		= 0x00000002,	/*!< Open for writing only; place the file pointer at the beginning of the file and truncate the file to zero length. If the file does not exist, attempt to create it. */
	X_FMF_BINARY	= 0x00000004,	/*!< Open in binary mode.  If this flag is not specified, the mode defaults to text mode. */
	X_FMF_APPEND	= 0x00000008,	/*!< After opening the file, position file pointer to at the end of the file. */
};


//*****************************************************************************
//*****************************************************************************
class XPointerContainer
{
public:
	void* pUserPointer;
	XPointerContainer* pPrev;
	XPointerContainer* pNext;

	XPointerContainer::XPointerContainer()
	{
		pUserPointer = NULL;
		pPrev = NULL;
		pNext = NULL;
	}
};



//*****************************************************************************
//Convience Macros for Memory Allocation
//*****************************************************************************
#ifdef XVP_FINAL
#define XNew(pointer,constructor,heap) {pointer = new (heap) constructor;}
#define XNewArray(pointer,dataType,numElements,heap) {pointer = new (heap) dataType[numElements];}
#define XNewArrayFill(pointer,dataType,numElements,fillValue,heap) { XNewArray(pointer,dataType,numElements,heap); memset(pointer,fillValue,sizeof(dataType)*numElements);}
#define XDelete(pointer) {if (pointer){delete pointer;pointer = NULL;}}
#define XDeleteArray(pointer) {if (pointer){delete[] pointer;pointer = NULL;}}

//#define XNew(pointer,dataType,heap) {pointer = new (heap) dataType();}
//#define XNewArray(pointer,dataType,size,heap) {pointer = new (heap) dataType[size];}
//#define XDelete(pointer) {if (pointer){delete pointer;pointer = NULL;}}
//#define XDeleteArray(pointer) {if (pointer){delete[] pointer;pointer = NULL;}}
#else //XVP_FINAL
#define XNew(pointer,constructor,heap) {pointer = new (heap,__FILE__,__LINE__) constructor;}
#define XNewArray(pointer,dataType,numElements,heap) {pointer = new (heap,__FILE__,__LINE__) dataType[numElements];}
#define XNewArrayFill(pointer,dataType,numElements,fillValue,heap) { XNewArray(pointer,dataType,numElements,heap); memset(pointer,fillValue,sizeof(dataType)*numElements);}
#define XDelete(pointer) {if (pointer){delete pointer;pointer = NULL;}}
#define XDeleteArray(pointer) {if (pointer){delete[] pointer;pointer = NULL;}}
#endif //XVP_FINAL

//base class for classes accessible from scripts
class EgScScriptedClass
{
#ifdef USE_CUSTOM_LUA_BINDINGS
	public:
		virtual XS32 nGetClassTableRefID() = 0;
#endif
};

#endif

