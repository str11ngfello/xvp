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
//	Module Name:	XTextureSequence.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-5-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XTextureSequence::XTextureSequence()
{
	m_currentIndex = 0;
	m_elapsedTime = 0;
	m_delayFrameTime = 1;
	m_numFramesToElapse = 0;
	m_width = 0;
	m_height = 0;
	m_playing = true;
}



//*****************************************************************************
//*****************************************************************************
XTextureSequence::~XTextureSequence()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XTextureSequence::init(XS8* pFilenameBase,XS8* pExtension,XU32 numFiles)
{

	//Validate parameters.
	if ((!pFilenameBase) || (!pExtension) || (numFiles == 0))
		return X_ERR_INVALID_PARAMS;

	//Destroy first
	destroy();
	XTextureSequence();

	//Store the base string
	char pBase[X_MAX_PATH_SIZE];
	char pTempFilename[X_MAX_PATH_SIZE];
	char pTempBuf[15];
	XBOOL firstTime = true;
	XHandle tempHandle;
	strcpy(pBase,pFilenameBase);
	
	for (XU32 i = 0;i < numFiles;++i)
	{
		strcpy(pTempFilename,pBase);
		_itoa(i,pTempBuf,10);
		strcat(pTempFilename,pTempBuf);
		strcat(pTempFilename,pExtension);
		if (XOSMgr::fileExists(pTempFilename))
		{
			if (XVP::GraphicsMgr.createTextureFromFile(pTempFilename,tempHandle) == X_OK)
			{
				m_textureHandles.push_back(tempHandle);
				if (firstTime)
				{
					m_width = XVP::GraphicsMgr.getTextureWidth(tempHandle);
					m_height = XVP::GraphicsMgr.getTextureHeight(tempHandle);
					firstTime = false;
				}
			}
			else
				return X_ERR_PARTIAL_FAILURE;
		}
		else
			return X_ERR_PARTIAL_FAILURE;
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTextureSequence::destroy()
{
	//Destroy all textures in the sequence
	for (XU32 i = 0;i < m_textureHandles.size();++i)
	{
		XVP::GraphicsMgr.destroyTexture(m_textureHandles[i]);
		m_textureHandles[i] = X_INVALID_HANDLE;
	}
	m_currentIndex = 0;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTextureSequence::update(XF32 dt)
{
	if (!m_playing)
		return X_OK;

	//Add dt which is the amount of time that has passed since
	//we last updated the texture sequence
	m_elapsedTime += dt;

	//If enough time has elapsed such that at least 1 whole frame should
	//have been elapsed, then we need to determine the number of frames
	//that we should skip based on the amount of time as well as keep
	//the left over amount of elapsed time that does not contribute to 
	//the elapsed frames for this update.  We keep this "left over" amount
	//in the m_elapsedTime variable for use during the next update method.
	if ((m_elapsedTime / m_delayFrameTime) >= 1.0)
	{
		modf((m_elapsedTime / m_delayFrameTime),&m_numFramesToElapse);
		m_elapsedTime = fmod(m_elapsedTime,m_delayFrameTime);
	}
	else
		m_numFramesToElapse = 0;

	while (m_numFramesToElapse > 0)
	{
		++m_currentIndex;
		if (m_currentIndex > (m_textureHandles.size()-1))
			m_currentIndex = 0;
		m_numFramesToElapse -= 1;
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTextureSequence::pause()
{
	m_playing = false;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTextureSequence::resume()
{
	m_playing = true;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTextureSequence::setFrameDelay(XF32 frameDelay)
{
	if (frameDelay <= 0)
		m_delayFrameTime = 1;
	else
		m_delayFrameTime = frameDelay;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XTextureSequence::getFrameDelay()
{
	return m_delayFrameTime;
}



//*****************************************************************************
//*****************************************************************************
XHandle XTextureSequence::getCurrentHandle()
{
	return m_textureHandles[m_currentIndex];
}



//*****************************************************************************
//*****************************************************************************
XU32 XTextureSequence::getNumTextures()
{
	return (XU32)m_textureHandles.size();
}



//*****************************************************************************
//*****************************************************************************
XU32 XTextureSequence::getWidth()
{
	return m_width;
}



//*****************************************************************************
//*****************************************************************************
XU32 XTextureSequence::getHeight()
{
	return m_height;
}

