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
//	Module Name:	XCursor.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-1-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the cursor.
*/
//*****************************************************************************
XCursor::XCursor()
{
	m_speed = 1;
	m_acceleration = .25f;
	m_relXMov = 0;
	m_relYMov = 0;
	m_state = X_PS_NORMAL;
	m_locked = false;
	m_enabled = true;
}



//*****************************************************************************
/**
Destructor for the cursor.
*/
//*****************************************************************************
XCursor::~XCursor()
{
}



//*****************************************************************************
/**
Initializes the cursor object with settings such as an image, animation properties,
hotspot and speed.  This function must be called for the cursor to function
properly.

@param pTextureFilename	Path to the image to be used for the cursor.
@param desc	Describes the animation properties of the cursor.
@param hotSpot	Pixel location relative to the upper-left corner of the source
				image that will be used for screen space picking.
@param speed	The speed factor to multiply by the raw device input.  Use this
				parameter to magnify or dampen the device input data effectively
				controlling the speed at which the cursor moves around the screen.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XCursor::init(XS8* pTemplateFile)
{
	//Make sure the parameters are valid.
	if (!pTemplateFile)
		return X_ERR_INVALID_PARAMS;

	TiXmlDocument file(pTemplateFile);
	if (!file.LoadFile())
		return X_ERR_OPERATION_FAILED;
	else
	{
		TiXmlNode* pNode = NULL;
		TiXmlElement* pPointerElement = NULL;
		TiXmlNode* pChildNode = NULL;
		CursorStateInfo* pState;

		pNode = file.FirstChild( "pointer" );
		if (pNode)
		{
			//pPointerElement = pNode->ToElement();
			while(pChildNode = pNode->IterateChildren(pChildNode))
			{
				TiXmlElement* pElement = pChildNode->ToElement();
				XS8* pName = (XS8*)pElement->Value();
				if (strcmp("normal",pName) == 0)
					pState = &m_states[X_PS_NORMAL];
				else if (strcmp("help",pName) == 0)
					pState = &m_states[X_PS_HELP];
				else if (strcmp("busy",pName) == 0)
					pState = &m_states[X_PS_BUSY];
				else if (strcmp("workinginbackground",pName) == 0)
					pState = &m_states[X_PS_WORKING_IN_BACKGROUND];
				else if (strcmp("precisionselect",pName) == 0)
					pState = &m_states[X_PS_PRECISION_SELECT];
				else if (strcmp("textselect",pName) == 0)
					pState = &m_states[X_PS_TEXT_SELECT];
				else if (strcmp("unavailable",pName) == 0)
					pState = &m_states[X_PS_UNAVAILABLE];
				else if (strcmp("verticalresize",pName) == 0)
					pState = &m_states[X_PS_VERTICAL_RESIZE];
				else if (strcmp("horizontalresize",pName) == 0)
					pState = &m_states[X_PS_HORIZONTAL_RESIZE];
				else if (strcmp("diagonalresize1",pName) == 0)
					pState = &m_states[X_PS_DIAGONAL_RESIZE_1];
				else if (strcmp("diagonalresize2",pName) == 0)
					pState = &m_states[X_PS_DIAGONAL_RESIZE_2];
				else if (strcmp("move",pName) == 0)
					pState = &m_states[X_PS_MOVE];
				else if (strcmp("custom1",pName) == 0)
					pState = &m_states[X_PS_CUSTOM_1];
				else if (strcmp("custom2",pName) == 0)
					pState = &m_states[X_PS_CUSTOM_2];
				else if (strcmp("custom3",pName) == 0)
					pState = &m_states[X_PS_CUSTOM_3];
				else if (strcmp("custom4",pName) == 0)
					pState = &m_states[X_PS_CUSTOM_4];
				else if (strcmp("custom5",pName) == 0)
					pState = &m_states[X_PS_CUSTOM_5];
				else if (strcmp("custom6",pName) == 0)
					pState = &m_states[X_PS_CUSTOM_6];
				else if (strcmp("custom7",pName) == 0)
					pState = &m_states[X_PS_CUSTOM_7];
				else if (strcmp("custom8",pName) == 0)
					pState = &m_states[X_PS_CUSTOM_8];
				else
					return X_ERR_OPERATION_FAILED;

				XS32 intValue = 0;
				XF64 doubleValue = 0;
				XS8* pFilename;

				pElement->QueryIntAttribute("BeginRow",&intValue);
				pState->desc.beginRow = intValue;
				pElement->QueryIntAttribute("EndRow",&intValue);
				pState->desc.endRow = intValue;
				pElement->QueryIntAttribute("EndCol",&intValue);
				pState->desc.endCol = intValue;
				pElement->QueryIntAttribute("FrameWidth",&intValue);
				pState->desc.frameWidth = intValue;
				pElement->QueryIntAttribute("FrameHeight",&intValue);
				pState->desc.frameHeight = intValue;
				pElement->QueryIntAttribute("Visible",&intValue);
				pState->desc.visible = (XBOOL)intValue;
				pElement->QueryDoubleAttribute("DelayFrameTime",&doubleValue);
				pState->desc.delayFrameTime = (float)doubleValue;
				pElement->QueryDoubleAttribute("Speed",&doubleValue);
				pState->speed = doubleValue;
				pElement->QueryDoubleAttribute("Acceleration",&doubleValue);
				pState->acceleration = doubleValue;
				pElement->QueryIntAttribute("HotSpotX",&intValue);
				pState->hotSpot.x = (float)intValue;
				pElement->QueryIntAttribute("HotSpotY",&intValue);
				pState->hotSpot.y = (float)intValue;
  
				pFilename = (XS8*)pElement->Attribute("TextureFile");
				if (pFilename)
				{
					if (XVP::GraphicsMgr.createTextureFromFile(pFilename,pState->textureHandle) != X_OK)
						return X_ERR_OPERATION_FAILED;
				}
				else
					return X_ERR_OPERATION_FAILED;

				pState->enabled = true;
			}
		}
		else
			return X_ERR_OPERATION_FAILED;
	}
	

	//Initialize cursor to the normal state
	XAnimSprite::setTextureHandle(m_states[X_PS_NORMAL].textureHandle);
	XAnimSprite::init(m_states[X_PS_NORMAL].desc);
	m_speed = m_states[X_PS_NORMAL].speed;
	m_acceleration = m_states[X_PS_NORMAL].acceleration;
	m_hotSpot = m_states[X_PS_NORMAL].hotSpot;
	play(-1);

	return X_OK;
}



//*****************************************************************************
/**
Destroys a cursor by freeing all of it allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XCursor::destroy()
{
	//Normally we'd call anim sprite's destroy here, but since we decided to 
	//manage the textures manually we need to take more caution.  Since the
	//sprite will destroy it's current texture when it gets destroyed, and we
	//don't want it to, we'll set the texture to be invalid.
	//XAnimSprite::setTextureHandle(X_INVALID_HANDLE);

	//Now we can clean up all of the textures we've created and be sure that
	//we're aren't destroying the same texture multiple times.
	for (XU32 i = 0;i < X_MAX_POINTER_STATES;++i)
	{
		if ((m_states[i].enabled) && (m_states[i].textureHandle != X_INVALID_HANDLE))
		{
			XVP::GraphicsMgr.destroyTexture(m_states[i].textureHandle);
			m_states[i].textureHandle = X_INVALID_HANDLE;
		}
	}

	XAnimSprite::destroy();

	return X_OK;
}



//*****************************************************************************
/**
Called once per frame to update the XCursor object's time dependent data.
Normally, this is handled by X-VP once the XCursor has
been added to the scene, so you don't need to call it explicitly.

@param	dt	The time in seconds that has passed since the last frame
					update.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XCursor::update(XF32 dt)
{	
	//Update the base sprite.
	XAnimSprite::update(dt);
	
	//Get the mouse movement delta.
	int xDelta = XVP::InputMgr.getMouseRelXMovement(); 
	int yDelta = XVP::InputMgr.getMouseRelYMovement();

	//Get the screen size each time in case of resolution change.
	
#pragma message("XCursor isn't going to work without these 2 lines fixed")
	XU32 screenWidth = XVP::GraphicsMgr.getRenderContextWidth(XVP::GraphicsMgr.getRenderContext());
	XU32 screenHeight = XVP::GraphicsMgr.getRenderContextHeight(XVP::GraphicsMgr.getRenderContext());

	//Store the relative x movement since the last update.
	m_relXMov = (int)(xDelta * m_speed);

	//Build in some acceleration if the delta is large
	if ((m_relXMov > 1) || (m_relXMov < -1))
		m_relXMov += (XS32)(m_relXMov * m_acceleration);

	//Let's deal with updating the x coordinate.
	m_destRect.left += m_relXMov;

	//Now check to see if we have overstepped the screens boundaries.
	//If we have, then correct it.
	if ((m_destRect.left + m_hotSpot.x) < 0)
		m_destRect.left = (XS32)-m_hotSpot.x;
	if ((m_destRect.left + m_hotSpot.x) > screenWidth)
		m_destRect.left = (XS32)(screenWidth - m_hotSpot.x);
	
	//Store the relative y movement since the last update.
	m_relYMov = (int)(yDelta * m_speed);

	//Build in some acceleration if the delta is large
	if ((m_relYMov > 1) || (m_relYMov < -1))
		m_relYMov += (XS32)(m_relYMov * m_acceleration);

	//Now update the y coordinate.
	m_destRect.top += m_relYMov;
		
	//Now check to see if we have overstepped the screens boundaries.
	//If we have, then correct it.
	if ((m_destRect.top + m_hotSpot.y) < 0)
		m_destRect.top = (XS32)-m_hotSpot.y;
	if ((m_destRect.top + m_hotSpot.y)  > screenHeight)
		m_destRect.top = (XS32)(screenHeight - m_hotSpot.y);
	
	//Update the right and bottom of the destination rect.
	m_destRect.right = m_destRect.left + m_frameWidth;
	m_destRect.bottom = m_destRect.top + m_frameHeight;

	//Update the final position
	m_position.x = (float)m_destRect.left;
	m_position.y = (float)m_destRect.top;

	//Everything went ok.
	return X_OK;
}



//*****************************************************************************
/**
Returns the location of the cursor's hotspot in screen space.

@return The screen location in pixels of the cursor's hotspot.
*/
//*****************************************************************************
XVector2D XCursor::getHotSpot()
{
	return XVector2D((XF32)m_hotSpot.x + m_destRect.left,
					  (XF32)m_hotSpot.y + m_destRect.top);
}



