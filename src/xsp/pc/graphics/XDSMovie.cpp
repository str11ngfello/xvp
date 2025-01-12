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
//	Module Name:	XDSMovie.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-31-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
//*****************************************************************************
XDSMovie::XDSMovie()
{
	//m_pGraph = NULL;
	//m_pMediaControl = NULL;
	//m_pMediaPosition = NULL;
	//m_pMediaEvent = NULL;
	//m_pD3DDevice = NULL;
	m_playCount = 0;
	m_isPlaying = false;
	m_width = 0;
	m_height = 0;

}



//*****************************************************************************
//*****************************************************************************
XDSMovie::~XDSMovie()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::init(XS8* pFilename)
{
	//IBaseFilter    *pFilter        = NULL;
	//		IPin           *pFilterPinIn   = NULL;
	//		IBaseFilter    *pSourceFilter  = NULL;
	//		IPin           *pSourcePinOut  = NULL;

	//		WCHAR           wFilename[MAX_PATH];
	//		HRESULT         hr;

	//		// Store device
	//		if((m_pD3DDevice = XVP::GraphicsMgr.m_pD3DDevice) == NULL)
	//			return X_ERR_OPERATION_FAILED;

	//		// Create the filter graph manager
	//		CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, 
	//						IID_IGraphBuilder, (void **)&m_pGraph);

	//		// Add filter
	//		XNew(pTextureFilter,cTextureFilter(m_pD3DDevice, NULL, &hr),XOSMgr::getDefaultHeap())
	//		pFilter = pTextureFilter;
	//		m_pGraph->AddFilter(pFilter, L"ANIMATEDTEXTURE");

	//		// Add source filter
	//		mbstowcs(wFilename, pFilename, MAX_PATH);
	//		m_pGraph->AddSourceFilter(wFilename, L"SOURCE", &pSourceFilter);

	//		// Find the input and out pins and connect together
	//		//pFilter->FindPin(L"In", &pFilterPinIn);
	//		pSourceFilter->FindPin(L"Output", &pSourcePinOut);
	//		m_pGraph->Render(pSourcePinOut);

	//		// Query for interfaces
	//		m_pGraph->QueryInterface(IID_IMediaControl,  (void **)&m_pMediaControl);
	//		m_pGraph->QueryInterface(IID_IMediaPosition, (void **)&m_pMediaPosition);
	//		m_pGraph->QueryInterface(IID_IMediaEvent,    (void **)&m_pMediaEvent);

	//		// Save the width and height.
	//		m_width = pTextureFilter->m_lVideoWidth;
	//		m_height = pTextureFilter->m_lVideoHeight;

	//		// Free interfaces used here
	//		//SAFE_RELEASE(pFilterPinIn);
	//		SAFE_RELEASE(pSourceFilter);
	//		SAFE_RELEASE(pSourcePinOut);

			return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::destroy()
{
	//Stop any playback
	stop();

	//Clear Direct3D object pointer
	//m_pD3DDevice = NULL;

	XResource::destroy();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
ID3D10ShaderResourceView* XDSMovie::getD3DTexture()
{
	return NULL;
	//return pTextureFilter->GetTexture();
}

	

//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::play(int playCount)
{
	m_playCount = playCount;
	//gotoTime(0);
	m_isPlaying = true;
			//if(m_pMediaControl != NULL)
			//	m_pMediaControl->Run();
			return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::pause()
{
	return stop(); 
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::resume()
{
	if (m_playCount > 0)
	{
		m_isPlaying = true;
		//if(m_pMediaControl != NULL)
		//	m_pMediaControl->Run();
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::stop()
{
	m_isPlaying = false;
	//		if(m_pMediaControl != NULL)
	//			m_pMediaControl->Stop();
			return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::setTime(XU32 milliseconds)
{
	/*REFTIME time;
	m_pMediaPosition->get_Duration(&time);

	if (milliseconds > (time*1000))
		return gotoTime(0);
	else
		return gotoTime(milliseconds/1000.0f);
		*/
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XDSMovie::getTime()
{
	/*if(m_pMediaPosition != NULL)
	{
		REFTIME time;
		m_pMediaPosition->get_CurrentPosition(&time);
		return (XU32)(time * 1000);
	}
	else
		return 0;*/
	return 0;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::setPlayCount(int playCount)
{
	m_playCount = playCount;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS32 XDSMovie::getPlayCount()
{
	return m_playCount;
}



//*****************************************************************************
//*****************************************************************************
XU32 XDSMovie::getDuration()
{
	/*
	if (m_pMediaPosition != NULL)
	{
		REFTIME time;
		m_pMediaPosition->get_Duration(&time);
		return (XU32)(time * 1000);
	}
	else
		return 0;
		*/
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XDSMovie::getWidth()
{
	return m_width;
}



//*****************************************************************************
//*****************************************************************************
XU32 XDSMovie::getHeight()
{
	return m_height;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XDSMovie::isPlaying()
{
	return m_isPlaying;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::endOfAnimation()
{
	//If this is not an infinite loop animation then decrement
	//the m_playCount.
	if (m_playCount > 0)
		--m_playCount;
	if (m_playCount != 0)
		restart();
	else
		stop();
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::update()
{/*
	long lEventCode;
	long lParam1;
	long lParam2;

			// Error checking
			if(!m_pMediaEvent)
				return X_ERR_OPERATION_FAILED;

			// Process all waiting events
			while(1)
			{
				// Get the event
				if(FAILED(m_pMediaEvent->GetEvent(&lEventCode, &lParam1, &lParam2, 0)))
					break;

				// Call the end of animation function if playback complete
				if(lEventCode == EC_COMPLETE)
					endOfAnimation();

				// Free the event resources
				m_pMediaEvent->FreeEventParams(lEventCode, lParam1, lParam2);
			}
			  */
			return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDSMovie::restart()
{
	//return gotoTime(0);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
//XStatus XDSMovie::gotoTime(REFTIME Time)
//{
//	if(m_pMediaPosition != NULL)
//		m_pMediaPosition->put_CurrentPosition(Time);
//	return X_OK;
//}