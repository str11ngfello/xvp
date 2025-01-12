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
//	Module Name:	XCursor.h
//	Author:			Lynn Duke
//	Creation Date:	5-1-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_CURSOR_H_
#define __X_CURSOR_H_




enum X_POINTER_STATE
{
	X_PS_NORMAL,
	X_PS_HELP,
	X_PS_BUSY,
	X_PS_WORKING_IN_BACKGROUND,
	X_PS_PRECISION_SELECT,
	X_PS_TEXT_SELECT,
	X_PS_UNAVAILABLE,
	X_PS_VERTICAL_RESIZE,
	X_PS_HORIZONTAL_RESIZE,
	X_PS_DIAGONAL_RESIZE_1,
	X_PS_DIAGONAL_RESIZE_2,
	X_PS_MOVE,
	X_PS_CUSTOM_1,
	X_PS_CUSTOM_2,
	X_PS_CUSTOM_3,
	X_PS_CUSTOM_4,
	X_PS_CUSTOM_5,
	X_PS_CUSTOM_6,
	X_PS_CUSTOM_7,
	X_PS_CUSTOM_8,
	X_MAX_POINTER_STATES
};

			

//*****************************************************************************
//! Provides cursor support with animated sprite capabilities.
/**
The XCursor object is an extension of the XAnimSprite object.  It provides
additional capabilites that allow the sprite to be moved around the screen
based on input from a device.
*/
//*****************************************************************************
class XCursor : public XAnimSprite
{
private:
	struct CursorStateInfo
	{
		XBOOL enabled;
		XHandle textureHandle;
		XAnimSpriteDesc desc;
		XVector2D hotSpot;
		XF32 speed;
		XF32 acceleration;

		CursorStateInfo()
		{
			enabled = false;
			textureHandle = X_INVALID_HANDLE;
		}
	};

	CursorStateInfo	m_states[X_MAX_POINTER_STATES];
	X_POINTER_STATE m_state;
	XBOOL			m_locked;
	XBOOL			m_enabled;
	XVector2D		m_hotSpot;			//point relative to upper-left of destRect 
	XF32			m_speed;			//Speed of cursor movement.
	XF32			m_acceleration;		//Acceleration of cursor movement.
	int				m_relXMov;			//Relative X movement since last update.
	int				m_relYMov;			//Relative Y movement since last update.

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XCursor();
	~XCursor();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pTemplateFile);
	XStatus destroy();
	XStatus update(XF32 dt);
	XVector2D getHotSpot();
	XS32 getRelXMov();
	XS32 getRelYMov();
	XStatus setSpeed(XF32 speed);
	XF32 getSpeed();
	XStatus setAcceleration(XF32 acceleration);
	XF32 getAcceleration();
	XStatus setState(X_POINTER_STATE state);
	X_POINTER_STATE getState();
	XStatus lockState(X_POINTER_STATE state);
	XStatus unlockState();
	XBOOL isLocked();	
	
};

#endif
