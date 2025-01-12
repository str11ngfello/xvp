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
//	Module Name:	XOSMgr.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_OS_MGR_H__
#define __X_OS_MGR_H__



#define X_OS_MAX_OPEN_FILES				64
#define X_OS_MAX_FREE_BLOCK_SIZE		1024
#define X_OS_PATH_SEPERATOR				'/'


//*****************************************************************************
//! Operating System Driver and resource manager.
//*****************************************************************************
class XOSMgr
{
private:

	class XFile : public XResource 
	{
		XBOOL inUse;
		X_FILE_MODE_FLAGS modeFlags;
		XS8* filename[X_MAX_PATH_SIZE];
		FILE* pFile;

		XFile()
		{
			filename[0] = '\0';
			inUse = false;
			modeFlags = (X_FILE_MODE_FLAGS)0;
			pFile = NULL;
		}
	};

	static XHeap*				m_pStringHeap;
	static XHeap*				m_pDebugHeap;
	static XHeap*				m_pAnimHeap;
	static XHeap*				m_pCameraHeap;
	static XHeap*				m_pModelHeap;

	static XHeap*				m_pFreeBlocksHeap1;
	static XHeap*				m_pFreeBlocksHeap2;
	static XHeap*				m_pFreeBlocksHeap3;
	static XHeap*				m_pFreeBlocksHeap4;
	static XHeap*				m_pFreeBlocksHeap5;
	static XHeap*				m_pFreeBlocksHeap6;
	static XHeap*				m_pFreeBlocksHeap7;
	static XHeap*				m_pFreeBlocksHeap8;
	static XHeap*				m_pFreeBlocksHeap9;
	static XResourceList		m_files;
	static XU32					m_debugLoggingIndentAmount;
	static XU32					m_debugLoggingLevel;
	static XU32					m_lineNumber;
	static char					m_systemPath[X_MAX_PATH_SIZE];
	static char					m_texturePath[X_MAX_PATH_SIZE];
	static char					m_moviePath[X_MAX_PATH_SIZE];
	static char					m_audioPath[X_MAX_PATH_SIZE];
	static char					m_UITemplatePath[X_MAX_PATH_SIZE];
	static char					m_shaderPath[X_MAX_PATH_SIZE];


	struct privateSizeStruct1 {void* nothing;};
	struct privateSizeStruct2 {XU8 nothing[sizeof(privateSizeStruct1)*2];};
	struct privateSizeStruct3 {XU8 nothing[sizeof(privateSizeStruct2)*2];};
	struct privateSizeStruct4 {XU8 nothing[sizeof(privateSizeStruct3)*2];};
	struct privateSizeStruct5 {XU8 nothing[sizeof(privateSizeStruct4)*2];};
	struct privateSizeStruct6 {XU8 nothing[sizeof(privateSizeStruct5)*2];};
	struct privateSizeStruct7 {XU8 nothing[sizeof(privateSizeStruct6)*2];};
	struct privateSizeStruct8 {XU8 nothing[sizeof(privateSizeStruct7)*2];};
	struct privateSizeStruct9 {XU8 nothing[sizeof(privateSizeStruct8)*2];};

	static XFreeList<privateSizeStruct1>	m_freeBlockList1;
	static XFreeList<privateSizeStruct2>	m_freeBlockList2;
	static XFreeList<privateSizeStruct3>	m_freeBlockList3;
	static XFreeList<privateSizeStruct4>	m_freeBlockList4;
	static XFreeList<privateSizeStruct5>	m_freeBlockList5;
	static XFreeList<privateSizeStruct6>	m_freeBlockList6;
	static XFreeList<privateSizeStruct7>	m_freeBlockList7;
	static XFreeList<privateSizeStruct8>	m_freeBlockList8;
	static XFreeList<privateSizeStruct9>	m_freeBlockList9;
	static XU32	m_totalFreeBlockAlloc1;
	static XU32	m_totalFreeBlockAlloc2;
	static XU32	m_totalFreeBlockAlloc3;
	static XU32	m_totalFreeBlockAlloc4;
	static XU32	m_totalFreeBlockAlloc5;
	static XU32	m_totalFreeBlockAlloc6;
	static XU32	m_totalFreeBlockAlloc7;
	static XU32	m_totalFreeBlockAlloc8;
	static XU32	m_totalFreeBlockAlloc9;
	static XU32	m_numFreeBlockAllocs1;
	static XU32	m_numFreeBlockAllocs2;
	static XU32	m_numFreeBlockAllocs3;
	static XU32	m_numFreeBlockAllocs4;
	static XU32	m_numFreeBlockAllocs5;
	static XU32	m_numFreeBlockAllocs6;
	static XU32	m_numFreeBlockAllocs7;
	static XU32	m_numFreeBlockAllocs8;
	static XU32	m_numFreeBlockAllocs9;


