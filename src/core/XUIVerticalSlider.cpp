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
//	Module Name:	XUIVerticalSlider.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-21-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the user interface slider.
*/
//*****************************************************************************
XUIVerticalSlider::XUIVerticalSlider()
{
	m_percentage = 0.0f;
	m_prevPercentage = 0.0f;
	m_fill = false;
	m_sliderState = X_UISS_NOT_SLIDING;
	m_nativeKnobHeight = 0;
	m_scaledKnobHeight = 0;
	m_inSoundHandle = X_INVALID_HANDLE;
	m_outSoundHandle = X_INVALID_HANDLE;
}



//*****************************************************************************
/**
Destructor for the user interface slider.
*/
//*****************************************************************************
XUIVerticalSlider::~XUIVerticalSlider()
{
}



//*****************************************************************************
/**
Initializes the user interface slider.

@param pParentUI	The user interface to which this slider belongs.
@param ID	The element ID used to identify this user interface slider within
			a user interface.
@param zOrder	The zOrder of the user interface slider with respect to other
				user interface elements. 
@param screenRect	The rectangular area of the screen in which the user interface
					slider will play.
@param pTrackTexFilename	The image to be used for the track of the slider.
@param pFillTexFilename	The image to be used to fill the slider track.
@param pKnobTexFilename	The image to be used for as the knob of the slider.
@param pInSoundFilename	The sound that is played when the mouse is depressed over the
						slider.
@param pOutSoundFilename    The sound that is played when the mouse is released over
							the slider.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIVerticalSlider::init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
								XS8* pTemplateFile)
{
	//Validate parameters.
	if (!pTemplateFile)
		return X_ERR_INVALID_PARAMS;

	char path[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_UI_TEMPLATE,pTemplateFile,path);

	TiXmlDocument file(path);
	if (!file.LoadFile())
		return X_ERR_OPERATION_FAILED;
	else
	{
		TiXmlNode* pNode = NULL;
		TiXmlElement* pSliderElement = NULL;
		
		pNode = file.FirstChild( "slider" );
		if (pNode)
		{
			pSliderElement = pNode->ToElement();
			if (pSliderElement)
			{	
				XRect knobRect,trackRect;
				XS8* pFilename = NULL;

				pSliderElement->QueryIntAttribute("KnobSrcLeft",&knobRect.left);
				pSliderElement->QueryIntAttribute("KnobSrcTop",&knobRect.top);
				pSliderElement->QueryIntAttribute("KnobSrcRight",&knobRect.right);
				pSliderElement->QueryIntAttribute("KnobSrcBottom",&knobRect.bottom);

				pSliderElement->QueryIntAttribute("TrackSrcLeft",&trackRect.left);
				pSliderElement->QueryIntAttribute("TrackSrcTop",&trackRect.top);
				pSliderElement->QueryIntAttribute("TrackSrcRight",&trackRect.right);
				pSliderElement->QueryIntAttribute("TrackSrcBottom",&trackRect.bottom);
				
				pFilename = (XS8*)pSliderElement->Attribute("TrackTextureFile");
				if (pFilename)
				{
					if (m_trackSprite.init(pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_trackSprite.setSrcRect(trackRect);
				}
				else
					return X_ERR_OPERATION_FAILED;

				pFilename = (XS8*)pSliderElement->Attribute("KnobTextureFile");
				if (pFilename)
				{
					if (m_knobSprite.init(pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_knobSprite.setSrcRect(knobRect);
				}
				else
					return X_ERR_OPERATION_FAILED;

				m_percentage = 0;
				m_nativeScreenRect = screenRect;
				//m_knobSprite.getDestRect(r);
				m_nativeKnobHeight = knobRect.getHeight();

				pFilename = (XS8*)pSliderElement->Attribute("FillTextureFile");
				if (pFilename)
				{
					if (m_fillSprite.init(pFilename) == X_OK)
						m_fill = true;
				}

				pFilename = (XS8*)pSliderElement->Attribute("DownSoundFile");
				if (pFilename)
					XVP::AudioMgr.createSoundFromFile(pFilename,m_inSoundHandle);
				pFilename = (XS8*)pSliderElement->Attribute("UpSoundFile");
				if (pFilename)
					XVP::AudioMgr.createSoundFromFile(pFilename,m_outSoundHandle);
			}
			else
				return X_ERR_OPERATION_FAILED;
		}
		else
			return X_ERR_OPERATION_FAILED;
	
		m_zOrderOffset = zOrder;
		m_pParentUI = pParentUI;
		XUIElement::setOwner(pParentUI,this,zOrder);

		return X_OK;
	}

	/*
	//Validate parameters.
	if ((!pTrackTexFilename) || (!pKnobTexFilename))
		return X_ERR_INVALID_PARAMS;


	//Initialize the sprites.
	XStatus status = m_trackSprite.init(pTrackTexFilename);
	if (status == X_OK)
	{
		status = m_knobSprite.init(pKnobTexFilename);
		if (status == X_OK)
		{
			XRect r;
			m_percentage = 0;
			m_nativeScreenRect = screenRect;
			m_knobSprite.getDestRect(r);
			m_nativeKnobHeight = r.getHeight();
			
			//Create a fill texture if needed by the user.
			if (pFillTexFilename)
				if (m_fillSprite.init(pFillTexFilename) == X_OK)
					m_fill = true;

			//Create the sounds if necessary.
			if (pInSoundFilename)
				XVP::AudioMgr.createSoundFromFile(pInSoundFilename,m_inSoundHandle);
			if (pOutSoundFilename)
				XVP::AudioMgr.createSoundFromFile(pOutSoundFilename,m_outSoundHandle);			
		}
	}
	
	m_zOrderOffset = zOrder;
	m_pParentUI = pParentUI;
	XUIElement::setOwner(pParentUI,this,zOrder);

	return status;*/
}



