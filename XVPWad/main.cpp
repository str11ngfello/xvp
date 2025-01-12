#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <tchar.h>
#include <zlib.h>
#include <assert.h>
#include <time.h>

using std::vector;

#define CHUNK 16384*2

#define MAX_FILE_NAME_SIZE		64+1	
#define MAX_FILE_NAME_LENGTH	64	


struct FILE_HEADER
{
	char sourcePath[MAX_PATH];
	char name[MAX_FILE_NAME_SIZE];
	LONG size;
	LONG compressedSize;

	FILE_HEADER()
	{
		name[0] = '\0'; 
		size = 0;
		compressedSize = 0;
	}
};


//options
#define X_EXTRACT					"-x"
#define X_ADDFILES					"-a"
#define X_ADDFILESCOMPRESSED		"-ac"
#define X_DELETEFILES				"-d"
#define X_LISTFILES					"-l"

void AddFiles(bool compressed,LONG numParams,int argc,char** argv);
LONG CalculateOffset(char* targetFile);
LONG GetFileSize(char* targetFile);
bool GenCompressedFile(char* pFilename,char* pSourcePath);
//bool GenDecompressedFile(char* pFilename);
//bool GenDecompressedFileInPlace(char* pFilename,LONG size);
bool GenDecompressedFile(char* pFilename,FILE* pSourceWad,LONG compressedSize,LONG uncompressedSize);
void DeleteTemporaryFiles();
void CreateWadTOC(bool compressed,FILE* wadFile,std::vector<FILE_HEADER>& fileList);
void CreateWadBody(bool compressed,FILE* wadFile,std::vector<FILE_HEADER>& fileList);

vector<FILE_HEADER> fileList;

using namespace std;


