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
//	Module Name:	XUIFrame.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-10-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the user interface sprite.
*/
//*****************************************************************************
XUIFrame::XUIFrame()
{
	m_dragging = false;
}



//*****************************************************************************
/**
Destructor for the user interface sprite.
*/
//*****************************************************************************
XUIFrame::~XUIFrame()
{
}



//*****************************************************************************
/**
Initializes the user interface sprite.

@param pParentUI	The user interface to which this sprite belongs.
@param ID	The element ID used to identify this user interface sprite 
			within a user interface.
@param zOrder	The zOrder of the user interface sprite with respect to other
				user interface elements. 
@param screenRect	The rectangular area of the screen in which the sprite 
					will be displayed.
@param pTextureFilename	The texture that contains the sprite image.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIFrame::init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
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
		TiXmlElement* pFrameElement = NULL;
		
		pNode = file.FirstChild( "frame" );
		if (pNode)
		{
			pFrameElement = pNode->ToElement();
			if (pFrameElement)
			{	
				XRect r;
				XS8* pFilename = NULL;
				pFrameElement->QueryIntAttribute("SrcLeft",&r.left);
				pFrameElement->QueryIntAttribute("SrcTop",&r.top);
				pFrameElement->QueryIntAttribute("SrcRight",&r.right);
				pFrameElement->QueryIntAttribute("SrcBottom",&r.bottom);

				pFilename = (XS8*)pFrameElement->Attribute("TextureFile");
				if (pFilename)
				{
					if (m_sprite.init(pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_nativeScreenRect = screenRect;
					m_sprite.setSrcRect(r);
				}
				else
					return X_ERR_OPERATION_FAILED;

				pFrameElement->QueryIntAttribute("MoveAreaLeft",&m_nativeMoveArea.left);
				pFrameElement->QueryIntAttribute("MoveAreaTop",&m_nativeMoveArea.top);
				pFrameElement->QueryIntAttribute("MoveAreaRight",&m_nativeMoveArea.right);
				pFrameElement->QueryIntAttribute("MoveAreaBottom",&m_nativeMoveArea.bottom);
			}
		}
		else
			return X_ERR_OPERATION_FAILED;
	
		m_zOrderOffset = zOrder;
		m_pParentUI = pParentUI;
		XUIElement::setOwner(pParentUI,this,zOrder);

		//Finally, add this frame to the list of frames in the UI so that
		//auto Z management will occur 
		m_pParentUI->addFrame(this);
	}

	return X_OK;
}
	


//*****************************************************************************
/**
Destroys the user interface sprite and frees all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIFrame::destroy()
{
	//First, remove this frame from the UI's frame list
	m_pParentUI->removeFrame(this);

	XMemBlockList<XUIElement*>::iterator i(m_elementList.begin());
	while (i.isValid())
	{
		(i.getValue())->setParentFrame(NULL);
		i.getNext();
	}

	m_elementList.clear();
	m_sprite.destroy();
	XUIElement::destroy();
	return X_OK;
}



//*****************************************************************************
/**
Updates the user interface sprite.  This function is called by the
engine once per frame.

@param dt	The amount of time, in seconds, that has elapsed since
					the last update.
@param xScale	The horizontal scale factor relative to the original
				width of the user interface sprite.
@param yScale	The vertical scale factor relative to the original
				height of the user interface sprite.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIFrame::update(XF32 dt)
{
	//Update the sprite.
	m_sprite.update(dt);

	return X_OK;
}



//*****************************************************************************
/**
Renders the user interface sprite.  This function is called
automatically by the engine once per frame.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIFrame::render()
{
	if (!m_visible)
		return X_OK;

	m_sprite.render();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIFrame::setOpacity(XF32 opacity)
{
	m_sprite.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIFrame::onResize(XF32 xScale,XF32 yScale)
{
	//Adjust the size of the sprite if necessary.
	m_scaledScreenRect.left = (XS32)(m_nativeScreenRect.left * xScale);
	m_scaledScreenRect.top = (XS32)(m_nativeScreenRect.top * yScale);
	m_scaledScreenRect.right = (XS32)(m_nativeScreenRect.right * xScale);
	m_scaledScreenRect.bottom = (XS32)(m_nativeScreenRect.bottom * yScale);
	m_sprite.setDestRect(m_scaledScreenRect);

	m_scaledMoveArea.left = (XS32)(m_nativeMoveArea.left * xScale);
	m_scaledMoveArea.top = (XS32)(m_nativeMoveArea.top * yScale);
	m_scaledMoveArea.right = (XS32)(m_nativeMoveArea.right * xScale);
	m_scaledMoveArea.bottom = (XS32)(m_nativeMoveArea.bottom * yScale);

	return X_OK;
}



//*****************************************************************************
/**
Returns the XSprite object associated with this user interface sprite.

@return The XSprite object associated with this user interface sprite.
*/
//*****************************************************************************
XSprite* XUIFrame::getSprite()
{
	return &m_sprite;
}