//*****************************************************************************
/**
Gets the relative movement of the cursor in the X direction since the last
update.

@return The relative X movement.
*/
//*****************************************************************************
int XCursor::getRelXMov()
{
	return m_relXMov;
}



//*****************************************************************************
/**
Gets the relative movement of the cursor in the Y direction since the last
update.

@return The relative Y movement.
*/
//*****************************************************************************
int XCursor::getRelYMov()
{
	return m_relYMov;
}



//*****************************************************************************
//*****************************************************************************
XStatus XCursor::setSpeed(XF32 speed)
{
	m_speed = speed;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XCursor::getSpeed()
{
	return m_speed;
}



//*****************************************************************************
//*****************************************************************************
XStatus XCursor::setAcceleration(XF32 acceleration)
{
	m_acceleration = acceleration;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XCursor::getAcceleration()
{
	return m_acceleration;
}

	
	
//*****************************************************************************
//*****************************************************************************
XStatus XCursor::setState(X_POINTER_STATE state)
{
	if (state >= X_MAX_POINTER_STATES)
		return X_ERR_INVALID_PARAMS;

	if ((m_state == state) || (m_locked))
		return X_OK;
	
	//If the requested state is not available, then default
	//back to the normal state.
	X_POINTER_STATE newState = X_PS_NORMAL;
	if (m_states[state].enabled)
		newState = state;
	
	XVector2D pos = getPosition();
	setTextureHandle(m_states[newState].textureHandle);
	XAnimSprite::init(m_states[newState].desc);
	setPosition(pos + m_states[m_state].hotSpot - m_states[newState].hotSpot);
	m_speed = m_states[newState].speed;
	m_hotSpot = m_states[newState].hotSpot;
	m_state = newState;
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_POINTER_STATE XCursor::getState()
{
	return m_state;
}



//*****************************************************************************
//*****************************************************************************
XStatus XCursor::lockState(X_POINTER_STATE state)
{
	//First, set the state then lock it
	setState(state);
	m_locked = true;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XCursor::unlockState()
{
	m_locked = false;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XCursor::isLocked()
{
	return m_locked;
}


