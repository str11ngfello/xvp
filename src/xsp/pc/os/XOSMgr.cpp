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
//	Module Name:	XOSMgr.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//#ifndef XVP_FINAL
XU32 XOSMgr::m_debugLoggingIndentAmount = 0;
XU32 XOSMgr::m_debugLoggingLevel = X_DLL_INFO;
XU32 XOSMgr::m_lineNumber = 0;
//#endif

XHeap* XOSMgr::m_pStringHeap = NULL;
XHeap* XOSMgr::m_pDebugHeap = NULL;
XHeap* XOSMgr::m_pAnimHeap = NULL;
XHeap* XOSMgr::m_pCameraHeap = NULL;
XHeap* XOSMgr::m_pModelHeap = NULL;

XHeap* XOSMgr::m_pFreeBlocksHeap1 = NULL;
XHeap* XOSMgr::m_pFreeBlocksHeap2 = NULL;
XHeap* XOSMgr::m_pFreeBlocksHeap3 = NULL;
XHeap* XOSMgr::m_pFreeBlocksHeap4 = NULL;
XHeap* XOSMgr::m_pFreeBlocksHeap5 = NULL;
XHeap* XOSMgr::m_pFreeBlocksHeap6 = NULL;
XHeap* XOSMgr::m_pFreeBlocksHeap7 = NULL;
XHeap* XOSMgr::m_pFreeBlocksHeap8 = NULL;
XHeap* XOSMgr::m_pFreeBlocksHeap9 = NULL;
char XOSMgr::m_systemPath[X_MAX_PATH_SIZE] = "";
char XOSMgr::m_texturePath[X_MAX_PATH_SIZE] = "";
char XOSMgr::m_moviePath[X_MAX_PATH_SIZE] = "";
char XOSMgr::m_audioPath[X_MAX_PATH_SIZE] = "";
char XOSMgr::m_UITemplatePath[X_MAX_PATH_SIZE] = "";
char XOSMgr::m_shaderPath[X_MAX_PATH_SIZE] = "";


XFreeList<XOSMgr::privateSizeStruct1>	XOSMgr::m_freeBlockList1;
XFreeList<XOSMgr::privateSizeStruct2>	XOSMgr::m_freeBlockList2;
XFreeList<XOSMgr::privateSizeStruct3>	XOSMgr::m_freeBlockList3;
XFreeList<XOSMgr::privateSizeStruct4>	XOSMgr::m_freeBlockList4;
XFreeList<XOSMgr::privateSizeStruct5>	XOSMgr::m_freeBlockList5;
XFreeList<XOSMgr::privateSizeStruct6>	XOSMgr::m_freeBlockList6;
XFreeList<XOSMgr::privateSizeStruct7>	XOSMgr::m_freeBlockList7;
XFreeList<XOSMgr::privateSizeStruct8>	XOSMgr::m_freeBlockList8;
XFreeList<XOSMgr::privateSizeStruct9>	XOSMgr::m_freeBlockList9;
XU32	XOSMgr::m_totalFreeBlockAlloc1 = 0;
XU32	XOSMgr::m_totalFreeBlockAlloc2 = 0;
XU32	XOSMgr::m_totalFreeBlockAlloc3 = 0;
XU32	XOSMgr::m_totalFreeBlockAlloc4 = 0;
XU32	XOSMgr::m_totalFreeBlockAlloc5 = 0;
XU32	XOSMgr::m_totalFreeBlockAlloc6 = 0;
XU32	XOSMgr::m_totalFreeBlockAlloc7 = 0;
XU32	XOSMgr::m_totalFreeBlockAlloc8 = 0;
XU32	XOSMgr::m_totalFreeBlockAlloc9 = 0;
XU32	XOSMgr::m_numFreeBlockAllocs1 = 0;
XU32	XOSMgr::m_numFreeBlockAllocs2 = 0;
XU32	XOSMgr::m_numFreeBlockAllocs3 = 0;
XU32	XOSMgr::m_numFreeBlockAllocs4 = 0;
XU32	XOSMgr::m_numFreeBlockAllocs5 = 0;
XU32	XOSMgr::m_numFreeBlockAllocs6 = 0;
XU32	XOSMgr::m_numFreeBlockAllocs7 = 0;
XU32	XOSMgr::m_numFreeBlockAllocs8 = 0;
XU32	XOSMgr::m_numFreeBlockAllocs9 = 0;

XBOOL	XOSMgr::m_memInited = false;
XBOOL	XOSMgr::m_genLogFile = false;
XBOOL	XOSMgr::m_assertsEnabled = true;
XBOOL	XOSMgr::m_internalFreeListsEnabled = true;

void*	XOSMgr::m_pProcessHandle = NULL;