XStatus XUIFrame::onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	XRect translatedRect;
	translatedRect.left = m_scaledScreenRect.left + m_scaledMoveArea.left;
	translatedRect.top = m_scaledScreenRect.top + m_scaledMoveArea.top;
	translatedRect.right = m_scaledScreenRect.left + m_scaledMoveArea.right;
	translatedRect.bottom = m_scaledScreenRect.top + m_scaledMoveArea.bottom;

	if ((button == X_MB_LEFT) && (translatedRect.isPointInside(hotSpot)))
	{
		m_dragging = true;
		m_pParentUI->enterDragMode(this);
	}

	m_pParentUI->bringFrameToFront(this);

	return X_OK;
}


XStatus XUIFrame::onDrag(XVector2D& drag)
{
	XRect scaledRect;
	getScaledRect(scaledRect);

	XVector2D v = XVector2D((XF32)scaledRect.left + drag.x,(XF32)scaledRect.top + drag.y);
	setScaledPosition(v);

	//Now, run through the element list and move them as well
	XMemBlockList<XUIElement*>::iterator i(m_elementList.begin());
	while (i.isValid())
	{
		(i.getValue())->move(drag);
		i.getNext();
	}

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUIFrame::onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement)
{
	XRect translatedRect;
	translatedRect.left = m_scaledScreenRect.left + m_scaledMoveArea.left;
	translatedRect.top = m_scaledScreenRect.top + m_scaledMoveArea.top;
	translatedRect.right = m_scaledScreenRect.left + m_scaledMoveArea.right;
	translatedRect.bottom = m_scaledScreenRect.top + m_scaledMoveArea.bottom;

	XCursor* pCursor = XUserInterface::getCursor();
	if (pCursor)
	{
		if (translatedRect.isPointInside(hotSpot))
			pCursor->setState(X_PS_MOVE);
		else
			pCursor->setState(m_pointerState);
	}

	return X_OK;
}


	
//*****************************************************************************
//*****************************************************************************
XStatus XUIFrame::addElement(XUIElement* pElement)
{
	if (!pElement)
		return X_ERR_INVALID_PARAMS;

	pElement->setParentFrame(this);
	m_elementList.insertBack(pElement);
	return X_OK;
}


XStatus XUIFrame::setZOrderOffset(XU32 zOrder)
{
	return X_ERR_NOT_SUPPORTED;
}



XStatus XUIFrame::changeZOrder(XU32 zOrder)
{
	XS32 difference = zOrder - m_zOrderOffset;

	XUIElement::setZOrderOffset(zOrder);

	//Now, run through the element list and move them as well
	XMemBlockList<XUIElement*>::iterator i(m_elementList.begin());
	while (i.isValid())
	{
		(i.getValue())->setZOrderOffset((i.getValue())->getZOrderOffset() + difference);
		i.getNext();
	}

	return X_OK;
}