	static XBOOL	m_memInited;
	static XBOOL	m_genLogFile;
	static XBOOL	m_assertsEnabled;
	static XBOOL	m_internalFreeListsEnabled;

	static void*    m_pProcessHandle;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XOSMgr();
	~XOSMgr();

	static XStatus initFreeMemBlocks();


public:
	//*****************************************************************************
	//General Functions
	//*****************************************************************************
	static XStatus init(XVPDesc& initDesc);
	static XStatus destroy();
	static void* getProcessHandle() { return m_pProcessHandle; }

	//*****************************************************************************
	//Assert Functions
	//*****************************************************************************
	static X_ASSERT_ACTION_TYPE getAssertAction(XS8* outputString);
	static XStatus enableAsserts(XBOOL enable);
	static XBOOL enableAsserts();

	//*****************************************************************************
	//Path Functions
	//*****************************************************************************
	static XBOOL pathExists(XS8* pPath);
	static XStatus setPath(X_PATH_TYPE pathType,XS8* pPath);
	static XS8* getPath(X_PATH_TYPE pathType);
	static XStatus buildPath(X_PATH_TYPE pathType,XS8* pFilename,XS8 fullPath[X_MAX_PATH_SIZE]);
	static XStatus changePathExtension();
	static XStatus combinePaths();
	static XStatus getPathDirName();
	static XS8* getPathFilename(XS8* pPath);
	static XStatus getPathFilenameWithoutExtension();
	static XStatus hasExtension();


	//*****************************************************************************
	//Time Functions
	//*****************************************************************************
	static XF32 getSysTime();
	static XS8* getSysTimeString();

	//*****************************************************************************
	//String Functions
	//*****************************************************************************
	static XS8* createString(XS8* pString);
	static XStatus destroyString(XS8** pString);
	//static XStatus sprintf();
	//etc...

	//*****************************************************************************
	//Thread Functions
	//*****************************************************************************
	//createThread, etc...


	//*****************************************************************************
	//File Functions
	//*****************************************************************************
	static XStatus fileOpen(XS8* pFilename,X_FILE_MODE_FLAGS modeFlags,XHandle& fileHandle);
	static XStatus fileClose(XHandle fileHandle);
	static XU32 fileReadBytes(XHandle fileHandle,XU32 blockSize,XU32 numBlocks,XU8* pBytes);
	static XStatus fileWriteBytes(XHandle fileHandle,XU32 blockSize,XU32 numBlocks,XU8* pBytes);
	static XU32 fileReadString(XHandle fileHandle,XS8* pCharBuffer);
	static XStatus fileWriteString(XHandle fileHandle,XS8* pString);
	static XStatus fileFlush(XHandle fileHandle);
	static XStatus fileEOF(XHandle fileHandle);
	static XStatus fileSeek(XHandle fileHandle);
	static XU32 fileTell(XHandle fileHandle);
	static XU32 fileGetSize(XS8* pFilename);
	static XBOOL fileExists(XS8* pFilename);

	//*****************************************************************************
	//Memory Functions
	//*****************************************************************************
	static XHeap* createHeap(XS8* heapName);
	static XHeap* createHeap(XS8* heapName,XS8* parentHeapName);
	static XHeap* getDefaultHeap();
	static XHeap* getDebugHeap();
	static XHeap* getAnimHeap();
	static XHeap* getCameraHeap();
	static XHeap* getModelHeap();
#ifdef XVP_FINAL
	static void* getFreeMemBlock(XU32 size);
#else
	static void* getFreeMemBlock(XU32 size,XS8* pFilename = NULL,XU32 lineNum = 0);
#endif
	static XStatus returnMemBlock(XU32 size,void* pMemBlock);
	static XStatus getMemBlockSize(void* pMemBlock,XU32& size);
	static XBOOL isInternalFreeListsEnabled();

#ifndef XVP_FINAL
	static XStatus dumpAllocations();
#endif
	//static XStatus newMemory(void* userPointer,dataType,heap) {userPointer = new (heap) dataType();}

	//*************************************************************************
	//Logging Functions.
	//*************************************************************************
//#ifndef XVP_FINAL
	static void logDebugString(const XS8* debugString,const XS8* file,XU32 line,XU32 debugLevel,XBOOL newline);
	static void incrementDebugIndent(XU32 increment);
	static void decrementDebugIndent(XU32 decrement);
	static void setDebugIndent(XU32 amount);
	static XU32 getDebugIndent(XU32 increment);
	static void setDebugLevel(XU32 level);
	static XS32 getDebugLevel();
//#endif

	//*************************************************************************
	//Utility Functions.
	//*************************************************************************
	static XStatus dumpStatistics();
};


#endif