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
//	Module Name:	XUIToggle.h
//	Author:			Lynn Duke
//	Creation Date:	6-21-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_TOGGLE_H__
#define __X_UI_TOGGLE_H__





//*****************************************************************************
//! Toggle switch/button control.
/**
The XUIToggle object is similar to the XUIButton object with one exception.
When the user depresses the toggle, it stays depressed until the user presses
the toggle again.     

See the member functions below for more information.
*/
//*****************************************************************************
class XUIToggle : public XUIButton
{
private:
public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIToggle();
	virtual ~XUIToggle();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus setState(XBOOL isPressed);
	XStatus onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	XStatus onMouseUp(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	XStatus onMouseEnter(XUIElement* pChildElement);
	XStatus onMouseLeave(XUIElement* pChildElement);
};

#endif