//*****************************************************************************
/**
Destroys the user interface slider and frees all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIVerticalSlider::destroy()
{
	m_trackSprite.destroy();
	m_knobSprite.destroy();
	m_fillSprite.destroy();

	if (m_inSoundHandle != X_INVALID_HANDLE)
		XVP::AudioMgr.destroySound(m_inSoundHandle);
	if (m_outSoundHandle != X_INVALID_HANDLE)
		XVP::AudioMgr.destroySound(m_outSoundHandle);

	m_inSoundHandle = X_INVALID_HANDLE;
	m_outSoundHandle = X_INVALID_HANDLE;

	///XUIElement::removeOwner(m_pParentUI,this);
	XUIElement::destroy();
	return X_OK;
}



//*****************************************************************************
/**
Updates the user interface slider.  This function is called automatically by the
engine once per frame.

@param dt	The amount of time, in seconds, that has elapsed since
					the last update.
@param xScale	The horizontal scale factor relative to the original
				width of the user interface slider.
@param yScale	The vertical scale factor relative to the original
				height of the user interface slider.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIVerticalSlider::update(XF32 dt)
{
		if (m_sliderState == X_UISS_SLIDING)
		{	
			XVector2D hotSpot = XUserInterface::getCursor()->getHotSpot();
			XS32 widthTolerance = (m_scaledScreenRect.right - m_scaledScreenRect.left)*6;
			XS32 heightTolerance = m_scaledScreenRect.bottom - m_scaledScreenRect.top;

			//Accept the movement only if we are within the slider region
			if (hotSpot.y < (m_scaledScreenRect.top))
				m_percentage = 1.0f;
			else if (hotSpot.y > m_scaledScreenRect.bottom)
				m_percentage = 0;
			else				
			{
				m_percentage  -= (XF32)XUserInterface::getCursor()->getRelYMov()/*XVP::InputMgr.getMouseRelXMovement()*//(XF32)( m_scaledScreenRect.bottom - m_scaledScreenRect.top - m_scaledKnobHeight);//   (XF32)(delta - m_scaledScreenRect.left)/(XF32)(m_scaledScreenRect.right - m_scaledScreenRect.left - m_scaledKnobHeight);			
				if (m_percentage > 1.0)
					m_percentage = 1.0;
				if (m_percentage < 0)
					m_percentage = 0;
			}	

			if (hotSpot.x > (m_scaledScreenRect.right + widthTolerance) ||
				hotSpot.x < (m_scaledScreenRect.left - widthTolerance) ||
				hotSpot.y < (m_scaledScreenRect.top - heightTolerance) ||
				hotSpot.y > m_scaledScreenRect.bottom + heightTolerance)
			{
				m_sliderState = X_UISS_NOT_SLIDING;
				onEndSlide(true);
			}
			else if (XVP::InputMgr.mouseButtonUpOnce(X_MB_LEFT) ||
				XVP::InputMgr.mouseButtonUpOnce(X_MB_RIGHT) ||
				XVP::InputMgr.mouseButtonUpOnce(X_MB_MIDDLE))
			{
								m_sliderState = X_UISS_NOT_SLIDING;
				onEndSlide(false);

			}
			
			//If the percentage changed notify the appropriate event.
			if (m_percentage != m_prevPercentage)
			{
				m_prevPercentage = m_percentage;
                onChange();
			}
		}

	
	m_scaledKnobRect.left = m_scaledScreenRect.left;
	m_scaledKnobRect.right = m_scaledScreenRect.right;
	m_scaledKnobRect.top = (XS32)(m_scaledScreenRect.bottom - m_scaledKnobHeight - ((m_scaledScreenRect.bottom - m_scaledScreenRect.top - m_scaledKnobHeight) * m_percentage));
	m_scaledKnobRect.bottom = m_scaledKnobRect.top + m_scaledKnobHeight;
	m_knobSprite.setDestRect(m_scaledKnobRect);
	
	//Adjust the size of the fill sprite if we are filling the slider
	if (m_fill)
	{
		m_fillSprite.setDestRect(XRect(m_scaledScreenRect.left,m_scaledKnobRect.bottom/*m_scaledScreenRect.top*/,
										m_scaledScreenRect.right/*m_scaledKnobRect.left*/,m_scaledScreenRect.bottom));
	}

	//Update the sprites.
	m_knobSprite.update(dt);
	m_trackSprite.update(dt);
	if (m_fill)
		m_fillSprite.update(dt);

	return X_OK;
}



