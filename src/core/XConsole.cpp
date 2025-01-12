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
//	Module Name:	XConsole.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-8-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

#ifdef XVP_CONSOLE


XUserInterface		XConsole::m_consoleUI;
vector<string>		XConsole::m_commandHistory;
ULONG				XConsole::m_commandIndex = 0;
XUITextBox			XConsole::m_textBox;
XUISprite			XConsole::m_background;
XBOOL				XConsole::m_active = false;
string				XConsole::m_cmdResponse;
char				XConsole::m_conversionBuffer[256];
XBOOL				XConsole::m_waitingForResponse = false;
XBOOL				XConsole::m_inited = false;



//*****************************************************************************
//*****************************************************************************
XStatus XConsole::init(XHandle charSetHandle)
{
	m_consoleUI.setNativeResolution(1366,768);
	
	XTextBoxDesc tbDesc;
	tbDesc.maxChars = X_CONSOLE_MAX_CHARS;
	tbDesc.screenRect = XRect(0,0,1280,1024);
	tbDesc.charPadding = 1;
	tbDesc.charSetHandle = charSetHandle;
	tbDesc.showCursor = true;
	//m_textBox.init(&m_consoleUI,1,tbDesc,"ui/textCursor.bmp",5,16);


	char oldUIPath[X_MAX_PATH_SIZE];
	char oldTexturePath[X_MAX_PATH_SIZE];
	strcpy(oldUIPath,XOSMgr::getPath(X_PATH_UI_TEMPLATE));
	strcpy(oldTexturePath,XOSMgr::getPath(X_PATH_TEXTURE));
	XOSMgr::setPath(X_PATH_UI_TEMPLATE,"../system/console/");
	XOSMgr::setPath(X_PATH_TEXTURE,"../system/console/");
	if (m_textBox.init(&m_consoleUI,1,XRect(0,0,1280,1024),"textbox.xml") != X_OK)
		return X_ERR_OPERATION_FAILED;
	m_textBox.onGotFocus();
	m_textBox.setText("XVP>");
	m_textBox.setReadOnlyMarker(m_textBox.getTextLength());
	m_textBox.enableAutoScroll(true);

	//Return original path
	XOSMgr::setPath(X_PATH_UI_TEMPLATE,oldUIPath);
	XOSMgr::setPath(X_PATH_TEXTURE,oldTexturePath);


	//m_background.init(&m_consoleUI,0,0,0,XRect(0,0,1280,1024),
	//	"c:/bg.png");

	m_inited = true;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XConsole::destroy()
{
	hide();
	
	m_consoleUI.destroy();
	m_textBox.destroy();
	m_commandHistory.clear();
	m_commandIndex = 0;
	m_active = false;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XConsole::isInitialized()
{
	return m_inited;
}



//*****************************************************************************
//*****************************************************************************
XStatus XConsole::show()
{
	m_active = true;
	XScene* pScene = XRenderContext::getDebugScene();
	if (pScene)
	{
		if (!pScene->isUIInScene(&m_consoleUI))
			pScene->addUIToScene(&m_consoleUI);
	}

	return X_OK;
}
	


//*****************************************************************************
//*****************************************************************************
XStatus XConsole::hide()
{	
	m_active = false;

	XScene* pScene = XRenderContext::getDebugScene();
	if (pScene)
	{
		pScene->removeUIFromScene(&m_consoleUI);
	}
	return X_OK;
}
	


//*****************************************************************************
//*****************************************************************************
XBOOL XConsole::isVisible()
{
	return m_active;
}




//*****************************************************************************
//*****************************************************************************
XStatus XConsole::update(XF32 dt)
{	
	//If the console is active it doesn't get updated.
	if (!m_active)
		return X_OK;

	//If the return key is pressed, send the console buffer to xvp to be
	//executed.
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_RETURN))
	{
		XS8* pCommand = findInsertionPoint();
		if ((pCommand[0] != '\n') && (pCommand[0] != '\0'))
		{
			m_waitingForResponse = true;
			XStatus status = XLua::executeString(pCommand);
			m_waitingForResponse = false;

			//If we received a response, add a newline to the end. Otherwise,
			//if there was a Lua error, use it.
			if (m_cmdResponse.size() != 0)
				m_cmdResponse += "\n\n";
			else if (status != X_OK)
				m_cmdResponse = XLua::getLastErrorString();

			
			//Put the command into the command history.
			string s;
			s = pCommand;
			m_commandHistory.push_back(s);
			m_commandIndex = m_commandHistory.size();
		}
		
		//If we have a response, output it.
		if (m_cmdResponse.size() != 0)
		{
			if ((m_textBox.appendText((XS8*)m_cmdResponse.c_str()) == X_ERR_OPERATION_FAILED) ||
				(m_textBox.appendText("XVP>") == X_ERR_OPERATION_FAILED))
			{
				m_textBox.clear();
				m_textBox.appendText((XS8*)m_cmdResponse.c_str());
				m_textBox.appendText("XVP>");
			}
			m_cmdResponse.clear();
		}
		else
		{
			if (m_textBox.appendText("XVP>") == X_ERR_OPERATION_FAILED)
			{
				m_textBox.clear();
				m_textBox.appendText("XVP>");
			}
		}
		m_textBox.setReadOnlyMarker(m_textBox.getTextLength());
	}

	//It the up cursor key is pressed, cycle through the command history.
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_UP) && (m_commandHistory.size() != 0))
	{
		if (m_commandIndex == 0)
			m_commandIndex = m_commandHistory.size()-1;
		else
			m_commandIndex--;

		XS8* pCommand = findInsertionPoint();
		if (pCommand)
			m_textBox.removeTextFromEnd(strlen(pCommand));
		
		m_textBox.appendText((XS8*)m_commandHistory[m_commandIndex].c_str());
		m_textBox.removeTextFromEnd(1);
	}

	//It the down cursor key is pressed, cycle through the command history.
	if ((XVP::InputMgr.keyDownOnceRepeat(X_KEY_DOWN)&& (m_commandHistory.size() != 0)))
	{
		m_commandIndex++;
		if (m_commandIndex >= m_commandHistory.size())
			m_commandIndex = 0;

		XS8* pCommand = findInsertionPoint();
		if (pCommand)
			m_textBox.removeTextFromEnd(strlen(pCommand));

		m_textBox.appendText((XS8*)m_commandHistory[m_commandIndex].c_str());
		m_textBox.removeTextFromEnd(1);
	}

	return X_OK;
}

	

