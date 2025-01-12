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
//	Module Name:	XUserInterface.h
//	Author:			Lynn Duke
//	Creation Date:	6-12-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_USER_INTERFACE_H__
#define __X_USER_INTERFACE_H__



#define X_UI_TOOL_TIP_Z_ORDER		1000000



//*****************************************************************************
//*****************************************************************************
enum X_UI_EVENT_TYPE 
{
	X_UIE_INVALID,	
	X_UIE_GLOBAL_MOUSE_UP,		
	X_UIE_GLOBAL_MOUSE_DOWN,		
	X_UIE_MOUSE_UP,		
	X_UIE_MOUSE_DOWN,		
	X_UIE_MOUSE_OVER,
	X_UIE_MOUSE_NOT_OVER,		
	X_UIE_KEY_DOWN,
	X_UIE_KEY_UP
};


enum X_UI_LUA_EVENT
{
	X_UILE_ADDED_TO_SCENE,
	X_UILE_REM_FROM_SCENE,
	X_UILE_CHANGE,
	X_UILE_MOUSE_OVER,
	X_UILE_MOUSE_ENTER,
	X_UILE_MOUSE_LEAVE
};



//*****************************************************************************
//! Provides a user interface framework with many types of controls.
/**
The XUserInterface object provides a framework to create and interact with
interface components such as buttons, sliders, text boxes, etc.  Much the same
way as Microsoft Windows provides controls for the user to interact with, the
XUserInterface object serves as a container for interactive controls.

Interactive controls (elements) include things like:

1. Buttons <br>
2. Sliders <br>
3. Text Boxes <br>
4. Sliders <br>
5. Progress Bars <br>
6. Sprites <br>
7. Movies <br>
8. Toggle Switches <br>
9. List Views  <br>
10. Combo Boxes <br>
11. Spinners <br>
12. Scroll Bars <br>
13. Etc... <br>

New types of controls can be added by deriving from the XUIElement object.  Also,
the XUserInterface object automatically handles screen resolution changes
by resizing controls appropriately.  What this means is that, once you
design a user interface layout for your target resolution, say 1024x768, you
never design the layout for other resolutions.  The XUserInterface deals with
the details.

Controls that are compatible with the XUserInterface object also respond to
events.  Events include things such as mouse up, mouse down, etc.  

See the member functions below for more information.
*/
//*****************************************************************************
class XUserInterface
{
protected:

	enum X_CMD_TYPE
	{
		X_CMDT_ADD,
		X_CMDT_REMOVE,
	};

	struct XElementCmd
	{
		X_CMD_TYPE		cmdType;
		XUIElement*		pElement;
		XS32			zOrder;
	};	

	XU32					m_nativeXRes;
	XU32					m_nativeYRes;
	XF32					m_xScale;
	XF32					m_yScale;
	XMemBlockList<XUIElement*>		m_elementList;
	XMemBlockList<XUIFrame*>			m_frameList;
	XBOOL					m_scale;
	XUIElement*				m_pLastElementEntered;
	XUIElement*				m_pElementWithFocus;
	XUIElement*				m_pDragElement;
	XTimer					m_toolTipTimer;
	XF32					m_toolTipTimeout;
	XUITextBox*				m_pToolTipBox;
	XBOOL					m_toolTipsEnabled;
	static XCursor			m_cursor;
	static XBOOL			m_cursorLoaded;
	static XBOOL			m_cursorEnabled;
	static XBOOL			m_cursorVisible;
	XVector2D				m_lastCursorHotSpot;
	XS8*					m_pCreationHandler;
	XS8*					m_pDestructionHandler;
	XS8*					m_pEventHandler;
	XS8*					m_pUpdateHandler;
	XMemBlockList<XElementCmd>		m_elementCmdQueue;
	XBOOL					m_queueElementCmds;


private:
	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XStatus addElement(XUIElement* pElement,XU32 zOrder);
	XStatus removeElement(XUIElement* pElement);
	void updateScalingRatios();
	XStatus addFrame(XUIFrame* pFrame);
	XStatus removeFrame(XUIFrame* pFrame);
	XStatus bringFrameToFront(XUIFrame* pFrame);
	XStatus processElementCmds();


public:
	static XUserInterface*	m_currentWorkingUI;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUserInterface();
	~XUserInterface();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus load(XS8* pFilename);
	XStatus destroy();
	XStatus render();
	XStatus update(XF32 dt);	
	XStatus setNativeResolution(XU32 nativeXRes,XU32 nativeYRes);
	XStatus notify(X_UI_LUA_EVENT event);
	XU32 getNumElements();
	XStatus enableScaling(XBOOL enable);
	XBOOL isScalingEnabled();
	XStatus enableToolTips(XS8* pTextBoxTemplateFile);
	XStatus disableToolTips();
	XBOOL isToolTipsEnabled();
	XF32 getXScale();
	XF32 getYScale();
	XStatus setToolTipTimeout(XF32 seconds);
	XF32 getToolTipTimeout();
	XStatus setOpacity(XF32 opacity);
	XStatus sendLuaEvent(XUIElement* pElement,X_UI_LUA_EVENT event);
	XStatus enterDragMode(XUIElement* pElement);
	//XStatus sendScriptEvent(XUIElement* pElement,X_UIS_EVENT event,
	XStatus setCreationHandler(XS8* pHandlerName);
	XStatus setDestructionHandler(XS8* pHandlerName);
	XStatus setEventHandler(XS8* pHandlerName);
	XStatus setUpdateHandler(XS8* pHandlerName);
	static XCursor* getCursor();
	static XStatus createCursor(XS8* pTemplateFile);
	static XStatus destroyCursor();
	static XStatus updateCursor(XF32 dt);
	static XStatus renderCursor();
	static XStatus showCursor(XBOOL show);
	static XBOOL isCursorVisible();
	static XStatus enableCursor(XBOOL enable);
	static XBOOL isCursorEnabled();
	static XStatus initLuaGlue();
	friend XUIElement;
	friend XUIFrame;
};

#endif