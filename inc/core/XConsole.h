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
//	Module Name:	XConsole.h
//	Author:			Lynn Duke
//	Creation Date:	5-8-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_CONSOLE_H__
#define __X_CONSOLE_H__





//*****************************************************************************
//*****************************************************************************
enum XConsoleReturnCode
{
	X_CRC_OK,
	X_CRC_FAILED,
	X_CRC_NO_EFFECT,
	X_CRC_NO_SUCH_COMMAND,
	X_CRC_WRONG_NUM_PARAMS,
	X_CRC_INVALID_PARAMS,
	X_CRC_NOT_SUPPORTED
};


//*****************************************************************************
//*****************************************************************************
class XConsole
{
private:
	static XUserInterface		m_consoleUI;
	static vector<string>		m_commandHistory;
	static ULONG				m_commandIndex;
	static XUITextBox			m_textBox;
	static XUISprite			m_background;
	static XBOOL					m_active;
	static string				m_cmdResponse;
	static char					m_conversionBuffer[256];
	static XBOOL					m_waitingForResponse;
	static XBOOL				m_inited;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	static XS8* findInsertionPoint();

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XConsole();
	~XConsole();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	static XStatus init(XHandle charSetHandle);
	static XStatus update(XF32 dt);
	static XStatus destroy();
	static XStatus show();
	static XStatus hide();
	static XBOOL isVisible();
	static XBOOL isInitialized();
	static void setLastCommandResponseXStatus(XStatus response);
	static void setLastCommandResponseBool(XBOOL response);
	static void setLastCommandResponseFloat(XS8* tag,XF32 f);
	static void setLastCommandResponseUInt32(XS8* tag,XU32 u);
	static void setLastCommandResponseInt32(XS8* tag,XS32 i);
	static void setLastCommandResponse2DVec(XS8* tag,XVector2D& vec);
	static void setLastCommandResponse3DVec(XS8* tag,XVector3D& vec);
	static void setLastCommandResponse4DVec(XS8* tag,XVector4D& vec);
	static void setLastCommandResponseString(XS8* tag,XS8* string);
	static void setLastCommandResponseColor(XS8* tag,XColor& color);
};


#endif