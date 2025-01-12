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
//	Module Name:	XDSMovie.h
//	Author:			Lynn Duke
//	Creation Date:	1-31-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_DS_MOVIE_H__
#define __X_DS_MOVIE_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XBaseMovie.h>



#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! DirectShow Movie.
//*****************************************************************************
class XDSMovie : public XBaseMovie
{
private:
	//*****************************************************************************
	//Internal Movie Objects (DirectShow)
	//*****************************************************************************
	/*struct __declspec(uuid("{61DA4980-0487-11d6-9089-00400536B95F}")) CLSID_AnimatedTexture;

	class cTextureFilter : public CBaseVideoRenderer
	{
	public:
		IDirect3DDevice9*  m_pD3DDevice;    // 3-D device
		IDirect3DTexture9* m_pTexture;      // Video texture storage
		D3DFORMAT          m_Format;

		XS32               m_lVideoWidth;   // Width of video surface
		XS32               m_lVideoHeight;  // Height of video surface
		XS32               m_lVideoPitch;   // Pitch of video surface

	public:
		cTextureFilter::cTextureFilter(IDirect3DDevice9* pD3DDevice, 
										LPUNKNOWN pUnk, HRESULT *phr) 
					: CBaseVideoRenderer(__uuidof(CLSID_AnimatedTexture), 
					NAME("ANIMATEDTEXTURE"), pUnk, phr)
		{
			// Save device pointer
			m_pD3DDevice  = pD3DDevice;

			// Return success
			*phr = S_OK;
		}

		HRESULT cTextureFilter::CheckMediaType(const CMediaType *pMediaType)
		{
			// Only accept video type media
			if(*pMediaType->FormatType() != FORMAT_VideoInfo)
				return E_INVALIDARG;

			// Make sure media data is using RGB 24-bit color format
			if(IsEqualGUID(*pMediaType->Type(),    MEDIATYPE_Video) &&
				IsEqualGUID(*pMediaType->Subtype(), MEDIASUBTYPE_RGB24))
				return S_OK;

			return E_FAIL;
		}

		HRESULT cTextureFilter::SetMediaType(const CMediaType *pMediaType)
		{
			HRESULT          hr;
			VIDEOINFO       *pVideoInfo;
			D3DSURFACE_DESC  ddsd;

			// Retrive the size of this media type
			pVideoInfo     = (VIDEOINFO *)pMediaType->Format();
			m_lVideoWidth  = pVideoInfo->bmiHeader.biWidth;
			m_lVideoHeight = abs(pVideoInfo->bmiHeader.biHeight);
			m_lVideoPitch  = (m_lVideoWidth * 3 + 3) & ~(3);

			// Create the texture that maps to this media type
			if(FAILED(hr = D3DXCreateTexture(m_pD3DDevice,
							m_lVideoWidth, m_lVideoHeight,
							1, 0, D3DFMT_A8R8G8B8, 
							D3DPOOL_MANAGED, &m_pTexture)))
				return hr;
			
			// Get texture description and verify settings
			if(FAILED(hr = m_pTexture->GetLevelDesc(0, &ddsd)))
				return hr;
			m_Format = ddsd.Format;
			if(m_Format != D3DFMT_A8R8G8B8 && m_Format != D3DFMT_A1R5G5B5)
				return VFW_E_TYPE_NOT_ACCEPTED;

			return S_OK;
		}

		HRESULT cTextureFilter::DoRenderSample(IMediaSample *pMediaSample)
		{
			// Get a pointer to video sample buffer
			XU8 *pSamplePtr;
			pMediaSample->GetPointer(&pSamplePtr);
			
			// Lock the texture surface
			D3DLOCKED_RECT d3dlr;
			if(FAILED(m_pTexture->LockRect(0, &d3dlr, 0, 0)))
				return E_FAIL;

			// Get texture pitch and pointer to texture data
			XU8 *pTexturePtr  = (XU8*)d3dlr.pBits;
			XS32 lTexturePitch = d3dlr.Pitch;

			// Offset texture to bottom line, since video 
			// is stored upside down in buffer
			pTexturePtr += (lTexturePitch * (m_lVideoHeight-1));
			  
			// Copy the bits using specified video format
			int x, y, SrcPos, DestPos;
			switch(m_Format) {
				case D3DFMT_A8R8G8B8:  // 32-bit

				// Loop through each row, copying bytes as you go
				for(y=0;y<m_lVideoHeight;y++) {

					// Copy each column
					SrcPos = DestPos = 0;
					for(x=0;x<m_lVideoWidth;x++) {
					pTexturePtr[DestPos++] = pSamplePtr[SrcPos++];
					pTexturePtr[DestPos++] = pSamplePtr[SrcPos++];
					pTexturePtr[DestPos++] = pSamplePtr[SrcPos++];
					pTexturePtr[DestPos++] = 0xff;
					}

					// Move pointers to next line
					pSamplePtr  += m_lVideoPitch;
					pTexturePtr -= lTexturePitch;
				}
				break;

				case D3DFMT_A1R5G5B5:  // 16-bit

				// Loop through each row, copying bytes as you go
				for(y=0;y<m_lVideoHeight;y++) {

					// Copy each column
					SrcPos = DestPos = 0;
					for(x=0;x<m_lVideoWidth;x++) {
					*(WORD*)pTexturePtr[DestPos++] = 0x8000 + 
							((pSamplePtr[SrcPos+2] & 0xF8) << 7) +
							((pSamplePtr[SrcPos+1] & 0xF8) << 2) +
							(pSamplePtr[SrcPos] >> 3);
					SrcPos += 3;
					}
			         
					// Move pointers to next line
					pSamplePtr  += m_lVideoPitch;
					pTexturePtr -= lTexturePitch;
				}
				break;
			}
			      
			// Unlock the Texture
			if(FAILED(m_pTexture->UnlockRect(0)))
				return E_FAIL;

			return S_OK;
		}

		IDirect3DTexture9* cTextureFilter::GetTexture()
		{
			return m_pTexture;
		}
	};


	IGraphBuilder     *m_pGraph;          //Filter graph
	IMediaControl     *m_pMediaControl;   //Playback control
	IMediaPosition    *m_pMediaPosition;  //Positioning control
	IMediaEvent       *m_pMediaEvent;     //Event control

	cTextureFilter    *pTextureFilter;
	IDirect3DDevice9  *m_pD3DDevice;      //3-D device
*/
	int					m_playCount;
	bool				m_isPlaying;
	XU32				m_width;
	XU32				m_height;

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XDSMovie();
	~XDSMovie();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	XStatus init(XS8* pFilename);
	XStatus destroy();
	ID3D10ShaderResourceView* getD3DTexture();
	XStatus play(int playCount);
	XStatus pause();
	XStatus resume();
	XStatus stop();
	XStatus setTime(XU32 milliseconds);
	XU32 getTime();
	XStatus setPlayCount(XS32 playCount);
	XS32 getPlayCount();
	XU32 getDuration();
	XU32 getWidth();
	XU32 getHeight();
	bool isPlaying();
	XStatus update();
	XStatus endOfAnimation();
	XStatus restart();
	//XStatus gotoTime(REFTIME Time);
};


#endif