int main(int argc,char** argv)
{
	//output an initial blank line for neatness
	//cout << endl;

	//if not enough params
	if (argc < 3)
	{
		cout << "Usage: xwad -option wadfile [filelist]" << endl << endl
		     << "   Options:" << endl
			 << "       -h  ->help" << endl
			 << "       -x  ->extract files from wad file" << endl
			 << "       -a  ->add files to wad file" << endl
			 << "       -ac ->compress files and add them to wad file" << endl
			 << "       -d  ->delete files from wad file" << endl
			 << "       -l  ->list wad file contents" << endl << endl
			 << "   Wad File:" << endl
			 << "       This parameter is the filename of the wad file\n"
			 << "       to perform the option on. For example, \"mywad.wad\"" << endl << endl
			 << "   [filelist]:" << endl
			 << "       *  ->all files in directory" << endl
			 << "       filename.ext ->specific file or a list of files seperated by a space" << endl;
	
		return 0;
	}
	
	//Update the global number of parameters after the first 3
	LONG numParams = 0;
	numParams = argc - 3;
	
	if (strcmp(X_LISTFILES,argv[1]) == 0)
	{
		WIN32_FIND_DATA findData;
		HANDLE fileHandle;
		fileHandle = FindFirstFile(argv[2], &findData);
		
		//if file does not exist
		if (fileHandle == INVALID_HANDLE_VALUE)
			cout << "No such file named \"" << argv[2] << "\"" << endl;
		else
		{
			LONG numFiles = 0;
			FILE_HEADER tempStruct;
			FILE* wadFile = fopen(argv[2],"rb");
			fread((void*)&numFiles,sizeof(LONG),1,wadFile);
			for (int x = 0;x < numFiles; x++)
			{
				fread ((void*)&tempStruct,sizeof(FILE_HEADER),1,wadFile);
				fileList.push_back(tempStruct);
			}
			fclose(wadFile);
			FindClose(fileHandle);

			//Output filenames
			//SYSTEMTIME s;
			cout << "Contents of \"" << argv[2] << "\": " << endl;
			for (int x = 0;x < numFiles;x++)
			{
				//FileTimeToSystemTime(fileList[x].ftLastWriteTime,&s);
				cout << "   \"" << fileList[x].name << "\"   ";
				cout << "original(" << fileList[x].size << ")   ";
				if (fileList[x].compressedSize != 0)
					cout << "compressed(" << fileList[x].compressedSize <<")"  << endl;
				else
					cout << endl;
			}
		}
	}
	else if (strcmp(X_DELETEFILES,argv[1]) == 0)
	{
		//first, look at the params and decide what to do
		if (numParams == 0)
		{
			cout << "Missing Parameters" << endl;
			return 0;
		}
		
		if (strcmp(argv[3],"*") == 0)
		{
			//If they want to erase all the files in a wad file, just erase
			//the wad file and create a new one with the same name.
			cout << "Deleting all files in wad..." << endl;
			char buf[MAX_PATH];
			sprintf(buf,"del \"%s\"",argv[2]);
			system(buf);
			ofstream f;
			f.open(argv[2]);
			f.close();
		}
		//else there wad no "*" but a file list instead.
		else
		{
			WIN32_FIND_DATA findData;
			HANDLE fileHandle;
			fileHandle = FindFirstFile(argv[2], &findData);
			
			//if file does not exist
			if (fileHandle == INVALID_HANDLE_VALUE)
				cout << "No such file named \"" << argv[2] << "\"" << endl;			
			else
			{
				LONG numFiles = 0;
				FILE_HEADER tempStruct;
				vector<FILE_HEADER> newFileList;
				vector<FILE_HEADER> removeFileList;
				FILE* oldWadFile;
				FILE* newWadFile;
				oldWadFile = fopen(argv[2],"rb");
				fread((void*)&numFiles,sizeof(LONG),1,oldWadFile);
				for (int x = 0;x < numFiles; x++)
				{
					fread ((void*)&tempStruct,sizeof(FILE_HEADER),1,oldWadFile);
					fileList.push_back(tempStruct);
				}
				
				//Create a new file list the represents the orignal files in the
				//wad file minus the ones to be removed
				for (unsigned int ctr = 0;ctr < fileList.size();ctr++)
				{
					bool remove = false;
					for (int x = 0;x < numParams; x++)
					{
						if (strcmp(argv[x+3],fileList[ctr].name) == 0)
						{
							cout << "Deleting \"" << fileList[ctr].name << "\"..." << endl;
							remove = true;
							break;
						}
					}

					//Now test the remove variable to see if we should push or not
					if (!remove)
						newFileList.push_back(fileList[ctr]);
				}
			
				//update the numFiles
				LONG oldNumFiles = numFiles;
				LONG newNumFiles = newFileList.size();

				//Now we need to create the base for the new wad file
				char buf2[512];
				sprintf(buf2,"%s",argv[2]);
				strcat(buf2,"_temp");
				newWadFile = fopen(buf2,"wb");//tmpfile();
				fwrite((void*)&newNumFiles,sizeof(LONG),1,newWadFile);
				for (int x = 0;x < newNumFiles;x++)
					fwrite((void*)&newFileList[x],sizeof(FILE_HEADER),1,newWadFile);
					

				//Now add the actual file data for each file.
				//The first part will be an actual copy of the oldWadFile's files
				for (int x = 0;x < newNumFiles; x++)
				{
					LONG size = 0;
					if (newFileList[x].compressedSize != 0)
						size = newFileList[x].compressedSize;
					else
						size = newFileList[x].size;

					BYTE* bytePtr = new BYTE[size];
					LONG offset = CalculateOffset(newFileList[x].name);
					fseek(oldWadFile,offset,SEEK_SET);
					fread((void*)bytePtr,sizeof(BYTE),size,oldWadFile);
					fwrite((void*)bytePtr,sizeof(BYTE),size,newWadFile);
					fflush(newWadFile);
					delete[] bytePtr;
				}

								
				//erase old file
				fclose(oldWadFile);
				char buf[MAX_PATH];
				sprintf(buf,"del \"%s\"",argv[2]);
				system(buf);

				//create the new file with same name
				fclose(newWadFile);

				sprintf(buf2,"%s",argv[2]);
				strcat(buf2,"_temp");
				//sprintf(buf,"move \"%s\" \"%s\"",buf2,argv[2]);
				//system(buf);
				rename(buf2,argv[2]);
			}
		}
	}
	else if (strcmp(X_EXTRACT,argv[1]) == 0)
	{
		//first, look at the params and decide what to do
		if (numParams == 0)
		{
			cout << "Missing Parameters" << endl;
			return 0;
		}
		
		if (strcmp(argv[3],"*") == 0)
		{

			WIN32_FIND_DATA findData;
			HANDLE fileHandle;
			fileHandle = FindFirstFile(argv[2], &findData);
			
			//if file does not exist
			if (fileHandle == INVALID_HANDLE_VALUE)
				cout << "No such file named \"" << argv[2] << "\"" << endl;
			else
			{
				LONG numFiles = 0;
				FILE_HEADER tempStruct;
				FILE* wadFile = fopen(argv[2],"rb");
				fread((void*)&numFiles,sizeof(LONG),1,wadFile);
				for (int x = 0;x < numFiles; x++)
				{
					fread ((void*)&tempStruct,sizeof(FILE_HEADER),1,wadFile);
					fileList.push_back(tempStruct);
				}
				FindClose(fileHandle);

				//Extract all files from the wad file
				for (int x = 0;x < numFiles; x++)
				{
					DWORD start,finish;
					start = timeGetTime();

					cout << "Extracting \"" << fileList[x].name << "\"... ";

					if (fileList[x].compressedSize != 0)
					{
						//char buf[MAX_PATH];
						//strcpy(buf,fileList[x].name);
						//strcat(buf,".compressed.temp");

						//BYTE* bytePtr = new BYTE[fileList[x].compressedSize];
						LONG offset = CalculateOffset(fileList[x].name);
						fseek(wadFile,offset,SEEK_SET);
						//fread((void*)bytePtr,sizeof(BYTE),fileList[x].compressedSize,wadFile);
						//FILE* outputFile = fopen(buf,"wb");
						//fwrite((void*)bytePtr,sizeof(BYTE),fileList[x].compressedSize,outputFile);
						//fflush(outputFile);
						//fclose(outputFile);
						//delete[] bytePtr;

						finish = timeGetTime();
						cout << "in " << (finish-start)/1000.0f << " seconds" << endl;

						cout << "Decompressing \"" << fileList[x].name << "\"... ";

						start = timeGetTime();
						GenDecompressedFile(fileList[x].name,wadFile,fileList[x].compressedSize,fileList[x].size);	
						finish = timeGetTime();
						cout << "in " << (finish-start)/1000.0f << " seconds" << endl;
					}
					else
					{
						BYTE* bytePtr = new BYTE[fileList[x].size];
						LONG offset = CalculateOffset(fileList[x].name);
						fseek(wadFile,offset,SEEK_SET);
						fread((void*)bytePtr,sizeof(BYTE),fileList[x].size,wadFile);
						FILE* outputFile = fopen(fileList[x].name,"wb");
						fwrite((void*)bytePtr,sizeof(BYTE),fileList[x].size,outputFile);
						fflush(outputFile);
						fclose(outputFile);
						delete[] bytePtr;

						finish = timeGetTime();
						cout << "in " << (finish-start)/1000.0f << " seconds" << endl;

					}

			
				}
				fclose(wadFile);

			}
		}
		//else a "*" was not found so there must be a list of filenames to extract
		else
		{
			WIN32_FIND_DATA findData;
			HANDLE fileHandle;
			fileHandle = FindFirstFile(argv[2], &findData);
			
			//if file does not exist
			if (fileHandle == INVALID_HANDLE_VALUE)
				cout << "No such file named \"" << argv[2] << "\"" << endl;
			else
			{
				LONG numFiles = 0;
				FILE_HEADER tempStruct;
				FILE* wadFile = fopen(argv[2],"rb");
				fread((void*)&numFiles,sizeof(LONG),1,wadFile);
				for (int x = 0;x < numFiles; x++)
				{
					fread ((void*)&tempStruct,sizeof(FILE_HEADER),1,wadFile);
					fileList.push_back(tempStruct);
				}
				FindClose(fileHandle);

				//Extract each specified file on the command line
				for (int x = 0;x < numParams; x++)
				{
					FILE_HEADER w;
					bool fileFound = false;

					//find the struct that corresponds to each file
					for (unsigned int y = 0;y < fileList.size();y++)
					{
						if (strcmp(fileList[y].name,argv[x+3]) == 0)
						{
							memcpy((void*)&w,(void*)&fileList[y],sizeof(FILE_HEADER));	
							fileFound = true;
							break;
						}	
					}
					//if the file was found in the wad file
					if (fileFound)
					{
						DWORD start,finish;
						start = timeGetTime();

						cout << "Extracting \"" << w.name << "\"... ";

						if (w.compressedSize != 0)
						{
							//char buf[MAX_PATH];
							//strcpy(buf,w.name);
							//strcat(buf,".compressed.temp");

							//BYTE* bytePtr = new BYTE[w.compressedSize];
							LONG offset = CalculateOffset(w.name);
							fseek(wadFile,offset,SEEK_SET);
							//fread((void*)bytePtr,sizeof(BYTE),w.compressedSize,wadFile);
							//FILE* outputFile = fopen(buf,"wb");
							//fwrite((void*)bytePtr,sizeof(BYTE),w.compressedSize,outputFile);
							//fflush(outputFile);
							//fclose(outputFile);
							//delete[] bytePtr;

							finish = timeGetTime();
							cout << "in " << (finish-start)/1000.0f << " seconds" << endl;

							cout << "Decompressing \"" << w.name << "\"... ";
						
							start = timeGetTime();
							GenDecompressedFile(w.name,wadFile,w.compressedSize,w.size);	
							finish = timeGetTime();
							cout << "in " << (finish-start)/1000.0f << " seconds" << endl;
						}
						else
						{
							BYTE* bytePtr = new BYTE[w.size];
							LONG offset = CalculateOffset(w.name);
							fseek(wadFile,offset,SEEK_SET);
							fread((void*)bytePtr,sizeof(BYTE),w.size,wadFile);
							FILE* outputFile = fopen(w.name,"wb");
							fwrite((void*)bytePtr,sizeof(BYTE),w.size,outputFile);
							fflush(outputFile);
							fclose(outputFile);
							delete[] bytePtr;

							finish = timeGetTime();
							cout << "in " << (finish-start)/1000.0f << " seconds" << endl;
						}
					}
					else
						cout << "Warning! No such file \"" << argv[x+3] << "\"" << endl;	
				}
				fclose(wadFile);
			}
		}
	}
	else if (strcmp(X_ADDFILES,argv[1]) == 0)
	{
		AddFiles(false,numParams,argc,argv);
	}
	else if (strcmp(X_ADDFILESCOMPRESSED,argv[1]) == 0)
	{
		AddFiles(true,numParams,argc,argv);
	}
	else 
	{
		cout << "Unknown option" << endl;
	}

   
	DeleteTemporaryFiles();

	return 0;
}