//*****************************************************************************
/**
Renders the slider. This function is called automatically by the engine once per 
frame.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIVerticalSlider::render()
{
	if (!m_visible)
		return X_OK;

	m_trackSprite.render();
	m_knobSprite.render();
	if (m_fill)
		m_fillSprite.render();

	return X_OK;
}



//*****************************************************************************
/**
Returns the percentage of the slider.  For example, if the slider knob is set
completely to the left, the percentage is 0.  Similarly, if the slider knob is
set completely to the right, the percentage returned is 1.

@return The slider knob's position as a percentage of the slider's range.
*/
//*****************************************************************************
XF32 XUIVerticalSlider::getPercentage()
{
	return m_percentage;
}



//*****************************************************************************
/**
Sets the percentage of the user interface slider.

@param percent	The slider's knob position as a percentage of the slider 
				range. If the value is .5, the slider knob will be set to the
				very middle of the slider.  If the value is 1, the slider knob
				will be set to the far right of the slider's range.
*/
//*****************************************************************************
XStatus XUIVerticalSlider::setPercentage(XF32 percent)
{
	if ((percent < 0) || (percent > 1.0))
		return X_ERR_INVALID_PARAMS;
	else
	{
		m_percentage = percent;
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIVerticalSlider::isSliding()
{
	return (m_sliderState == X_UISS_SLIDING);
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalSlider::setOpacity(XF32 opacity)
{
	m_knobSprite.setOpacity(opacity);
	m_trackSprite.setOpacity(opacity);
	m_fillSprite.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalSlider::move(XVector2D& moveVector)
{
	XRect r;
	m_knobSprite.getDestRect(r);
	r.left += (XU32)moveVector.x;
	r.right += (XU32)moveVector.x;
	r.top += (XU32)moveVector.y;
	r.bottom += (XU32)moveVector.y;
	m_knobSprite.setDestRect(r);

	m_trackSprite.getDestRect(r);
	r.left += (XU32)moveVector.x;
	r.right += (XU32)moveVector.x;
	r.top += (XU32)moveVector.y;
	r.bottom += (XU32)moveVector.y;
	m_trackSprite.setDestRect(r);

	m_fillSprite.getDestRect(r);
	r.left += (XU32)moveVector.x;
	r.right += (XU32)moveVector.x;
	r.top += (XU32)moveVector.y;
	r.bottom += (XU32)moveVector.y;
	m_fillSprite.setDestRect(r);

	m_nativeScreenRect.left += (XU32)moveVector.x;
	m_nativeScreenRect.right += (XU32)moveVector.x;
	m_nativeScreenRect.top += (XU32)moveVector.y;
	m_nativeScreenRect.bottom += (XU32)moveVector.y;

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());


	return X_OK;
}



XStatus XUIVerticalSlider::onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{

		
		m_percentage = (XF32)(m_scaledScreenRect.bottom - (hotSpot.y + (m_scaledKnobHeight/2)))/(XF32)(m_scaledScreenRect.bottom - m_scaledScreenRect.top - m_scaledKnobHeight);			

		//Cap the percentage because of the way the knob is not allowed
		//to extend past the slider graphic.
		if (m_percentage > 1.0f)
			m_percentage = 1.0f;
		if (m_percentage < 0.0f)
			m_percentage = 0.0f;
		m_scaledKnobRect.top = (int)((m_scaledScreenRect.bottom - m_scaledScreenRect.top - m_scaledKnobHeight) * m_percentage) + m_scaledScreenRect.top;
		//m_scaledKnobHeight = m_nativeKnobHeight;
		//m_scaledKnobRect.right = m_scaledKnobRect.left + m_scaledKnobHeight;
		

		m_sliderState = X_UISS_SLIDING;

		onBeginSlide();

		XUIElement::onMouseDown(hotSpot,button,pChildElement);
	return X_OK;
}

XStatus XUIVerticalSlider::onMouseUp(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	if (m_sliderState == X_UISS_SLIDING)
	{
		m_sliderState = X_UISS_NOT_SLIDING;
		onEndSlide(false);
	}

	XUIElement::onMouseUp(hotSpot,button,pChildElement);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalSlider::onResize(XF32 xScale,XF32 yScale)
{
	//Adjust the size of the sprites if necessary.
	m_scaledScreenRect.left = (int)(m_nativeScreenRect.left * xScale);
	m_scaledScreenRect.top = (int)(m_nativeScreenRect.top * yScale);
	m_scaledScreenRect.right = (int)(m_nativeScreenRect.right * xScale);
	m_scaledScreenRect.bottom = (int)(m_nativeScreenRect.bottom * yScale);
	m_trackSprite.setDestRect(m_scaledScreenRect);

	m_scaledKnobHeight = (int)(m_nativeKnobHeight * yScale);
	
	return X_OK;
}



XStatus XUIVerticalSlider::onBeginSlide()
{
	if (m_inSoundHandle != X_INVALID_HANDLE)
			XVP::AudioMgr.playSound(m_inSoundHandle);
	return X_OK;
}

XStatus XUIVerticalSlider::onEndSlide(XBOOL forced)
{
	if ((m_outSoundHandle != X_INVALID_HANDLE) && (!forced))
			XVP::AudioMgr.playSound(m_outSoundHandle);
	return X_OK;
}