//*****************************************************************************
//*****************************************************************************
XStatus XOSMgr::init(XVPDesc& initDesc)
{
	X_DEBUG_LOG_LN("\n*** BEGIN OS MANAGER INIT ***",X_DLL_INFO);

	m_pProcessHandle = initDesc.pProcessHandle;

	if (!m_pDebugHeap)
		m_pDebugHeap = createHeap("Debug");
	if (!m_pAnimHeap)
		m_pAnimHeap = createHeap("Animation");
	if (!m_pCameraHeap)
		m_pCameraHeap = createHeap("Camera");
	if (!m_pModelHeap)
		m_pModelHeap = createHeap("Model");

	initFreeMemBlocks();

	m_genLogFile = initDesc.generateLogFile;
	if (initDesc.generateLogFile)
	{
		ofstream f("XVPLog.txt");
		f.close();
	}

	m_assertsEnabled = initDesc.enableAsserts;
	m_internalFreeListsEnabled = initDesc.enableInternalFreeLists;

	X_DEBUG_LOG_LN("*** END OS MANAGER INIT ***",X_DLL_INFO);

	return X_OK;
}




//***************************************************************************** 
/**
Shuts down the OS Manager.  This routine is called by X-VP during
shutdown and gives the manager an opportunity to clean up any remaining
resources and shut down the OS.

@return A return code of type XStatus.
*/ 
//***************************************************************************** 
XStatus XOSMgr::destroy()
{
	m_freeBlockList1.destroy();
	m_freeBlockList2.destroy();
	m_freeBlockList3.destroy();
	m_freeBlockList4.destroy();
	m_freeBlockList5.destroy();
	m_freeBlockList6.destroy();
	m_freeBlockList7.destroy();
	m_freeBlockList8.destroy();
	m_freeBlockList9.destroy();

	m_memInited = false;
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_ASSERT_ACTION_TYPE XOSMgr::getAssertAction(XS8* outputString)
{
	//If we are in full screen mode, present a message box, otherwise just exit(0)
	//TODO: implement a fullscreen graphical message box instead of exiting.

	XS32 result = MessageBox(NULL,outputString,"",MB_ABORTRETRYIGNORE);
	if (result == IDABORT)
		return X_AAT_BREAK;
	if (result == IDRETRY)
		return X_AAT_IGNORE_SINGLE;
	if (result == IDIGNORE)
		return X_AAT_IGNORE_MULTIPLE;

	return X_AAT_BREAK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOSMgr::enableAsserts(XBOOL enable)
{
	m_assertsEnabled = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XOSMgr::enableAsserts()
{
	return m_assertsEnabled;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOSMgr::initFreeMemBlocks()
{
	XS8 nameBuf[30];
	sprintf(nameBuf,"FreeBlocks%d",sizeof(privateSizeStruct1));
	m_pFreeBlocksHeap1 = createHeap(nameBuf);
	sprintf(nameBuf,"FreeBlocks%d",sizeof(privateSizeStruct2));
	m_pFreeBlocksHeap2 = createHeap(nameBuf);
	sprintf(nameBuf,"FreeBlocks%d",sizeof(privateSizeStruct3));
	m_pFreeBlocksHeap3 = createHeap(nameBuf);
	sprintf(nameBuf,"FreeBlocks%d",sizeof(privateSizeStruct4));
	m_pFreeBlocksHeap4 = createHeap(nameBuf);
	sprintf(nameBuf,"FreeBlocks%d",sizeof(privateSizeStruct5));
	m_pFreeBlocksHeap5 = createHeap(nameBuf);
	sprintf(nameBuf,"FreeBlocks%d",sizeof(privateSizeStruct6));
	m_pFreeBlocksHeap6 = createHeap(nameBuf);
	sprintf(nameBuf,"FreeBlocks%d",sizeof(privateSizeStruct7));
	m_pFreeBlocksHeap7 = createHeap(nameBuf);
	sprintf(nameBuf,"FreeBlocks%d",sizeof(privateSizeStruct8));
	m_pFreeBlocksHeap8 = createHeap(nameBuf);
	sprintf(nameBuf,"FreeBlocks%d",sizeof(privateSizeStruct9));
	m_pFreeBlocksHeap9 = createHeap(nameBuf);
	
	m_freeBlockList1.init(8192,m_pFreeBlocksHeap1->GetName(),m_pFreeBlocksHeap1);
	m_freeBlockList2.init(2048,m_pFreeBlocksHeap2->GetName(),m_pFreeBlocksHeap2);
	m_freeBlockList3.init(8192,m_pFreeBlocksHeap3->GetName(),m_pFreeBlocksHeap3);
	m_freeBlockList4.init(2048,m_pFreeBlocksHeap4->GetName(),m_pFreeBlocksHeap4);
	m_freeBlockList5.init(512,m_pFreeBlocksHeap5->GetName(),m_pFreeBlocksHeap5);
	m_freeBlockList6.init(512,m_pFreeBlocksHeap6->GetName(),m_pFreeBlocksHeap6);
	m_freeBlockList7.init(512,m_pFreeBlocksHeap7->GetName(),m_pFreeBlocksHeap7);
	m_freeBlockList8.init(1024,m_pFreeBlocksHeap8->GetName(),m_pFreeBlocksHeap8);
	m_freeBlockList9.init(512,m_pFreeBlocksHeap9->GetName(),m_pFreeBlocksHeap9);

	m_memInited = true;

	return X_OK;
}

XBOOL XOSMgr::pathExists(XS8* pPath)
{
	if (GetFileAttributes(pPath) != INVALID_FILE_ATTRIBUTES)
		return true;
	else
		return false;
}

//*****************************************************************************
//*****************************************************************************
XStatus XOSMgr::setPath(X_PATH_TYPE pathType,XS8* pPath)
{
	//If NULL is passe in, clear out the path
	if (!pPath)
	{
		switch (pathType)
		{
		case X_PATH_SYSTEM:
			m_systemPath[0] = '\0';
			break;
		case X_PATH_TEXTURE:
			m_texturePath[0] = '\0';
			break;
		case X_PATH_MOVIE:
			m_moviePath[0] = '\0';
			break;
		case X_PATH_AUDIO:
			m_audioPath[0] = '\0';
			break;
		case X_PATH_UI_TEMPLATE:
			m_UITemplatePath[0] = '\0';
			break;
		case X_PATH_SHADER:
			m_shaderPath[0] = '\0';
			break;
		default:
			break;
		}
		return X_OK;
	}
	else
	{
		//Check length first
		if (strlen(pPath) >= X_MAX_PATH_SIZE-2)
			return X_ERR_INVALID_PARAMS;

		//Fix seperators.  Make sure a seperator is at the end of the path.
		char pWorkingBuffer[X_MAX_PATH_SIZE];
		strcpy(pWorkingBuffer,pPath);
		XU32 len = (XU32)strlen(pWorkingBuffer);
		if (pWorkingBuffer[len-1] == '\\')
			pWorkingBuffer[len-1] = '/';
		else if (pWorkingBuffer[len-1] != '/')
		{
			pWorkingBuffer[len] = '/';
			pWorkingBuffer[len+1] = '\0';
		}


		switch (pathType)
		{
		case X_PATH_SYSTEM:
			strcpy(m_systemPath,pWorkingBuffer);
			break;
		case X_PATH_TEXTURE:
			strcpy(m_texturePath,pWorkingBuffer);
			break;
		case X_PATH_MOVIE:
			strcpy(m_moviePath,pWorkingBuffer);
			break;
		case X_PATH_AUDIO:
			strcpy(m_audioPath,pWorkingBuffer);
			break;
		case X_PATH_UI_TEMPLATE:
			strcpy(m_UITemplatePath,pWorkingBuffer);
			break;
		case X_PATH_SHADER:
			strcpy(m_shaderPath,pWorkingBuffer);
			break;
		default:
			break;
		}

		return X_OK;
	}
}




//*****************************************************************************
//*****************************************************************************
XS8* XOSMgr::getPath(X_PATH_TYPE pathType)
{
	switch (pathType)
	{
	case X_PATH_SYSTEM:
		return m_systemPath;
		break;
	case X_PATH_TEXTURE:
		return m_texturePath;
		break;
	case X_PATH_MOVIE:
		return m_moviePath;
		break;
	case X_PATH_AUDIO:
		return m_audioPath;
		break;
	case X_PATH_UI_TEMPLATE:
		return m_UITemplatePath;
		break;
	case X_PATH_SHADER:
		return m_shaderPath;
		break;
	default:
		return NULL;
		break;
	}
}
	

//*****************************************************************************
//*****************************************************************************
XStatus XOSMgr::buildPath(X_PATH_TYPE pathType,XS8* pFilename,char fullPath[X_MAX_PATH_SIZE])
{
	//Make sure we have enough room
	XU32 pathLength = (XU32)strlen(pFilename);
	switch (pathType)
	{
	case X_PATH_SYSTEM:
		pathLength += (XU32)strlen(m_systemPath);
		break;
	case X_PATH_TEXTURE:
		pathLength += (XU32)strlen(m_texturePath);
		break;
	case X_PATH_MOVIE:
		pathLength += (XU32)strlen(m_moviePath);
		break;
	case X_PATH_AUDIO:
		pathLength += (XU32)strlen(m_audioPath);
		break;
	case X_PATH_UI_TEMPLATE:
		pathLength += (XU32)strlen(m_UITemplatePath);
		break;
	case X_PATH_SHADER:
		pathLength += (XU32)strlen(m_shaderPath);
		break;
	default:
		break;
	}

	if (pathLength+1 >= X_MAX_PATH_SIZE)
		return X_ERR_OPERATION_FAILED;
	else
	{
		switch (pathType)
		{
		case X_PATH_SYSTEM:
			strcpy(fullPath,m_systemPath);
			break;
		case X_PATH_TEXTURE:
			strcpy(fullPath,m_texturePath);
			break;
		case X_PATH_MOVIE:
			strcpy(fullPath,m_moviePath);
			break;
		case X_PATH_AUDIO:
			strcpy(fullPath,m_audioPath);
			break;
		case X_PATH_UI_TEMPLATE:
			strcpy(fullPath,m_UITemplatePath);
			break;
		case X_PATH_SHADER:
			strcpy(fullPath,m_shaderPath);
			break;
		default:
			break;
		}

		strcat(fullPath,pFilename);
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
XS8* XOSMgr::getPathFilename(XS8* pPath)
{
	X_ASSERTM(pPath,"Null path string.");

	//Return a pointer to the beginning of the filename in the string.
	XS8* p = strrchr(pPath,X_OS_PATH_SEPERATOR);
	if (!p)
		return pPath;
	else
		return p+1;
}



//*****************************************************************************
//*****************************************************************************
XF32 XOSMgr::getSysTime()
{
	//return timeGetTime();
	static XBOOL timerInitialized = false;
	static XS64 llQPFTicksPerSec = 0;

	//Initialize the timer
	if (timerInitialized == false)
	{
		timerInitialized = TRUE;

		//Use QueryPerformanceFrequency() to get frequency of timer.  
		LARGE_INTEGER qwTicksPerSec;
		QueryPerformanceFrequency(&qwTicksPerSec);
		llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

	//Query the performance counter
	LARGE_INTEGER qwTime;
	QueryPerformanceCounter(&qwTime);
	return (XF32)((qwTime.QuadPart / (XF64)llQPFTicksPerSec));
}



//*****************************************************************************
//*****************************************************************************
XS8* XOSMgr::getSysTimeString()
{
	struct tm *newtime;
	char am_pm[] = "AM";
	__time64_t long_time;
	_time64(&long_time);                
	newtime = _localtime64(&long_time); 

	if(newtime->tm_hour > 12)        
		strcpy(am_pm, "PM");
	if(newtime->tm_hour > 12)        
		newtime->tm_hour -= 12;			
	if(newtime->tm_hour == 0)       
		newtime->tm_hour = 12;

	return asctime(newtime);
}



//*****************************************************************************
//*****************************************************************************
XS8* XOSMgr::createString(XS8* pString)
{
	if (!m_pStringHeap)
		m_pStringHeap = createHeap("Strings");

	XS8* pNewString = NULL;
	XNewArray(pNewString,XS8,strlen(pString)+1,m_pStringHeap); 
	strcpy(pNewString,pString);
	return pNewString;
}

XStatus XOSMgr::destroyString(XS8** pString)
{
	if (*pString) 
	{
		XDeleteArray(*pString)
		*pString = NULL;
	}
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus fileOpen(XS8* pFilename,X_FILE_MODE_FLAGS modeFlags,XHandle& fileHandle)
{
	//Check for invalid parameters
	/*X_ASSERTM(pFilename,"Invalid filename.",__FILE__,__LINE__);
	X_ASSERTM(strlen(pFilename) < X_MAX_PATH_SIZE,"Filename too long.",__FILE__,__LINE__);

	//Find the next open slot
	XHandle potentialFileHandle = X_INVALID_HANDLE;
	for (XU32 i = 0;i < X_OS_MAX_OPEN_FILES;++i)
	{
		if (!m_files[i].inUse)
		{
			potentialFileHandle = i;
			break;
		}
	}

	//Have we exhausted our open file limit?
	if (potentialFileHandle == X_INVALID_HANDLE)
	{
		fileHandle = X_INVALID_HANDLE;
		return X_ERR_LIMIT_REACHED;
	}

	//Try to open the file in the proper mode
	FILE* pFile = NULL;
	if ((modeFlags & X_FMF_READ) && (!(modeFlags & X_FMF_BINARY)))
		pFile = fopen(pFilename,"r");
	else if ((modeFlags & X_FMF_READ) && (modeFlags & X_FMF_BINARY))
		pFile = fopen(pFilename,"rb");
	else if ((modeFlags & X_FMF_WRITE) && (!(modeFlags & X_FMF_BINARY)) && (!(modeFlags & X_FMF_APPEND)))
		pFile = fopen(pFilename,"w");
	else if ((modeFlags & X_FMF_WRITE) && (!(modeFlags & X_FMF_BINARY)) && (modeFlags & X_FMF_APPEND))
		pFile = fopen(pFilename,"a");
	else if ((modeFlags & X_FMF_WRITE) && (modeFlags & X_FMF_BINARY) && (!(modeFlags & X_FMF_APPEND)))
		pFile = fopen(pFilename,"wb");
	else if ((modeFlags & X_FMF_WRITE) && (modeFlags & X_FMF_BINARY) && (modeFlags & X_FMF_APPEND))
		pFile = fopen(pFilename,"ab");
	else
	{
		X_ASSERT_FORCED("Invalid combination of file open flags.",__FILE__,__OPEN__);
	}

	X_DEBUG
	(
		if (!pFile)
		{
			std::string s;
			s = "Could not open file ";
			s += pFilename;
			X_DEBUG_LOG_LN(s.c_str(),X_DLL_NONE);
		}
	)

	if (!pFile)
	{
		fileHandle = X_INVALID_HANDLE;
		return X_ERR_OPERATION_FAILED;
	}
	else
	{
		fileHandle = potentialFileHandle;
		m_files[fileHandle].inUse = true;
		m_files[fileHandle].pFile = pFile;
		m_files[fileHandle].modeFlags = modeFlags;
		strcpy(m_files[fileHandle].filename,pFilename);

		return X_OK;
	}
*/
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus fileClose(XHandle fileHandle)
{
	//if (fileHandle < X_OS_MAX_OPEN_FILES)
	//	if (
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 fileReadBytes(XHandle fileHandle,XU32 blockSize,XU32 numBlocks,XU8* pBytes)
{
	return 0;
}



//*****************************************************************************
//*****************************************************************************
XStatus fileWriteBytes(XHandle fileHandle,XU32 blockSize,XU32 numBlocks,XU8* pBytes)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 fileReadString(XHandle fileHandle,XS8* pCharBuffer)
{
	return 0;
}



//*****************************************************************************
//*****************************************************************************
XStatus fileWriteString(XHandle fileHandle,XS8* pString)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus fileFlush(XHandle fileHandle)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus fileEOF(XHandle fileHandle)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus fileSeek(XHandle fileHandle)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 fileTell(XHandle fileHandle)
{
	return 0;
}



//*****************************************************************************
//*****************************************************************************
XU32 XOSMgr::fileGetSize(XS8* pFilename)
{
	WIN32_FIND_DATA findData;
	HANDLE fileHandle;

	if (!pFilename)
		return 0;

	fileHandle = FindFirstFile(pFilename, &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		return findData.nFileSizeLow;	
	}

	return 0;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XOSMgr::fileExists(XS8* pFilename)
{
	WIN32_FIND_DATA findData;
	HANDLE fileHandle;

	if (!pFilename)
		return 0;

	fileHandle = FindFirstFile(pFilename, &findData);
	if (fileHandle == INVALID_HANDLE_VALUE)
		return false;
	else
		return true;
}



//*****************************************************************************
//*****************************************************************************
XHeap* XOSMgr::createHeap(XS8* heapName)
{
	return XHeapFactory::CreateHeap(heapName);
}



//*****************************************************************************
//*****************************************************************************
XHeap* XOSMgr::createHeap(XS8* heapName,XS8* parentHeapName)
{
	return XHeapFactory::CreateHeap(heapName,parentHeapName);
}



//*****************************************************************************
//*****************************************************************************
XHeap* XOSMgr::getDefaultHeap()
{
	return XHeapFactory::GetDefaultHeap();
}



//*****************************************************************************
//*****************************************************************************
XHeap* XOSMgr::getDebugHeap()
{
	return m_pDebugHeap;
}



//*****************************************************************************
//*****************************************************************************
XHeap* XOSMgr::getAnimHeap()
{
	return m_pAnimHeap;
}



//*****************************************************************************
//*****************************************************************************
XHeap* XOSMgr::getCameraHeap()
{
	return m_pCameraHeap;
}



//*****************************************************************************
//*****************************************************************************
XHeap* XOSMgr::getModelHeap()
{
	return m_pModelHeap;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOSMgr::dumpStatistics()
{
	XHeapFactory::PrintInfo();

	XS8 buf[512];
	sprintf(buf,"FREE LIST STATISTICS\n");
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf(buf,"Free List Name    Max Capacity    Peak Usage    Current Usage     Avg Alloc");
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf(buf,"---------------------------------------------------------------------------");
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);

	XU32 avgAllocation1 = 0;
	if (m_numFreeBlockAllocs1 > 0) avgAllocation1 = (XU32)(m_totalFreeBlockAlloc1/m_numFreeBlockAllocs1);
	XU32 avgAllocation2 = 0;
	if (m_numFreeBlockAllocs2 > 0) avgAllocation2 = (XU32)(m_totalFreeBlockAlloc2/m_numFreeBlockAllocs2);
	XU32 avgAllocation3 = 0;
	if (m_numFreeBlockAllocs3 > 0) avgAllocation3 = (XU32)(m_totalFreeBlockAlloc3/m_numFreeBlockAllocs3);
	XU32 avgAllocation4 = 0;
	if (m_numFreeBlockAllocs4 > 0) avgAllocation4 = (XU32)(m_totalFreeBlockAlloc4/m_numFreeBlockAllocs4);
	XU32 avgAllocation5 = 0;
	if (m_numFreeBlockAllocs5 > 0) avgAllocation5 = (XU32)(m_totalFreeBlockAlloc5/m_numFreeBlockAllocs5);
	XU32 avgAllocation6 = 0;
	if (m_numFreeBlockAllocs6 > 0) avgAllocation6 = (XU32)(m_totalFreeBlockAlloc6/m_numFreeBlockAllocs6);
	XU32 avgAllocation7 = 0;
	if (m_numFreeBlockAllocs7 > 0) avgAllocation7 = (XU32)(m_totalFreeBlockAlloc7/m_numFreeBlockAllocs7);
	XU32 avgAllocation8 = 0;
	if (m_numFreeBlockAllocs8 > 0) avgAllocation8 = (XU32)(m_totalFreeBlockAlloc8/m_numFreeBlockAllocs8);
	XU32 avgAllocation9 = 0;
	if (m_numFreeBlockAllocs9 > 0) avgAllocation9 = (XU32)(m_totalFreeBlockAlloc9/m_numFreeBlockAllocs9);

	int spacing = 20;
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList1.getName(),m_freeBlockList1.getMaxBlockCapacity(),m_freeBlockList1.getPeakBlocksInUse(),m_freeBlockList1.getNumBlocksInUse(),avgAllocation1);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList2.getName(),m_freeBlockList2.getMaxBlockCapacity(),m_freeBlockList2.getPeakBlocksInUse(),m_freeBlockList2.getNumBlocksInUse(),avgAllocation2);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList3.getName(),m_freeBlockList3.getMaxBlockCapacity(),m_freeBlockList3.getPeakBlocksInUse(),m_freeBlockList3.getNumBlocksInUse(),avgAllocation3);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList4.getName(),m_freeBlockList4.getMaxBlockCapacity(),m_freeBlockList4.getPeakBlocksInUse(),m_freeBlockList4.getNumBlocksInUse(),avgAllocation4);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList5.getName(),m_freeBlockList5.getMaxBlockCapacity(),m_freeBlockList5.getPeakBlocksInUse(),m_freeBlockList5.getNumBlocksInUse(),avgAllocation5);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList6.getName(),m_freeBlockList6.getMaxBlockCapacity(),m_freeBlockList6.getPeakBlocksInUse(),m_freeBlockList6.getNumBlocksInUse(),avgAllocation6);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList7.getName(),m_freeBlockList7.getMaxBlockCapacity(),m_freeBlockList7.getPeakBlocksInUse(),m_freeBlockList7.getNumBlocksInUse(),avgAllocation7);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList8.getName(),m_freeBlockList8.getMaxBlockCapacity(),m_freeBlockList8.getPeakBlocksInUse(),m_freeBlockList8.getNumBlocksInUse(),avgAllocation8);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList9.getName(),m_freeBlockList9.getMaxBlockCapacity(),m_freeBlockList9.getPeakBlocksInUse(),m_freeBlockList9.getNumBlocksInUse(),avgAllocation9);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);

	X_DEBUG_LOG("\n\n",X_DLL_NONE);



	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
#ifndef XVP_FINAL
XStatus XOSMgr::dumpAllocations()
{
	XHeapFactory::dumpAllocations();
	return X_OK;
}
#endif



//*****************************************************************************
//*****************************************************************************
void XOSMgr::logDebugString(const XS8* debugString,const XS8* file,XU32 line,
						 XU32 debugLevel,XBOOL newline)
{
	
	//Filter based on the debugging level.
	if (debugLevel > m_debugLoggingLevel)
		return;
	XS8 buf[2048] = {'\0'}; 
	for (XU32 i = 0;i < m_debugLoggingIndentAmount;i++)
		strcat(buf," ");
	
	if (debugLevel == X_DLL_WARNING)
		strcat(buf,"XVP WARNING: ");
	if (debugLevel == X_DLL_ASSERT)
		strcat(buf,"\n**************************************************************************************\nXVP ASSERT\n     ");
	if (debugLevel == X_DLL_ASSERT || debugLevel == X_DLL_WARNING)
	{
		if (file)
		{
			XS8 lineNumBuf[16];
			XS8 adjustedFilename[64];
			if (file)
			{
				if (strchr(file,'\\') != NULL)
					strncpy(adjustedFilename,strrchr(file,'\\')+1,63);
				else if (strchr(file,'/') != NULL)
					strncpy(adjustedFilename,strrchr(file,'/')+1,63);
				else
					strncpy(adjustedFilename,file,63);
			}
			strcat(buf,adjustedFilename);
			strcat(buf," ");
			strcat(buf,"(");
			strcat(buf,_itoa(line,lineNumBuf,10));
			strcat(buf,")");
			strcat(buf,": ");
		}
	}
	strcat(buf,debugString);

	if (debugLevel == X_DLL_ASSERT)
		strcat(buf,"\n**************************************************************************************\n");


	//ofstream f(X_DEBUG_LOG_FILENAME,ios::app);
	//f.setf(ios::right);
	//f.fill('0');
	//f  << setw(8) << m_lineNumber << ":";
	//++m_lineNumber;
	XS8* currentTime = XOSMgr::getSysTimeString();
	XU32 length = (XU32)strlen(currentTime);
	//for (XU32 i = 0;i < length-1;i++)
     //   f.put(currentTime[i]);
	//OutputDebugString(currentTime);
	//OutputDebugString("XVP: ");
	OutputDebugString(buf);
	if (newline)
		OutputDebugString("\n");

	if (m_genLogFile)
	{
		ofstream f("XVPLog.txt",ios::app);
		f << buf;
		if (newline)
			f << "\n";
		f.close();
	}

	//f.close();
}



//*****************************************************************************
//*****************************************************************************
void XOSMgr::incrementDebugIndent(XU32 increment)
{
	m_debugLoggingIndentAmount += increment;
}



//*****************************************************************************
//*****************************************************************************
void XOSMgr::decrementDebugIndent(XU32 decrement)
{
	if (m_debugLoggingIndentAmount != 0)
		m_debugLoggingIndentAmount -= decrement;
}



//*****************************************************************************
//*****************************************************************************
void XOSMgr::setDebugIndent(XU32 amount)
{
	m_debugLoggingIndentAmount = amount;
}



//*****************************************************************************
//*****************************************************************************
XU32 XOSMgr::getDebugIndent(XU32 increment)
{
	return m_debugLoggingIndentAmount;
}



//*****************************************************************************
//*****************************************************************************
void XOSMgr::setDebugLevel(XU32 level)
{
	m_debugLoggingLevel = level;
}



//*****************************************************************************
//*****************************************************************************
int XOSMgr::getDebugLevel()
{
	return m_debugLoggingLevel;
}


/*
//*****************************************************************************
//*****************************************************************************
XPointerContainer* XOSMgr::getFreePointerContainer()
{
	return m_freePointerContainers.allocateItem();
}



//*****************************************************************************
//*****************************************************************************
XStatus XOSMgr::returnFreePointerContainer(XPointerContainer* pContainer)
{
	m_freePointerContainers.freeItem(pContainer);
	return X_OK;
}
*/

#ifdef XVP_FINAL
//*****************************************************************************
//*****************************************************************************
void* XOSMgr::getFreeMemBlock(XU32 size)
{
	//Make sure mem blocks have been allocated
	if (!m_memInited)
		return NULL;

	if (size <= sizeof(privateSizeStruct1))
	{
		m_totalFreeBlockAlloc1 += size;
		m_numFreeBlockAllocs1++;
		return (void*)m_freeBlockList1.allocateItem();
	}
	else if (size <= sizeof(privateSizeStruct2))
	{
		m_totalFreeBlockAlloc2 += size;
		m_numFreeBlockAllocs2++;
		return (void*)m_freeBlockList2.allocateItem();
	}
	else if (size <= sizeof(privateSizeStruct3))
	{
		m_totalFreeBlockAlloc3 += size;
		m_numFreeBlockAllocs3++;
		return (void*)m_freeBlockList3.allocateItem();
	}
	else if (size <= sizeof(privateSizeStruct4))
	{
		m_totalFreeBlockAlloc4 += size;
		m_numFreeBlockAllocs4++;
		return (void*)m_freeBlockList4.allocateItem();
	}
	else if (size <= sizeof(privateSizeStruct5))
	{
		m_totalFreeBlockAlloc5 += size;
		m_numFreeBlockAllocs5++;
		return (void*)m_freeBlockList5.allocateItem();
	}
	else if (size <= sizeof(privateSizeStruct6))
	{
		m_totalFreeBlockAlloc6 += size;
		m_numFreeBlockAllocs6++;
		return (void*)m_freeBlockList6.allocateItem();
	}
	else if (size <= sizeof(privateSizeStruct7))
	{
		m_totalFreeBlockAlloc7 += size;
		m_numFreeBlockAllocs7++;
		return (void*)m_freeBlockList7.allocateItem();
	}
	else if (size <= sizeof(privateSizeStruct8))
	{
		m_totalFreeBlockAlloc8 += size;
		m_numFreeBlockAllocs8++;
		return (void*)m_freeBlockList8.allocateItem();
	}
	else if (size <= sizeof(privateSizeStruct9))
	{
		m_totalFreeBlockAlloc9 += size;
		m_numFreeBlockAllocs9++;
		return (void*)m_freeBlockList9.allocateItem();
	}
	else
	{
		X_DEBUG
		(
			XS8 buf[256];
			sprintf(buf,"Can't allocate mem block of size %d bytes",size);
			X_ASSERT_FORCED(buf);
		)
		return NULL;
	}
}
#else
//*****************************************************************************
//*****************************************************************************
void* XOSMgr::getFreeMemBlock(XU32 size,XS8* pFilename,XU32 lineNum)
{
	//Make sure mem blocks have been allocated
	if (!m_memInited)
		return NULL;

	if (size <= sizeof(privateSizeStruct1))
	{
		m_totalFreeBlockAlloc1 += size;
		m_numFreeBlockAllocs1++;
		return (void*)m_freeBlockList1.allocateItem(pFilename,lineNum);
	}
	else if (size <= sizeof(privateSizeStruct2))
	{
		m_totalFreeBlockAlloc2 += size;
		m_numFreeBlockAllocs2++;
		return (void*)m_freeBlockList2.allocateItem(pFilename,lineNum);
	}
	else if (size <= sizeof(privateSizeStruct3))
	{
		m_totalFreeBlockAlloc3 += size;
		m_numFreeBlockAllocs3++;
		return (void*)m_freeBlockList3.allocateItem(pFilename,lineNum);
	}
	else if (size <= sizeof(privateSizeStruct4))
	{
		m_totalFreeBlockAlloc4 += size;
		m_numFreeBlockAllocs4++;
		return (void*)m_freeBlockList4.allocateItem(pFilename,lineNum);
	}
	else if (size <= sizeof(privateSizeStruct5))
	{
		m_totalFreeBlockAlloc5 += size;
		m_numFreeBlockAllocs5++;
		return (void*)m_freeBlockList5.allocateItem(pFilename,lineNum);
	}
	else if (size <= sizeof(privateSizeStruct6))
	{
		m_totalFreeBlockAlloc6 += size;
		m_numFreeBlockAllocs6++;
		return (void*)m_freeBlockList6.allocateItem(pFilename,lineNum);
	}
	else if (size <= sizeof(privateSizeStruct7))
	{
		m_totalFreeBlockAlloc7 += size;
		m_numFreeBlockAllocs7++;
		return (void*)m_freeBlockList7.allocateItem(pFilename,lineNum);
	}
	else if (size <= sizeof(privateSizeStruct8))
	{
		m_totalFreeBlockAlloc8 += size;
		m_numFreeBlockAllocs8++;
		return (void*)m_freeBlockList8.allocateItem(pFilename,lineNum);
	}
	else if (size <= sizeof(privateSizeStruct9))
	{
		m_totalFreeBlockAlloc9 += size;
		m_numFreeBlockAllocs9++;
		return (void*)m_freeBlockList9.allocateItem(pFilename,lineNum);
	}
	else
	{
		X_DEBUG
		(
			XS8 buf[256];
			sprintf(buf,"Can't allocate mem block of size %d bytes",size);
			X_ASSERT_FORCED(buf);
		)
		return NULL;
	}
}
#endif


//*****************************************************************************
//*****************************************************************************
XStatus XOSMgr::returnMemBlock(XU32 size,void* pMemBlock)
{
	if (!m_memInited)
		return X_ERR_NOT_READY;

	if (size <= sizeof(privateSizeStruct1))
	{
		m_freeBlockList1.freeItem((privateSizeStruct1*)pMemBlock);
		return X_OK;
	}
	else if (size <= sizeof(privateSizeStruct2))
	{
		m_freeBlockList2.freeItem((privateSizeStruct2*)pMemBlock);
		return X_OK;
	}
	else if (size <= sizeof(privateSizeStruct3))
	{
		m_freeBlockList3.freeItem((privateSizeStruct3*)pMemBlock);
		return X_OK;
	}
	else if (size <= sizeof(privateSizeStruct4))
	{
		m_freeBlockList4.freeItem((privateSizeStruct4*)pMemBlock);
		return X_OK;
	}
	else if (size <= sizeof(privateSizeStruct5))
	{
		m_freeBlockList5.freeItem((privateSizeStruct5*)pMemBlock);
		return X_OK;
	}
	else if (size <= sizeof(privateSizeStruct6))
	{
		m_freeBlockList6.freeItem((privateSizeStruct6*)pMemBlock);
		return X_OK;
	}
	else if (size <= sizeof(privateSizeStruct7))
	{
		m_freeBlockList7.freeItem((privateSizeStruct7*)pMemBlock);
		return X_OK;
	}
	else if (size <= sizeof(privateSizeStruct8))
	{
		m_freeBlockList8.freeItem((privateSizeStruct8*)pMemBlock);
		return X_OK;
	}
	else if (size <= sizeof(privateSizeStruct9))
	{
		m_freeBlockList9.freeItem((privateSizeStruct9*)pMemBlock);
		return X_OK;
	}
	else
	{
		X_DEBUG
		(
			XS8 buf[256];
			sprintf(buf,"Can't return mem block of size %d bytes",size);
			X_ASSERT_FORCED(buf);
		)
		return X_ERR_INVALID_PARAMS;
	}

}



//*****************************************************************************
//*****************************************************************************
XStatus XOSMgr::getMemBlockSize(void* pMemBlock,XU32& size)
{
	if (m_freeBlockList1.verifyItem((privateSizeStruct1*)pMemBlock))
		size = sizeof(privateSizeStruct1);
	else if (m_freeBlockList2.verifyItem((privateSizeStruct2*)pMemBlock))
		size = sizeof(privateSizeStruct2);
	else if (m_freeBlockList3.verifyItem((privateSizeStruct3*)pMemBlock))
		size = sizeof(privateSizeStruct3);
	else if (m_freeBlockList4.verifyItem((privateSizeStruct4*)pMemBlock))
		size = sizeof(privateSizeStruct4);
	else if (m_freeBlockList5.verifyItem((privateSizeStruct5*)pMemBlock))
		size = sizeof(privateSizeStruct5);
	else if (m_freeBlockList6.verifyItem((privateSizeStruct6*)pMemBlock))
		size = sizeof(privateSizeStruct6);
	else if (m_freeBlockList7.verifyItem((privateSizeStruct7*)pMemBlock))
		size = sizeof(privateSizeStruct7);
	else if (m_freeBlockList8.verifyItem((privateSizeStruct8*)pMemBlock))
		size = sizeof(privateSizeStruct8);
	else if (m_freeBlockList9.verifyItem((privateSizeStruct9*)pMemBlock))
		size = sizeof(privateSizeStruct9);
	else
		size = 0;

	if (!m_memInited)
		return X_ERR_NOT_READY;
	else
		return X_OK;
}


XBOOL XOSMgr::isInternalFreeListsEnabled()
{
	return m_internalFreeListsEnabled;
}