void AddFiles(bool compressed,LONG numParams,int argc,char** argv)
{
	//first, look at the params and decide what to do
	if (numParams == 0)
	{
		cout << "Missing Parameters" << endl;
		return;
	}
	
	if (strcmp(argv[3],"*") == 0)
	{
		WIN32_FIND_DATA findData;
		HANDLE fileHandle;
	
		fileHandle = FindFirstFile("*", &findData);
		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			while (true) 
			{
				//skip ".." "." "xvpwad.exe" and name of wad file
				if ((strcmp(findData.cFileName,".") != 0) &&
					(strcmp(findData.cFileName,"..") != 0) &&
					(strcmp(findData.cFileName,"xvpwad.exe") != 0) &&
					(strcmp(findData.cFileName,argv[2]) != 0) &&
					(findData.nFileSizeLow != 0))
				{
					struct FILE_HEADER fh;

					//Check file name size limit
					if (strlen(findData.cFileName) > MAX_FILE_NAME_LENGTH)
					{
						cout << "Warning! Filename \"" << findData.cFileName << "\" exceeds max length.  File will not be included." << endl;
					}
					else
					{
						GetFullPathName(findData.cFileName,MAX_PATH,fh.sourcePath,NULL);
						strcpy(fh.name,findData.cFileName);
						fh.size = findData.nFileSizeLow;
						fileList.push_back(fh);
					}

					if (false == FindNextFile(fileHandle, &findData))
						break;
				}
				else
				{
					if (false == FindNextFile(fileHandle, &findData))
						break;
				}			
			}
			FindClose(fileHandle);
		}

		FILE* wadFile = fopen(argv[2],"wb");
		LONG numFiles = fileList.size();
		fwrite((void*)&numFiles,sizeof(LONG),1,wadFile);
		CreateWadTOC(compressed,wadFile,fileList);
		CreateWadBody(compressed,wadFile,fileList);
		fclose(wadFile);
	}
	//else a "*" was not found so there must be a list of filenames to add
	else
	{
		WIN32_FIND_DATA findData;
		HANDLE fileHandle;
		fileHandle = FindFirstFile(argv[2], &findData);
		vector<FILE_HEADER> tempFileList;

		//if file does not exist
		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			//Now add all the files listed to the new wad file
			for (int x = 0;x < numParams; x++)
			{
				WIN32_FIND_DATA w;
				bool fileFound = false;
				HANDLE fileHandle2 = FindFirstFile(argv[x+3],&w);
				
				if (fileHandle2 != INVALID_HANDLE_VALUE)
				{
					FILE_HEADER fh;

					//Check file name size limit
					if (strlen(w.cFileName) > MAX_FILE_NAME_LENGTH)
						cout << "Warning! Filename \"" << argv[x+3] << "\" exceeds max length.  File will not be included." << endl;
					else
					{
						GetFullPathName(argv[x+3],MAX_PATH,fh.sourcePath,NULL);
						strcpy(fh.name,w.cFileName);
						fh.size = w.nFileSizeLow;
						fileFound = true;
						tempFileList.push_back(fh);
					}
				}
				else
					cout << "Warning! No such file \"" << argv[x+3] << "\"" << endl;
			}

			FILE* wadFile = fopen(argv[2],"wb");
			LONG numFiles = tempFileList.size();
			fwrite((void*)&numFiles,sizeof(LONG),1,wadFile);
			CreateWadTOC(compressed,wadFile,tempFileList);
			CreateWadBody(compressed,wadFile,tempFileList);
			fclose(wadFile);
		}		
		//else file does exist so we must preserve all previous files in wad file
		else
		{
			LONG numFiles = 0;
			FILE_HEADER tempStruct;
			vector<FILE_HEADER> newFileList;
			vector<FILE_HEADER> skipList;
			FILE* oldWadFile;
			FILE* newWadFile;
			oldWadFile = fopen(argv[2],"rb");
			fread((void*)&numFiles,sizeof(LONG),1,oldWadFile);
			for (int x = 0;x < numFiles; x++)
			{
				fread ((void*)&tempStruct,sizeof(FILE_HEADER),1,oldWadFile);
				fileList.push_back(tempStruct);
			}
			
			//Fill in the command line list of files
			//Now add all the files listed to the new wad file
			for (int x = 0;x < numParams; x++)
			{
				WIN32_FIND_DATA w;
				FILE_HEADER fh;

				HANDLE fileHandle2 = FindFirstFile(argv[x+3],&w);
				
				if (fileHandle2 != INVALID_HANDLE_VALUE)	
				{
					//Check file name size limit
					if (strlen(w.cFileName) > MAX_FILE_NAME_LENGTH)
					{
						cout << "Warning! Filename \"" << argv[x+3] << "\" exceeds max length.  File will not be included." << endl;
					}
					else
					{
						//Check to make sure the file doesn't already exist, if so,
						//stick it in the skip list for later					
						for (unsigned int y = 0;y < fileList.size(); y++)
						{
							if (strcmp(w.cFileName,fileList[y].name) == 0)
								skipList.push_back(fileList[y]);
						}		
						
						GetFullPathName(argv[x+3],MAX_PATH,fh.sourcePath,NULL);
						strcpy(fh.name,w.cFileName);
						fh.size = w.nFileSizeLow;
						newFileList.push_back(fh);
					}
				}
				else
					cout << "Warning! No such file \"" << argv[x+3] << "\"" << endl;
			}

			LONG oldNumFiles = numFiles;
			LONG newNumFiles = fileList.size() + newFileList.size() - skipList.size();

			//Now we need to create the base for the new wad file
			char buf2[512];
			sprintf(buf2,"%s",argv[2]);
			strcat(buf2,"_temp");
			newWadFile = fopen(buf2,"wb");//tmpfile();

			//Write the number of files to be included in the wad file.
			fwrite((void*)&newNumFiles,sizeof(LONG),1,newWadFile);
			for (unsigned int x = 0;x < fileList.size();x++)
			{
				bool skip = false;
				for (unsigned int y = 0;y < skipList.size(); y++)
				{
					if (strcmp(skipList[y].name,fileList[x].name) == 0)
					{
						skip = true;
						break;
					}
				}	

				if (!skip)
					fwrite((void*)&fileList[x],sizeof(FILE_HEADER),1,newWadFile);
			}
			CreateWadTOC(compressed,newWadFile,newFileList);

			//Now add the actual file data for each file.
			//The first part will be an actual copy of the oldWadFile's files
			//Don't do anything silly like re-compress, just copy 
			for (int x = 0;x < oldNumFiles; x++)
			{
				bool skip = false;
				for (unsigned int y = 0;y < skipList.size(); y++)
				{
					if (strcmp(skipList[y].name,fileList[x].name) == 0)
					{
						skip = true;
						break;
					}
				}	

				if (!skip)
				{
					LONG size = 0;
					if (fileList[x].compressedSize != 0)
						size = fileList[x].compressedSize;
					else
						size = fileList[x].size;

					BYTE* bytePtr = new BYTE[size];
					LONG offset = CalculateOffset(fileList[x].name);
					fseek(oldWadFile,offset,SEEK_SET);
					fread((void*)bytePtr,sizeof(BYTE),size,oldWadFile);
					fwrite((void*)bytePtr,sizeof(BYTE),size,newWadFile);
					fflush(newWadFile);
					delete[] bytePtr;
				}
			}

			CreateWadBody(compressed,newWadFile,newFileList);
				
			//erase old file
			fclose(oldWadFile);
			char buf[MAX_PATH];
			sprintf(buf,"del \"%s\"",argv[2]);
			system(buf);

			//create the new file with same name
			fclose(newWadFile);

			sprintf(buf2,"%s",argv[2]);
			strcat(buf2,"_temp");
			//sprintf(buf,"move \"%s\" \"%s\"",buf2,argv[2]);
			//system(buf);
			rename(buf2,argv[2]);
		}
	}
}