//*****************************************************************************
//*****************************************************************************
XS8* XConsole::findInsertionPoint()
{
	XS8* pCommand = NULL;
	XS8* pWholeBuffer = m_textBox.getText();
	for (XS32 i = strlen(pWholeBuffer)-1;i >= 0 ;i--)
	{
		if (pWholeBuffer[i] == 'X')
		{
			if ((pWholeBuffer[i+1] != '\0') && (pWholeBuffer[i+2] != '\0') &&
				 pWholeBuffer[i+3] != '\0')
			{
				if ((pWholeBuffer[i+1] == 'V') && (pWholeBuffer[i+2] == 'P') &&
					pWholeBuffer[i+3] == '>')
				{
					pCommand = &pWholeBuffer[i+4];
					break;
				}
			}
		}
	}

	return pCommand;
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponseXStatus(XStatus response)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	switch (response)
	{
	case X_OK:	
		m_cmdResponse += "X_OK";
		break;
	case X_ERR_MEMORY_EXHAUSTED:	
		m_cmdResponse += "X_ERR_MEMORY_EXHAUSTED";
		break;
	case X_ERR_INVALID_PARAMS:	
		m_cmdResponse += "X_ERR_INVALID_PARAMS";
		break;
	case X_ERR_NOT_SUPPORTED:	
		m_cmdResponse += "X_ERR_NOT_SUPPORTED";
		break;
	case X_ERR_REFERENCES_EXIST:	
		m_cmdResponse += "X_ERR_REFERENCES_EXIST";
		break;
	case X_ERR_NO_SUCH_OBJECT:	
		m_cmdResponse += "X_ERR_NO_SUCH_OBJECT";
		break;
	case X_ERR_OPERATION_FAILED:	
		m_cmdResponse += "X_ERR_OPERATION_FAILED";
		break;
	case X_ERR_LIMIT_REACHED:	
		m_cmdResponse += "X_ERR_LIMIT_REACHED";
		break;
	case X_ERR_NOT_RUNNING:	
		m_cmdResponse += "X_ERR_NOT_RUNNING";
		break;
	case X_ERR_NOT_READY:	
		m_cmdResponse += "X_ERR_NOT_READY";
		break;
	case X_ERR_NOT_INITIALIZED:	
		m_cmdResponse += "X_ERR_NOT_INITIALIZED";
		break;
	case X_ERR_PARTIAL_FAILURE:	
		m_cmdResponse += "X_ERR_PARTIAL_FAILURE";
		break;
	case X_ERR_REDUNDANT_OPERATION:	
		m_cmdResponse += "X_ERR_REDUNDANT_OPERATION";
		break;
	case X_ERR_WRONG_NUMBER_OF_PARAMS:	
		m_cmdResponse += "X_ERR_WRONG_NUMBER_OF_PARAMS";
		break;
	default:
		m_cmdResponse += "Response not recognized.";
		break;
	}
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponseBool(XBOOL response)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	if (response)
		m_cmdResponse += "True";
	else
		m_cmdResponse += "False";
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponseFloat(XS8* tag,XF32 f)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	if (tag)
		m_cmdResponse += tag;

	sprintf(m_conversionBuffer,"%f",f);
	m_cmdResponse += m_conversionBuffer;
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponseUInt32(XS8* tag,XU32 u)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	if (tag)
		m_cmdResponse += tag;

	sprintf(m_conversionBuffer,"%u",u);
	m_cmdResponse += m_conversionBuffer;
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponseInt32(XS8* tag,XS32 i)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	if (tag)
		m_cmdResponse += tag;

	sprintf(m_conversionBuffer,"%i",i);
	m_cmdResponse += m_conversionBuffer;
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponse2DVec(XS8* tag,XVector2D& vec)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	if (tag)
		m_cmdResponse += tag;

	m_cmdResponse += "x=";
	sprintf(m_conversionBuffer,"%f",vec.x);
	m_cmdResponse += m_conversionBuffer;
	m_cmdResponse += " y=";
	sprintf(m_conversionBuffer,"%f",vec.y);
	m_cmdResponse += m_conversionBuffer;
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponse3DVec(XS8* tag,XVector3D& vec)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	if (tag)
		m_cmdResponse += tag;

	m_cmdResponse += "x=";
	sprintf(m_conversionBuffer,"%f",vec.x);
	m_cmdResponse += m_conversionBuffer;
	m_cmdResponse += " y=";
	sprintf(m_conversionBuffer,"%f",vec.y);
	m_cmdResponse += m_conversionBuffer;
	m_cmdResponse += " z=";
	sprintf(m_conversionBuffer,"%f",vec.z);
	m_cmdResponse += m_conversionBuffer;
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponse4DVec(XS8* tag,XVector4D& vec)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	if (tag)
		m_cmdResponse += tag;

	m_cmdResponse += "x=";
	sprintf(m_conversionBuffer,"%f",vec.x);
	m_cmdResponse += m_conversionBuffer;
	m_cmdResponse += " y=";
	sprintf(m_conversionBuffer,"%f",vec.y);
	m_cmdResponse += m_conversionBuffer;
	m_cmdResponse += " z=";
	sprintf(m_conversionBuffer,"%f",vec.z);
	m_cmdResponse += m_conversionBuffer;
	m_cmdResponse += " w=";
	sprintf(m_conversionBuffer,"%f",vec.w);
	m_cmdResponse += m_conversionBuffer;
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponseString(XS8* tag,XS8* string)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	if (tag)
		m_cmdResponse += tag;

	if (string)
		m_cmdResponse += string;
	else
		m_cmdResponse += "Empty String";
}



//*****************************************************************************
//*****************************************************************************
void XConsole::setLastCommandResponseColor(XS8* tag,XColor& color)
{
	//Are we waiting for a response?
	if (!m_waitingForResponse)
		return;

	//If we still have an earlier response waiting to be output, tack this new
	//one on to the end.
	if (m_cmdResponse.size() != 0)
		m_cmdResponse += '\n';

	if (tag)
		m_cmdResponse += tag;

	m_cmdResponse += "r=";
	sprintf(m_conversionBuffer,"%f",color.m_R);
	m_cmdResponse += m_conversionBuffer;
	m_cmdResponse += " g=";
	sprintf(m_conversionBuffer,"%f",color.m_G);
	m_cmdResponse += m_conversionBuffer;
	m_cmdResponse += " b=";
	sprintf(m_conversionBuffer,"%f",color.m_B);
	m_cmdResponse += m_conversionBuffer;
	m_cmdResponse += " a=";
	sprintf(m_conversionBuffer,"%f",color.m_A);
	m_cmdResponse += m_conversionBuffer;
}


#endif