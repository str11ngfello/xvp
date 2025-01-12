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
//	Module Name:	XWad.cpp
//	Author:			Lynn Duke
//	Creation Date:	11-1-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

#define CHUNK 16384*2


//*****************************************************************************
/**
Constructor for the WAD file.  
*/
//*****************************************************************************
XWad::XWad()
{
	m_pWadFilename[0] = '\0';
	m_pTOC = NULL;
	m_pMemChunk = NULL;
	m_numFiles = 0;
}



//*****************************************************************************
/**
Destructor for the WAD file.  
*/
//*****************************************************************************
XWad::~XWad()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XWad::initFromFile(XS8* pFilename)
{
	X_ASSERTM(pFilename,"Invalid filename");
	X_ASSERTM(strlen(pFilename) < X_MAX_PATH_SIZE-1,"Filename too long");

	//Store the filename for the wad
	strcpy(m_pWadFilename,pFilename);
	m_pMemChunk = NULL;

	//Read in TOC
	m_pWadFile = fopen(pFilename,"rb");
	X_ASSERT(m_pWadFile)
	fread((void*)&m_numFiles,sizeof(XU32),1,m_pWadFile);
	XNewArray(m_pTOC,FILE_HEADER,m_numFiles,XOSMgr::getDefaultHeap());
	fread ((void*)m_pTOC,sizeof(FILE_HEADER)*m_numFiles,1,m_pWadFile);

	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XStatus XWad::initFromMemory(XS8* pMemChunk)
{
	X_ASSERTM(pMemChunk,"Invalid memchunk pointer");

	//Read in TOC
	m_pWadFilename[0] = '\0';
	m_pWadFile = NULL;
	//fopen(pFilename,"rb");
	
	XS8* pCurrent = pMemChunk;
	m_pMemChunk = pMemChunk;
	
	m_numFiles = *(XU32*)pCurrent;
	pCurrent += sizeof(XU32);
	//fread((void*)&m_numFiles,sizeof(XU32),1,m_pWadFile);
	XNewArray(m_pTOC,FILE_HEADER,m_numFiles,XOSMgr::getDefaultHeap());
	
	for (int x = 0;x < m_numFiles; x++)
	{
		m_pTOC[x] = *(FILE_HEADER*)pCurrent;
		pCurrent += sizeof(FILE_HEADER);
		//fread ((void*)m_pTOC,sizeof(FILE_HEADER)*m_numFiles,1,m_pWadFile);
	}
	
	return X_OK;
}

XStatus XWad::destroy()
{
	XDeleteArray(m_pTOC);
	m_numFiles = 0;
	m_pWadFilename[0] = '\0';
	m_pMemChunk = NULL;
	if (m_pWadFile)
	{
		fclose(m_pWadFile);
		m_pWadFile = NULL;
	}

	return X_OK;
}

XStatus XWad::extractFile(XS8* pFilename,XS8*& pBytes,XU32& byteLen)
{
	X_ASSERTM(pFilename,"Invalid wad filename");

	FILE_HEADER* pFoundHeader = NULL;
	
	//find the header we're looking for
	for (XU32 i = 0;i < m_numFiles;i++)
	{
		XS32 a = strlen (m_pTOC[i].name);
		XS32 b = strlen (pFilename);
		if (strcmp(m_pTOC[i].name,pFilename) == 0)
		{
			pFoundHeader = &m_pTOC[i];
			break;
		}	
	}
	
	//if the file was found in the wad file
	if (pFoundHeader)
	{
		//Is the wad file in memory or on disk?
		if (m_pMemChunk)
		{
			XU32 offset = calculateOffset(pFoundHeader);
			XS8* pStart = m_pMemChunk + offset;
			//fseek(m_pWadFile,offset,SEEK_SET);
			XNewArray(pBytes,XS8,pFoundHeader->size,XOSMgr::getDefaultHeap());
			byteLen = pFoundHeader->size;

			if (pFoundHeader->compressedSize != 0)
			{
				XS32 ret = infFromMemToMem(pStart,pFoundHeader->compressedSize,pBytes);
				X_ASSERTM(ret == Z_OK,"Error decompressing file from wad in memory");
			}
			else
			{
				memcpy((void*)pBytes,pStart,pFoundHeader->size);			
			}
		}
		else
		{
			XU32 offset = calculateOffset(pFoundHeader);
			fseek(m_pWadFile,offset,SEEK_SET);
			XNewArray(pBytes,XS8,pFoundHeader->size,XOSMgr::getDefaultHeap());
			byteLen = pFoundHeader->size;

			if (pFoundHeader->compressedSize != 0)
			{
				XS32 ret = infFromWadToMem(m_pWadFile,pFoundHeader->compressedSize,pBytes);
				X_ASSERTM(ret == Z_OK,"Error decompressing file from wad on disk");
			}
			else
			{
				fread((void*)pBytes,sizeof(XS8),pFoundHeader->size,m_pWadFile);			
			}
		}
	}
	
	return X_OK;
}

XStatus XWad::findFiles(XS8* pExtension,XMemBlockList<XS8*>& filenames)
{
	X_ASSERTM(pExtension,"Invalid extension");
	
	//find the extensions we're looking for
	for (XU32 i = 0;i < m_numFiles;i++)
	{
		//Get the filename extension
		XS8* pFoundExt = NULL;
		pFoundExt = strrchr(m_pTOC[i].name,'.');
		if (!pFoundExt)
			continue;

		if (strcmp(pFoundExt,pExtension) == 0)
			filenames.insertBack(m_pTOC[i].name);	
	}
	
	return X_OK;
}


/*
//*****************************************************************************
//*****************************************************************************
XStatus XWad::extract(XS8* pFilename,XS8* pTargetDirectory)
{
	if ((!pFilename) || (!pTargetDirectory))
		return X_ERR_INVALID_PARAMS;

	WIN32_FIND_DATA findData;
	HANDLE fileHandle;
	
	fileHandle = FindFirstFile(m_pWadFilename, &findData);
	//if file does not exist
	if (fileHandle == INVALID_HANDLE_VALUE)
		return X_ERR_OPERATION_FAILED;
	else
	{	
		XMemBlockList<WIN32_FIND_DATA> fileList;
		XU32 numFiles = 0;
		WIN32_FIND_DATA tempStruct;
		FILE* wadFile = fopen(m_pWadFilename,"rb");
		fread((void*)&numFiles,sizeof(XU32),1,wadFile);
		for (XU32 x = 0;x < numFiles; x++)
		{
			fread ((void*)&tempStruct,sizeof(WIN32_FIND_DATA),1,wadFile);
			fileList.insertBack(tempStruct);
		}
		FindClose(fileHandle);

		//Extract specified file 
		WIN32_FIND_DATA w;
		XBOOL fileFound = false;
		XMemBlockList<WIN32_FIND_DATA>::iterator y(fileList.begin());
		while (y.isValid())
		{
			if (strcmp(y.getValue().cFileName,pFilename) == 0)
			{
				memcpy((void*)&w,(void*)&y.getValue(),sizeof(WIN32_FIND_DATA));	
				fileFound = true;		
				break;
			}	
			y.getNext();
		}
		//if the file was found in the wad file
		if (fileFound)
		{
			XS8* pByte = NULL;
			XNewArray(pByte,XS8,w.nFileSizeLow,XOSMgr::getDefaultHeap());
			XU32 offset = calculateOffset(w.cFileName,fileList);
			fseek(wadFile,offset,SEEK_SET);
			fread((void*)pByte,sizeof(XS8),w.nFileSizeLow,wadFile);
			
			//Attatch the pFilename to the target directory
			char buf[X_MAX_PATH_SIZE+1];
			strcpy(buf,pTargetDirectory);

			//make sure a trailing "/" to end of target directory
			//if one does not exist.
			if (buf[strlen(buf)-1] != '/')
			{
				XU32 tempLocation = (XU32)strlen(buf);
				buf[tempLocation] = '/';
				buf[tempLocation+1] = '\0';
				
			}

			strcat(buf,pFilename);
			FILE* outputFile = fopen(buf,"wb");
			fwrite((void*)pByte,sizeof(XS8),w.nFileSizeLow,outputFile);
			fflush(outputFile);
			fclose(outputFile);
			XDeleteArray(pByte);
		}
		else
		{	
			fclose(wadFile);
			return X_ERR_OPERATION_FAILED;	
		}

		fileList.clear();
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWad::extractAll(XS8* pTargetDirectory)
{
	if (!pTargetDirectory)
		return X_ERR_INVALID_PARAMS;

	WIN32_FIND_DATA findData;
	HANDLE fileHandle;
	
	fileHandle = FindFirstFile(m_pWadFilename, &findData);
	//if file does not exist
	if (fileHandle == INVALID_HANDLE_VALUE)
		return X_ERR_OPERATION_FAILED;
	else
	{
		XMemBlockList<WIN32_FIND_DATA> fileList;
		XU32 numFiles = 0;
		WIN32_FIND_DATA tempStruct;
		FILE* wadFile = fopen(m_pWadFilename,"rb");
		fread((void*)&numFiles,sizeof(XU32),1,wadFile);
		for (XU32 x = 0;x < numFiles; x++)
		{
			fread ((void*)&tempStruct,sizeof(WIN32_FIND_DATA),1,wadFile);
			fileList.insertBack(tempStruct);
		}
		FindClose(fileHandle);

		//Extract all files 
		XMemBlockList<WIN32_FIND_DATA>::iterator y(fileList.begin());
		while (y.isValid())
		{
			XS8* pByte = NULL;
			XNewArray(pByte,XS8,y.getValue().nFileSizeLow,XOSMgr::getDefaultHeap());
			XU32 offset = calculateOffset(y.getValue().cFileName,fileList);
			fseek(wadFile,offset,SEEK_SET);
			fread((void*)pByte,sizeof(XS8),y.getValue().nFileSizeLow,wadFile);
			
			//Attatch the pFilename to the target directory
			char buf[X_MAX_PATH_SIZE+1];
			strcpy(buf,pTargetDirectory);
			//make sure a trailing "/" to end of target directory
			//if one does not exist.
			if (buf[strlen(buf)-1] != '/')
			{
				XU32 tempLocation = (XU32)strlen(buf);
				buf[tempLocation] = '/';
				buf[tempLocation+1] = '\0';
			}

			strcat(buf,y.getValue().cFileName);
			FILE* outputFile = fopen(buf,"wb");
			fwrite((void*)pByte,sizeof(XS8),y.getValue().nFileSizeLow,outputFile);
			fflush(outputFile);
			fclose(outputFile);
			XDeleteArray(pByte);

			y.getNext();
		}
		fileList.clear();
		fclose(wadFile);
	}
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWad::loadFileFromWadIntoMemory(XS8* pFilename,XS8** pByte,XU32* sizeInBytes)
{
	if ((!pFilename) || (!pByte))
		return X_ERR_INVALID_PARAMS;

	WIN32_FIND_DATA findData;
	HANDLE fileHandle;  
	
	fileHandle = FindFirstFile(m_pWadFilename, &findData);
	//if file does not exist
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		*pByte = NULL;
		*sizeInBytes = 0;
		return X_ERR_OPERATION_FAILED;;
	}
	else   
	{
		XMemBlockList<WIN32_FIND_DATA> fileList;
		XU32 numFiles = 0;
		WIN32_FIND_DATA tempStruct;
		FILE* wadFile = fopen(m_pWadFilename,"rb");
		fread((void*)&numFiles,sizeof(XU32),1,wadFile);
		for (XU32 x = 0;x < numFiles; x++)
		{
			fread ((void*)&tempStruct,sizeof(WIN32_FIND_DATA),1,wadFile);
			fileList.insertBack(tempStruct);
		}
		FindClose(fileHandle);

		//Extract specified file 
		WIN32_FIND_DATA w;
		XBOOL fileFound = false;
		XMemBlockList<WIN32_FIND_DATA>::iterator y(fileList.begin());
		while (y.isValid())
		{
			if (strcmp(y.getValue().cFileName,pFilename) == 0)
			{
				memcpy((void*)&w,(void*)&y.getValue(),sizeof(WIN32_FIND_DATA));	
				fileFound = true;		
				break;
			}	
			y.getNext();
		}
		//if the file was found in the wad file
		if (fileFound)
		{
			XNewArray(*pByte,XS8,w.nFileSizeLow,XOSMgr::getDefaultHeap());
			XU32 offset = calculateOffset(w.cFileName,fileList);
			fseek(wadFile,offset,SEEK_SET);
			fread((void*)*pByte,sizeof(XS8),w.nFileSizeLow,wadFile);
			fclose(wadFile);

			return X_OK;
		}
		else
		{	
			fclose(wadFile);
			return X_ERR_OPERATION_FAILED;
		}

		fileList.clear();
	}
}



//*****************************************************************************
//*****************************************************************************
XU32 XWad::calculateOffset(XS8* pFilename,XMemBlockList<WIN32_FIND_DATA> fileList)
{
	XU32 fileSizesSum = 0;
	XMemBlockList<WIN32_FIND_DATA>::iterator x(fileList.begin());
	while (x.isValid())
	{
		if (strcmp(x.getValue().cFileName,pFilename) == 0)
			return (XU32)(sizeof(XU32) + (sizeof(WIN32_FIND_DATA) * fileList.getNumItems()) + fileSizesSum);
		else
			fileSizesSum += x.getValue().nFileSizeLow;
		x.getNext();
	}

	//if target file was not found then return 0;
	return 0;
}
*/

int XWad::infFromMemToMem(XS8 *source, LONG compressedSize, XS8 *dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    //unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

	LONG bytesRemaining = compressedSize;
	XS8* nextDestWritePos = dest;
	XS8* nextSourceReadPos = source;

    /* decompress until deflate stream ends or end of file */
    do {
		LONG nextReadLen = 0;
		if (bytesRemaining - CHUNK > 0)
		{
			bytesRemaining -= CHUNK;
			nextReadLen = CHUNK;
		}
		else
		{
			nextReadLen = bytesRemaining;
			bytesRemaining = 0;
		}

        strm.avail_in = nextReadLen;//fread(in, 1, CHUNK, source);
		//memcpy(in,nextSourceReadPos,nextReadLen);
		
        /*if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }*/
        if (strm.avail_in == 0)
            break;
        strm.next_in = (Bytef*)nextSourceReadPos;//strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            X_ASSERTM(ret != Z_STREAM_ERROR,"Decompression error");  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;

			memcpy(nextDestWritePos,out,have);
			nextDestWritePos += have;
			

            /*if (fwrite(out, 1, have, dest) != have || ferror(dest)) 
			{
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }*/
        } while (strm.avail_out == 0);

		nextSourceReadPos += nextReadLen;

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


XS32 XWad::infFromWadToMem(FILE* pSourceWad, XU32 compressedSize, XS8* dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

	LONG bytesRemaining = compressedSize;
	XS8* nextDestWritePos = dest;

    /* decompress until deflate stream ends or end of file */
    do {
		LONG nextReadLen = 0;
		if (bytesRemaining - CHUNK > 0)
		{
			bytesRemaining -= CHUNK;
			nextReadLen = CHUNK;
		}
		else
		{
			nextReadLen = bytesRemaining;
			bytesRemaining = 0;
		}

		if (nextReadLen == 0)
			break;

        strm.avail_in = nextReadLen;
		fread(in, 1, nextReadLen, pSourceWad);
		if (ferror(pSourceWad)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
       
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            X_ASSERTM(ret != Z_STREAM_ERROR,"Stream error while decompressing from wad file");  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;

			memcpy(nextDestWritePos,out,have);
			nextDestWritePos += have;
			

            /*if (fwrite(out, 1, have, dest) != have || ferror(dest)) 
			{
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }*/
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}



XU32 XWad::calculateOffset(FILE_HEADER* pHeader)
{
	X_ASSERTM(pHeader,"Invalid header pointer");
	X_ASSERTM(m_pTOC,"Invalid TOC pointer");

	FILE_HEADER* pTempHeader = m_pTOC;
	XU32 fileSizesSum = 0;

	for (XU32 x = 0;x < m_numFiles;++x)
	{
		if (&m_pTOC[x] == pHeader)
			return sizeof(XU32) + (sizeof(FILE_HEADER) * m_numFiles) + fileSizesSum;
		else
		{
			if (m_pTOC[x].compressedSize != 0)
				fileSizesSum += m_pTOC[x].compressedSize;
			else
				fileSizesSum += m_pTOC[x].size;
		}
	}

	//if target file was not found then return 0;
	return 0;
}

XS8* XWad::getFilename()
{
	return m_pWadFilename;
}