LONG CalculateOffset(char* targetFile)
{
	LONG fileSizesSum = 0;
	for (unsigned int x = 0;x < fileList.size();x++)
	{
		if (strcmp(fileList[x].name,targetFile) == 0)
			return sizeof(LONG) + (sizeof(FILE_HEADER) * fileList.size()) + fileSizesSum;
		else
		{
			if (fileList[x].compressedSize != 0)
				fileSizesSum += fileList[x].compressedSize;
			else
				fileSizesSum += fileList[x].size;
		}
	}

	//if target file was not found then return 0;
	return 0;
}



/* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
int def(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

/* Decompress from file source to file dest until stream ends or EOF.
   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_DATA_ERROR if the deflate data is
   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
   the version of the library linked do not match, or Z_ERRNO if there
   is an error reading or writing the files. */
int inf(FILE *source, FILE *dest)
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

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


int infFromMemToMem(BYTE *source, LONG compressedSize, BYTE *dest)
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
	BYTE* nextDestWritePos = dest;
	BYTE* nextSourceReadPos = source;

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
        strm.next_in = nextSourceReadPos;//strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
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

int infFromWadToMem(FILE* pSourceWad, LONG compressedSize, BYTE *dest)
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
	BYTE* nextDestWritePos = dest;

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

        strm.avail_in = nextReadLen;
		fread(in, 1, nextReadLen, pSourceWad);
		if (ferror(pSourceWad)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
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


LONG GetFileSize(char* targetFile)
{
	FILE* f = fopen(targetFile,"rb");
	LONG size = 0;
	fseek(f,0,SEEK_END);
    size = ftell(f);
	return size;
}

bool GenCompressedFile(char* pFilename,char* pSourcePath)
{
	char buf[MAX_PATH];
	strcpy(buf,pFilename);
	strcat(buf,".compressed.temp");

	FILE* inFile = fopen(pSourcePath,"rb");
	FILE* outFile = fopen(buf,"w+b");
	  
	int ret = def(inFile, outFile, Z_DEFAULT_COMPRESSION);

	fclose(inFile);
	fclose(outFile);

    if (ret != Z_OK)
	{
		cout << "Error compressing file " << pSourcePath << endl;
		return false;
	}
	else
		return true;
}

bool GenDecompressedFile(char* pFilename,FILE* pSourceWad,LONG compressedSize,LONG uncompressedSize)
{
	BYTE* pDestData = new BYTE[uncompressedSize];
	if (pDestData == NULL)
	{
		cout << "Error trying to allocate uncompressed buffer of size " << uncompressedSize << " bytes" << endl;
		return false;
	}
	
	int ret = infFromWadToMem(pSourceWad,compressedSize,pDestData);

	FILE* outFile = fopen(pFilename,"w+b");
	if (outFile == NULL)
	{
		cout << "Error opening destination file \"" << pFilename << "\"" << endl;
		return false;
	}

	fwrite(pDestData,1,uncompressedSize,outFile);
	fclose(outFile);

	delete[] pDestData;

    if (ret != Z_OK)
	{
		cout << "Error decompressing file " << ret << endl;
		return false;
	}
	else
		return true;
}

/*
bool GenDecompressedFileInPlace(char* pFilename,LONG size)
{
	//The output file cannot be read only
	DWORD attr = GetFileAttributes(pFilename);
	if (attr != INVALID_FILE_ATTRIBUTES)
	{
		if ((attr & FILE_ATTRIBUTE_READONLY) ||
			(attr & FILE_ATTRIBUTE_HIDDEN))
		{
			cout << "A read only or hidden file with this name already exists in the directory.  Extraction failed ";
			return false;
		}
	}

	char buf[MAX_PATH];
	strcpy(buf,pFilename);
	strcat(buf,".compressed.temp");

	LONG compressedSize = GetFileSize(buf);

	FILE* inFile = fopen(buf,"rb");
	BYTE* pSourceData = new BYTE[compressedSize];
	fread(pSourceData,1,compressedSize,inFile);


	FILE* outFile = fopen(pFilename,"w+b");
	BYTE* pDestData = new BYTE[size];
	
	int ret = infFromMemToMem(pSourceData,compressedSize,pDestData);

	fwrite(pDestData,1,size,outFile);
	fclose(inFile);
	fclose(outFile);

    if (ret != Z_OK)
	{
		cout << "Error decompressing file " << ret << endl;
		return false;
	}
	else
		return true;
}


bool GenDecompressedFile(char* pFilename)
{
	//The output file cannot be read only
	if (GetFileAttributes(pFilename) != INVALID_FILE_ATTRIBUTES)
	{
		if (GetFileAttributes(pFilename) & FILE_ATTRIBUTE_READONLY)
		{
			cout << "A read only file with this name already exists in the directory.  Extraction failed ";
			return false;
		}
	}

	char buf[MAX_PATH];
	strcpy(buf,pFilename);
	strcat(buf,".compressed.temp");

	FILE* inFile = fopen(buf,"rb");
	FILE* outFile = fopen(pFilename,"w+b");
	  
	int ret = inf(inFile, outFile);

	fclose(inFile);
	fclose(outFile);

    if (ret != Z_OK)
	{
		cout << "Error decompressing file " << ret << endl;
		return false;
	}
	else
		return true;
}*/

void DeleteTemporaryFiles()
{
	//Make sure all file handles are closed
	_fcloseall();

	WIN32_FIND_DATA findData;
	HANDLE fileHandle;

	fileHandle = FindFirstFile("*.compressed.temp", &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		while (true) 
		{
			DeleteFile(findData.cFileName);
			if (false == FindNextFile(fileHandle, &findData))
				break;			
		}
		FindClose(fileHandle);
	}
}

void CreateWadTOC(bool compressed,FILE* wadFile,std::vector<FILE_HEADER>& fileList)
{
	//Are we compressing the files?  If so, generate a temporary compressed version 
	//of each and grab the compressed size to fill in the struct
	if (compressed)
	{
		for (unsigned int ctr = 0;ctr < fileList.size();ctr++)
		{
			cout << "Compressing \"" << fileList[ctr].name << "\"... ";
			
			DWORD start,finish;
			start = timeGetTime();
			GenCompressedFile(fileList[ctr].name,fileList[ctr].sourcePath);
			finish = timeGetTime();
			cout << "in " << (finish-start)/1000.0f << " seconds" << endl;

			char buf[MAX_PATH];
			strcpy(buf,fileList[ctr].name);
			strcat(buf,".compressed.temp");

			fileList[ctr].compressedSize = GetFileSize(buf);			
		}
	}

	//Write out the entire fileList of FILE_HEADER structures
	for (unsigned int ctr = 0;ctr < fileList.size();ctr++)
		fwrite((void*)&fileList[ctr],sizeof(FILE_HEADER),1,wadFile);
}

void CreateWadBody(bool compressed,FILE* wadFile,std::vector<FILE_HEADER>& fileList)
{
	//Now open each file mentioned in the fileList and write it in
	//sequence to the wadFile
	FILE* tempFile;
	for (unsigned int ctr = 0;ctr < fileList.size();ctr++)
	{
		DWORD start,finish;
		start = timeGetTime();

		cout << "Adding \"" << fileList[ctr].name << "\"... ";

		BYTE* bytePtr = NULL;
		if (compressed)
		{
			char buf[MAX_PATH];
			strcpy(buf,fileList[ctr].name);
			strcat(buf,".compressed.temp");

			tempFile = fopen(buf,"rb");
			bytePtr = new BYTE[fileList[ctr].compressedSize];
			fread((void*)bytePtr,sizeof(BYTE),fileList[ctr].compressedSize,tempFile);
			fwrite((void*)bytePtr,sizeof(BYTE),fileList[ctr].compressedSize,wadFile);
		}
		else
		{
			tempFile = fopen(fileList[ctr].sourcePath,"rb");
			bytePtr = new BYTE[fileList[ctr].size];
			fread((void*)bytePtr,sizeof(BYTE),fileList[ctr].size,tempFile);
			fwrite((void*)bytePtr,sizeof(BYTE),fileList[ctr].size,wadFile);
		}
		
		fflush(wadFile);
		fclose(tempFile);
		delete[] bytePtr;

		finish = timeGetTime();
		cout << "in " << (finish-start)/1000.0f << " seconds" << endl;
	}
}