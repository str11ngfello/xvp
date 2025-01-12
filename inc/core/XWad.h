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
//	Module Name:	XWad.h
//	Author:			Lynn Duke
//	Creation Date:	11-1-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_WADFILE_H__
#define __X_WADFILE_H__




#define MAX_FILE_NAME_SIZE		64+1	
#define MAX_FILE_NAME_LENGTH	64	


//*****************************************************************************
//*****************************************************************************
class XWad
{					  
private:

	struct FILE_HEADER
	{
		char sourcePath[MAX_PATH];
		XS8 name[MAX_FILE_NAME_SIZE];
		LONG size;
		LONG compressedSize;

		FILE_HEADER()
		{
			name[0] = '\0'; 
			size = 0;
			compressedSize = 0;
		}
	};

	XS8 m_pWadFilename[X_MAX_PATH_SIZE];
	XS8* m_pMemChunk;
	FILE* m_pWadFile;
	FILE_HEADER* m_pTOC;
	XU32 m_numFiles;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XU32 calculateOffset(FILE_HEADER* pHeader);
	XS32 infFromMemToMem(XS8 *source, LONG compressedSize, XS8 *dest);
	XS32 infFromWadToMem(FILE* pSourceWad, XU32 sourceLen, XS8* dest);
	
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XWad();
	~XWad();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus initFromFile(XS8* pFilename);
	XStatus initFromMemory(XS8* pMemChunk);
	XStatus destroy();
	XStatus extractFile(XS8* pFilename,XS8*& pBytes,XU32& byteLen);
	XStatus findFiles(XS8* pExtension,XMemBlockList<XS8*>& filenames);
	XS8* getFilename();
	/*XStatus extract(XS8* pFilename,XS8* pTargetDirectory);
	XStatus extractAll(XS8* pTargetDirectory);
	XStatus loadFileFromWadIntoMemory(XS8* pFilename,XS8** pByte,XU32* sizeInBytes);*/
};

#endif
