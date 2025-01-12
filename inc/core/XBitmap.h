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
//	Module Name:	XBitmap.h
//	Author:			Lynn Duke
//	Creation Date:	12-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BITMAP_H__
#define __X_BITMAP_H__




//*****************************************************************************
//! Bitmap file reader and writer for 24-bit bitmaps.
/**
The XBitmap object allows you read, write and modify 24-bit bitmaps.  This
object provides no rendering capabilites.
*/
//*****************************************************************************
class XBitmap
{
private:

	//*****************************************************************************
	//*****************************************************************************
	struct bmpFHEAD
	{
		unsigned short bfType;           
		unsigned int   bfSize;           
		unsigned short bfReserved1;      
		unsigned short bfReserved2;      
		unsigned int   bfOffBits;        
	};



	//*****************************************************************************
	//*****************************************************************************
	struct bmpIHEAD
	{
		unsigned int   biSize;           
		int            biWidth;          
		int            biHeight;         
		unsigned short biPlanes;         
		unsigned short biBitCount;       
		unsigned int   biCompression;    
		unsigned int   biSizeImage;      
		int            biXPelsPerMeter;  
		int            biYPelsPerMeter;  
		unsigned int   biClrUsed;        
		unsigned int   biClrImportant;   
	};



	//*****************************************************************************
	//*****************************************************************************
	struct bmpPAL
	{
		unsigned char b;
		unsigned char g;
		unsigned char r;
	};
	
	int			m_width;
	int			m_height;
	bmpFHEAD	m_h1;
	bmpIHEAD	m_h2;
	bmpPAL*		m_pixels;
	
public:
	//*****************************************************************************
	//Constructor/Destructor
	//*****************************************************************************
	XBitmap();
	virtual ~XBitmap();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus create(XU32 width,XU32 height);
	XStatus load(XS8* pFilename);
	XStatus save(XS8* pFilename);
	XStatus destroy();
	XStatus getColor(int x,int y,XS8* pR,XS8* pG,XS8* pB);
	XStatus setColor(int x,int y,XU8 r,XU8 g,XU8 b);
	XU32 getWidth(); 
	XU32 getHeight(); 
};

#endif 
