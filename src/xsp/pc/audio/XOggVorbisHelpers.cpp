
#include <xvpsdk.h>


//---------------------------------------------------------------------------------
// Function	: VorbisRead
// Purpose	: Callback for the Vorbis read function
// Info		: 
//---------------------------------------------------------------------------------
size_t VorbisRead(void *ptr			/* ptr to the data that the vorbis files need*/, 
				  size_t byteSize	/* how big a byte is*/, 
				  size_t sizeToRead /* How much we can read*/, 
				  void *datasource	/* this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/)
{
	size_t				spaceToEOF;			// How much more we can read till we hit the EOF marker
	size_t				actualSizeToRead;	// How much data we are actually going to read from memory
	SOggFile*			vorbisData;			// Our vorbis data, for the typecast

	// Get the data in the right format
	vorbisData = (SOggFile*)datasource;

	// Calculate how much we need to read.  This can be sizeToRead*byteSize or less depending on how near the EOF marker we are
	spaceToEOF = vorbisData->dataSize - vorbisData->dataRead;
	if ((sizeToRead*byteSize) < spaceToEOF)
		actualSizeToRead = (sizeToRead*byteSize);
	else
		actualSizeToRead = spaceToEOF;	
	
	// A simple copy of the data from memory to the datastruct that the vorbis libs will use
	if (actualSizeToRead)
	{
		// Copy the data from the start of the file PLUS how much we have already read in
		memcpy(ptr, (char*)vorbisData->dataPtr + vorbisData->dataRead, actualSizeToRead);
		// Increase by how much we have read by
		vorbisData->dataRead += (actualSizeToRead);
	}

	// Return how much we read (in the same way fread would)
	return actualSizeToRead;
}

//---------------------------------------------------------------------------------
// Function	: VorbisSeek
// Purpose	: Callback for the Vorbis seek function
// Info		: 
//---------------------------------------------------------------------------------
int VorbisSeek(void *datasource		/*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/, 
			   ogg_int64_t offset	/*offset from the point we wish to seek to*/, 
			   int whence			/*where we want to seek to*/)
{
	size_t				spaceToEOF;		// How much more we can read till we hit the EOF marker
	ogg_int64_t			actualOffset;	// How much we can actually offset it by
	SOggFile*			vorbisData;		// The data we passed in (for the typecast)

	// Get the data in the right format
	vorbisData = (SOggFile*)datasource;

	// Goto where we wish to seek to
	switch (whence)
	{
	case SEEK_SET: // Seek to the start of the data file
		// Make sure we are not going to the end of the file
		if (vorbisData->dataSize >= offset)
			actualOffset = offset;
		else
			actualOffset = vorbisData->dataSize;
		// Set where we now are
		vorbisData->dataRead = (int)actualOffset;
		break;
	case SEEK_CUR: // Seek from where we are
		// Make sure we dont go past the end
		spaceToEOF = vorbisData->dataSize - vorbisData->dataRead;
		if (offset < spaceToEOF)
			actualOffset = (offset);
		else
			actualOffset = spaceToEOF;	
		// Seek from our currrent location
		vorbisData->dataRead += actualOffset;
		break;
	case SEEK_END: // Seek from the end of the file
		vorbisData->dataRead = vorbisData->dataSize+1;
		break;
	default:
		break;
	};

	return 0;
}

//---------------------------------------------------------------------------------
// Function	: VorbisClose
// Purpose	: Callback for the Vorbis close function
// Info		: 
//---------------------------------------------------------------------------------
int VorbisClose(void *datasource /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/)
{
	// This file is called when we call ov_close.  If we wanted, we could free our memory here, but
	// in this case, we will free the memory in the main body of the program, so dont do anything
	return 1;
}

//---------------------------------------------------------------------------------
// Function	: VorbisTell
// Purpose	: Classback for the Vorbis tell function
// Info		: 
//---------------------------------------------------------------------------------
long VorbisTell(void *datasource /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/)
{
	SOggFile*	vorbisData;

	// Get the data in the right format
	vorbisData = (SOggFile*)datasource;

	// We just want to tell the vorbis libs how much we have read so far
	return vorbisData->dataRead;
}