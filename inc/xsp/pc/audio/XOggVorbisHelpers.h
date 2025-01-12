	
#ifndef __X_OGG_VORBIS_HELPERS_
#define __X_OGG_VORBIS_HELPERS_

#include <XOSMgr.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

struct SOggFile
	{
		char*		dataPtr;			// Pointer to the data in memoru
		int			dataSize;			// Sizeo fo the data
		int			dataRead;			// How much data we have read so far
	};

size_t VorbisRead(void *ptr			/* ptr to the data that the vorbis files need*/, 
				  size_t byteSize	/* how big a byte is*/, 
				  size_t sizeToRead /* How much we can read*/, 
				  void *datasource	/* this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/);
int VorbisSeek(void *datasource		/*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/, 
			   ogg_int64_t offset	/*offset from the point we wish to seek to*/, 
			   int whence			/*where we want to seek to*/);
int VorbisClose(void *datasource /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/);
long VorbisTell(void *datasource /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/);

#endif