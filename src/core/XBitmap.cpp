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
//	Module Name:	XBitmap.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the bitmap.
*/
//*****************************************************************************
XBitmap::XBitmap()
{
	m_pixels = 0;
	m_width = 0;
	m_height = 0;
}



//*****************************************************************************
/**
Destructor for the bitmap.
*/
//*****************************************************************************
XBitmap::~XBitmap()
{
}



//*****************************************************************************
/**
Creates a raw bitmap object by allocating (width * height) pixels in 24-bit
RGB format.  The bitmap is cleared to black by default.

@param width	Bitmap width in pixels.
@param height	Bitmap height in pixels.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XBitmap::create(XU32 width,XU32 height)
{	
	m_width  = width;
	m_height = height;
	XNewArray(m_pixels,bmpPAL,m_width*m_height,XOSMgr::getDefaultHeap());
	if (!m_pixels)
		return X_ERR_MEMORY_EXHAUSTED;

	//Clear the pixels to black by default.
	memset(m_pixels,0,sizeof(bmpPAL)*m_width*m_height);

	//Assign default values to headers
	m_h1.bfType = 19778;
	m_h1.bfSize = (m_width*m_height*3)+56;           
    m_h1.bfReserved1 = 0;      
    m_h1.bfReserved2 = 0;      
    m_h1.bfOffBits = 54;        
	m_h2.biSize = 40;           
    m_h2.biWidth = m_width;          
    m_h2.biHeight = m_height;         
    m_h2.biPlanes = 1;         
    m_h2.biBitCount = 24;       
    m_h2.biCompression = 0;    
    m_h2.biSizeImage = 0;      
    m_h2.biXPelsPerMeter = 2834;  
    m_h2.biYPelsPerMeter = 2834;  
    m_h2.biClrUsed = 0;        
    m_h2.biClrImportant = 0;   

	return X_OK;
}



//*****************************************************************************
/**
Destroys the bitmap by freeing all of its allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XBitmap::destroy()
{
	XDeleteArray(m_pixels);

	return X_OK;
}



//*****************************************************************************
/**
Gets the red, green and blue component of the specified pixel location in the
bitmap.

@param x	x coordinate of pixel.
@param y	y coordinate of pixel.
@param pR	Byte pointer to be filled with red component value of the pixel.
			This value will be in the range 0-255.
@param pG	Byte pointer to be filled with green component value of the pixel.
			This value will be in the range 0-255.
@param pB	Byte pointer to be filled with blue component value of the pixel.
			This value will be in the range 0-255.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XBitmap::getColor(int x,int y,XS8* pR,XS8* pG,XS8* pB) 
{
	//Translate the x and y coordinate.  Bitmaps are stored upside down.
	if ((x <= m_height-1) && (x >= 0) && (y <= m_width-1) && (y >= 0)
		&& (m_pixels != NULL))
	{
		*pR = m_pixels[(((m_width*m_height) - ((y+1)*m_height)) + x)].r;
		*pG = m_pixels[(((m_width*m_height) - ((y+1)*m_height)) + x)].g;
		*pB = m_pixels[(((m_width*m_height) - ((y+1)*m_height)) + x)].b;

		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Sets the red, green and blue component of the specified pixel location in the
bitmap.

@param x	x coordinate of pixel.
@param y	y coordinate of pixel.
@param r	Red component value of the pixel.  This value must be in the range
			0-255.
@param g	Green component value of the pixel.  This value must be in the range
			0-255.
@param b	Blue component value of the pixel.  This value must be in the range
			0-255.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XBitmap::setColor(int x,int y,XU8 r,XU8 g,XU8 b) 
{
	//Translate the x and y coordinate.  Bitmaps are stored upside down.
	if ((x <= m_height-1) && (x >= 0) && (y <= m_width-1) && (y >= 0)
		&& (m_pixels != NULL))
	{
		m_pixels[(((m_width*m_height) - ((y+1)*m_height)) + x)].r = r;
		m_pixels[(((m_width*m_height) - ((y+1)*m_height)) + x)].g = g;
		m_pixels[(((m_width*m_height) - ((y+1)*m_height)) + x)].b = b;

		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Loads in a 24-bit bitmap from a file. This method will fail if the specified
bitmap is not in 24-bit RGB format.

@param pFilename	Path to 24-bit bitmap file. 
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XBitmap::load(XS8* pFilename)
{
	//Verify filename
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;

	XDeleteArray(m_pixels);

	//Open file for reading.
	FILE* pInputFile = fopen(pFilename, "rb");
	if (!pInputFile)
		return X_ERR_INVALID_PARAMS;

	fread(&m_h1.bfType,sizeof(m_h1.bfType),1,pInputFile);
	fread(&m_h1.bfSize,sizeof(m_h1.bfSize),1,pInputFile);
	fread(&m_h1.bfReserved1,sizeof(m_h1.bfReserved1),1,pInputFile);
	fread(&m_h1.bfReserved2,sizeof(m_h1.bfReserved2),1,pInputFile);
	fread(&m_h1.bfOffBits,sizeof(m_h1.bfOffBits),1,pInputFile);
	fread(&m_h2.biSize,sizeof(m_h2.biSize),1,pInputFile);
	fread(&m_h2.biWidth,sizeof(m_h2.biWidth),1,pInputFile);
	fread(&m_h2.biHeight,sizeof(m_h2.biHeight),1,pInputFile);
	fread(&m_h2.biPlanes,sizeof(m_h2.biPlanes),1,pInputFile);
	fread(&m_h2.biBitCount,sizeof(m_h2.biBitCount),1,pInputFile);
	fread(&m_h2.biCompression,sizeof(m_h2.biCompression),1,pInputFile);
	fread(&m_h2.biSizeImage,sizeof(m_h2.biSizeImage),1,pInputFile);
	fread(&m_h2.biXPelsPerMeter,sizeof(m_h2.biXPelsPerMeter),1,pInputFile);
	fread(&m_h2.biYPelsPerMeter,sizeof(m_h2.biYPelsPerMeter),1,pInputFile);
	fread(&m_h2.biClrUsed,sizeof(m_h2.biClrUsed),1,pInputFile);
	fread(&m_h2.biClrImportant,sizeof(m_h2.biClrImportant),1,pInputFile);

	//Record width and height.
	m_width  = m_h2.biWidth;
	m_height = m_h2.biHeight;

	//Make sure bit count is 24.
	if (m_h2.biBitCount != 24)
		return X_ERR_OPERATION_FAILED;

	//Allocate memory for pixels.
	XNewArray(m_pixels,bmpPAL,m_width*m_height,XOSMgr::getDefaultHeap());
	if(!m_pixels) 
		return X_ERR_MEMORY_EXHAUSTED;
	
	//Read in the entire bitmap.
	fread(m_pixels,sizeof(bmpPAL),m_width*m_height,pInputFile);
	fclose(pInputFile);

	return X_OK;
}



//*****************************************************************************
/**
Saves the bitmap to a file in 24-bit RGB format.

@param pFilename	The path to save the bitmap to.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XBitmap::save(XS8* pFilename)
{
	FILE* pOutputFile = fopen(pFilename, "wb");

	if ((!m_pixels) || (!pOutputFile))					
	{
		fclose (pOutputFile); 
		return X_ERR_OPERATION_FAILED; 
	}

    fwrite(&m_h1.bfType,sizeof(m_h1.bfType), 1, pOutputFile);
	fwrite(&m_h1.bfSize,sizeof(m_h1.bfSize), 1, pOutputFile);
	fwrite(&m_h1.bfReserved1,sizeof(m_h1.bfReserved1), 1, pOutputFile);
	fwrite(&m_h1.bfReserved2,sizeof(m_h1.bfReserved2), 1, pOutputFile);
	fwrite(&m_h1.bfOffBits,sizeof(m_h1.bfOffBits), 1, pOutputFile);
	fwrite(&m_h2.biSize,sizeof(m_h2.biSize),1,pOutputFile);
	fwrite(&m_h2.biWidth,sizeof(m_h2.biWidth),1,pOutputFile);
	fwrite(&m_h2.biHeight,sizeof(m_h2.biHeight),1,pOutputFile);
	fwrite(&m_h2.biPlanes,sizeof(m_h2.biPlanes),1,pOutputFile);
	fwrite(&m_h2.biBitCount,sizeof(m_h2.biBitCount),1,pOutputFile);
	fwrite(&m_h2.biCompression,sizeof(m_h2.biCompression),1,pOutputFile);
	fwrite(&m_h2.biSizeImage,sizeof(m_h2.biSizeImage),1,pOutputFile);
	fwrite(&m_h2.biXPelsPerMeter,sizeof(m_h2.biXPelsPerMeter),1,pOutputFile);
	fwrite(&m_h2.biYPelsPerMeter,sizeof(m_h2.biYPelsPerMeter),1,pOutputFile);
	fwrite(&m_h2.biClrUsed,sizeof(m_h2.biClrUsed),1,pOutputFile);
	fwrite(&m_h2.biClrImportant,sizeof(m_h2.biClrImportant),1,pOutputFile);
	fwrite(m_pixels,sizeof(bmpPAL),m_width*m_height,pOutputFile);
	
	//Close the file.
	fclose(pOutputFile);

	return X_OK;
}



//*****************************************************************************
/**
Gets the width of the bitmap in pixels.

@return The width of the bitmap in pixels.
*/
//*****************************************************************************
XU32 XBitmap::getWidth()
{
	return m_width;
}



//*****************************************************************************
/**
Gets the height of the bitmap in pixels.

@return The height of the bitmap in pixels.
*/
//*****************************************************************************
XU32 XBitmap::getHeight()
{
	return m_height;
}



