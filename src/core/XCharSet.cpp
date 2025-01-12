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
//	Module Name:	XCharSet.cpp
//	Author:			Lynn Duke
//	Creation Date:	9-6-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the character set.
*/
//*****************************************************************************
XCharSet::XCharSet()
{
	m_texHandle = X_INVALID_HANDLE;
	m_lineHeight = 0;
}



//*****************************************************************************
/**
Destructor for the character set.
*/
//*****************************************************************************
XCharSet::~XCharSet()
{
}



//*****************************************************************************
/**
Initializes a character set with the passed in parameters.

@param pTextureFilename	The path to the image that contains the character set.
@param pCharDataFilename	The path to the text file that contains character
							dimension data.  This file must reside in the same
							directory as the texture file.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XCharSet::init(XS8* pTextureFilename,XS8* pCharDataFilename)
{
	//Make sure parameters are valid.
	if (!pTextureFilename)
		return X_ERR_INVALID_PARAMS;

	//Reset data.
	destroy();
	XCharSet();

	//Create the new texture for use by the charset.  Here, we use 
	//X-VP for texture creation to ensure that a texture is loaded
	//only once.
	if (XVP::GraphicsMgr.createTextureFromFile(pTextureFilename,m_texHandle) != X_OK)
		return X_ERR_OPERATION_FAILED;   
	
	//Read in the character width and height data if the user has passed
	//in a data file associated with this charset.
	if (pCharDataFilename)
	{
		//First, find the [text] section.
		char buf[255];
		char key;
		char valueBuf[50];
		XU32 x,y,width,height;
		char fullPath[X_MAX_PATH_SIZE];
		XOSMgr::buildPath(X_PATH_TEXTURE,pCharDataFilename,fullPath);

		ifstream f(fullPath);
		if (!f.good())
			return X_ERR_OPERATION_FAILED;

		do
		{
			f.getline(buf,254,'\n');
			
			//if we reach the end of the file withought finding the [text]
			//section then return and error.
			if (f.eof())
				return X_ERR_OPERATION_FAILED;
		}
		while(strcmp(buf,"[text]") != 0);

		do
		{
			f.getline(buf,254,'\n');
			
			//if we reach the end of the file withought finding the blocksize
			//key then return and error.
			if (f.eof())
				return X_ERR_OPERATION_FAILED;
		}
		while(strcmp(strtok(buf,"="),"blocksize") != 0);

		//Store the height of the characters.
		height = atoi(strtok(NULL,"="));
		m_lineHeight = height;
		
		do
		{
			f.getline(buf,254,'\n');
			
			//if we reach the end of the file withought finding the [charset]
			//section then return and error.
			if (f.eof())
				return X_ERR_OPERATION_FAILED;
		}
		while(strcmp(buf,"[charset]") != 0);

		//At this point we read in all the data until the end of file or until
		//the next section, whatever that may be.
		do
		{
			f.getline(buf,254,'\n');
			
			if (f.eof())
				break;
				
			//Save the character information.  Check for the special '=' case.
			if ((buf[0] == '=') && (buf[1] == '='))
			{
				key = '=';
				x = (atoi(strtok(&buf[2],",")));
				y = (atoi(strtok(NULL,",")));
				width = (atoi(strtok(NULL,",")));
			}
			else
			{
				key = *(strtok(buf,"="));
				strcpy(valueBuf,strtok(NULL,"="));
				x = (atoi(strtok(valueBuf,",")));
				y = (atoi(strtok(NULL,",")));
				width = (atoi(strtok(NULL,",")));
			}
           
			switch(key)
			{
			case 'a':
				m_a.m_xPos = x;
				m_a.m_yPos = y;
				m_a.m_width = width;
				m_a.m_height = height; 
				break;
			case 'b':
				m_b.m_xPos = x;
				m_b.m_yPos = y;
				m_b.m_width = width;
				m_b.m_height = height; 
				break;
			case 'c':
				m_c.m_xPos = x;
				m_c.m_yPos = y;
				m_c.m_width = width;
				m_c.m_height = height; 
				break;
			case 'd':
				m_d.m_xPos = x;
				m_d.m_yPos = y;
				m_d.m_width = width;
				m_d.m_height = height; 
				break;
			case 'e':
				m_e.m_xPos = x;
				m_e.m_yPos = y;
				m_e.m_width = width;
				m_e.m_height = height; 
				break;
			case 'f':
				m_f.m_xPos = x;
				m_f.m_yPos = y;
				m_f.m_width = width;
				m_f.m_height = height; 
				break;
			case 'g':
				m_g.m_xPos = x;
				m_g.m_yPos = y;
				m_g.m_width = width;
				m_g.m_height = height; 
				break;
			case 'h':
				m_h.m_xPos = x;
				m_h.m_yPos = y;
				m_h.m_width = width;
				m_h.m_height = height; 
				break;
			case 'i':
				m_i.m_xPos = x;
				m_i.m_yPos = y;
				m_i.m_width = width;
				m_i.m_height = height; 
				break;
			case 'j':
				m_j.m_xPos = x;
				m_j.m_yPos = y;
				m_j.m_width = width;
				m_j.m_height = height; 
				break;
			case 'k':
				m_k.m_xPos = x;
				m_k.m_yPos = y;
				m_k.m_width = width;
				m_k.m_height = height; 
				break;
			case 'l':
				m_l.m_xPos = x;
				m_l.m_yPos = y;
				m_l.m_width = width;
				m_l.m_height = height; 
				break;
			case 'm':
				m_m.m_xPos = x;
				m_m.m_yPos = y;
				m_m.m_width = width;
				m_m.m_height = height; 
				break;
			case 'n':
				m_n.m_xPos = x;
				m_n.m_yPos = y;
				m_n.m_width = width;
				m_n.m_height = height; 
				break;
			case 'o':
				m_o.m_xPos = x;
				m_o.m_yPos = y;
				m_o.m_width = width;
				m_o.m_height = height; 
				break;
			case 'p':
				m_p.m_xPos = x;
				m_p.m_yPos = y;
				m_p.m_width = width;
				m_p.m_height = height; 
				break;
			case 'q':
				m_q.m_xPos = x;
				m_q.m_yPos = y;
				m_q.m_width = width;
				m_q.m_height = height; 
				break;
			case 'r':
				m_r.m_xPos = x;
				m_r.m_yPos = y;
				m_r.m_width = width;
				m_r.m_height = height; 
				break;
			case 's':
				m_s.m_xPos = x;
				m_s.m_yPos = y;
				m_s.m_width = width;
				m_s.m_height = height; 
				break;
			case 't':
				m_t.m_xPos = x;
				m_t.m_yPos = y;
				m_t.m_width = width;
				m_t.m_height = height; 
				break;
			case 'u':
				m_u.m_xPos = x;
				m_u.m_yPos = y;
				m_u.m_width = width;
				m_u.m_height = height; 
				break;
			case 'v':
				m_v.m_xPos = x;
				m_v.m_yPos = y;
				m_v.m_width = width;
				m_v.m_height = height; 
				break;
			case 'w':
				m_w.m_xPos = x;
				m_w.m_yPos = y;
				m_w.m_width = width;
				m_w.m_height = height; 
				break;
			case 'x':
				m_x.m_xPos = x;
				m_x.m_yPos = y;
				m_x.m_width = width;
				m_x.m_height = height; 
				break;
			case 'y':
				m_y.m_xPos = x;
				m_y.m_yPos = y;
				m_y.m_width = width;
				m_y.m_height = height; 
				break;
			case 'z':
				m_z.m_xPos = x;
				m_z.m_yPos = y;
				m_z.m_width = width;
				m_z.m_height = height; 
				break;
			case 'A':
				m_A.m_xPos = x;
				m_A.m_yPos = y;
				m_A.m_width = width;
				m_A.m_height = height; 
				break;
			case 'B':
				m_B.m_xPos = x;
				m_B.m_yPos = y;
				m_B.m_width = width;
				m_B.m_height = height; 
				break;
			case 'C':
				m_C.m_xPos = x;
				m_C.m_yPos = y;
				m_C.m_width = width;
				m_C.m_height = height; 
				break;
			case 'D':
				m_D.m_xPos = x;
				m_D.m_yPos = y;
				m_D.m_width = width;
				m_D.m_height = height; 
				break;
			case 'E':
				m_E.m_xPos = x;
				m_E.m_yPos = y;
				m_E.m_width = width;
				m_E.m_height = height; 
				break;
			case 'F':
				m_F.m_xPos = x;
				m_F.m_yPos = y;
				m_F.m_width = width;
				m_F.m_height = height; 
				break;
			case 'G':
				m_G.m_xPos = x;
				m_G.m_yPos = y;
				m_G.m_width = width;
				m_G.m_height = height; 
				break;
			case 'H':
				m_H.m_xPos = x;
				m_H.m_yPos = y;
				m_H.m_width = width;
				m_H.m_height = height; 
				break;
			case 'I':
				m_I.m_xPos = x;
				m_I.m_yPos = y;
				m_I.m_width = width;
				m_I.m_height = height; 
				break;
			case 'J':
				m_J.m_xPos = x;
				m_J.m_yPos = y;
				m_J.m_width = width;
				m_J.m_height = height; 
				break;
			case 'K':
				m_K.m_xPos = x;
				m_K.m_yPos = y;
				m_K.m_width = width;
				m_K.m_height = height; 
				break;
			case 'L':
				m_L.m_xPos = x;
				m_L.m_yPos = y;
				m_L.m_width = width;
				m_L.m_height = height; 
				break;
			case 'M':
				m_M.m_xPos = x;
				m_M.m_yPos = y;
				m_M.m_width = width;
				m_M.m_height = height; 
				break;
			case 'N':
				m_N.m_xPos = x;
				m_N.m_yPos = y;
				m_N.m_width = width;
				m_N.m_height = height; 
				break;
			case 'O':
				m_O.m_xPos = x;
				m_O.m_yPos = y;
				m_O.m_width = width;
				m_O.m_height = height; 
				break;
			case 'P':
				m_P.m_xPos = x;
				m_P.m_yPos = y;
				m_P.m_width = width;
				m_P.m_height = height; 
				break;
			case 'Q':
				m_Q.m_xPos = x;
				m_Q.m_yPos = y;
				m_Q.m_width = width;
				m_Q.m_height = height; 
				break;
			case 'R':
				m_R.m_xPos = x;
				m_R.m_yPos = y;
				m_R.m_width = width;
				m_R.m_height = height; 
				break;
			case 'S':
				m_S.m_xPos = x;
				m_S.m_yPos = y;
				m_S.m_width = width;
				m_S.m_height = height; 
				break;
			case 'T':
				m_T.m_xPos = x;
				m_T.m_yPos = y;
				m_T.m_width = width;
				m_T.m_height = height; 
				break;
			case 'U':
				m_U.m_xPos = x;
				m_U.m_yPos = y;
				m_U.m_width = width;
				m_U.m_height = height; 
				break;
			case 'V':
				m_V.m_xPos = x;
				m_V.m_yPos = y;
				m_V.m_width = width;
				m_V.m_height = height; 
				break;
			case 'W':
				m_W.m_xPos = x;
				m_W.m_yPos = y;
				m_W.m_width = width;
				m_W.m_height = height; 
				break;
			case 'X':
				m_X.m_xPos = x;
				m_X.m_yPos = y;
				m_X.m_width = width;
				m_X.m_height = height; 
				break;
			case 'Y':
				m_Y.m_xPos = x;
				m_Y.m_yPos = y;
				m_Y.m_width = width;
				m_Y.m_height = height; 
				break;
			case 'Z':
				m_Z.m_xPos = x;
				m_Z.m_yPos = y;
				m_Z.m_width = width;
				m_Z.m_height = height; 
				break;
			case '0':
				m_num0.m_xPos = x;
				m_num0.m_yPos = y;
				m_num0.m_width = width;
				m_num0.m_height = height; 
				break;
			case '1':
				m_num1.m_xPos = x;
				m_num1.m_yPos = y;
				m_num1.m_width = width;
				m_num1.m_height = height; 
				break;
			case '2':
				m_num2.m_xPos = x;
				m_num2.m_yPos = y;
				m_num2.m_width = width;
				m_num2.m_height = height; 
				break;
			case '3':
				m_num3.m_xPos = x;
				m_num3.m_yPos = y;
				m_num3.m_width = width;
				m_num3.m_height = height; 
				break;
			case '4':
				m_num4.m_xPos = x;
				m_num4.m_yPos = y;
				m_num4.m_width = width;
				m_num4.m_height = height; 
				break;
			case '5':
				m_num5.m_xPos = x;
				m_num5.m_yPos = y;
				m_num5.m_width = width;
				m_num5.m_height = height; 
				break;
			case '6':
				m_num6.m_xPos = x;
				m_num6.m_yPos = y;
				m_num6.m_width = width;
				m_num6.m_height = height; 
				break;
			case '7':
				m_num7.m_xPos = x;
				m_num7.m_yPos = y;
				m_num7.m_width = width;
				m_num7.m_height = height; 
				break;
			case '8':
				m_num8.m_xPos = x;
				m_num8.m_yPos = y;
				m_num8.m_width = width;
				m_num8.m_height = height; 
				break;
			case '9':
				m_num9.m_xPos = x;
				m_num9.m_yPos = y;
				m_num9.m_width = width;
				m_num9.m_height = height; 
				break;
			case '.':
				m_period.m_xPos = x;
				m_period.m_yPos = y;
				m_period.m_width = width;
				m_period.m_height = height; 
				break;
			case ',':
				m_comma.m_xPos = x;
				m_comma.m_yPos = y;
				m_comma.m_width = width;
				m_comma.m_height = height; 
				break;
			case '?':
				m_questionMark.m_xPos = x;
				m_questionMark.m_yPos = y;
				m_questionMark.m_width = width;
				m_questionMark.m_height = height; 
				break;
			case ':':
				m_colon.m_xPos = x;
				m_colon.m_yPos = y;
				m_colon.m_width = width;
				m_colon.m_height = height; 
				break;
			case ';':
				m_semicolon.m_xPos = x;
				m_semicolon.m_yPos = y;
				m_semicolon.m_width = width;
				m_semicolon.m_height = height; 
				break;
			case '(':
				m_openParen.m_xPos = x;
				m_openParen.m_yPos = y;
				m_openParen.m_width = width;
				m_openParen.m_height = height; 
				break;
			case ')':
				m_closeParen.m_xPos = x;
				m_closeParen.m_yPos = y;
				m_closeParen.m_width = width;
				m_closeParen.m_height = height; 
				break;
			case '!':
				m_exclamation.m_xPos = x;
				m_exclamation.m_yPos = y;
				m_exclamation.m_width = width;
				m_exclamation.m_height = height; 
				break;
			case '/':
				m_forwardSlash.m_xPos = x;
				m_forwardSlash.m_yPos = y;
				m_forwardSlash.m_width = width;
				m_forwardSlash.m_height = height; 
				break;
			case '\\':
				m_backSlash.m_xPos = x;
				m_backSlash.m_yPos = y;
				m_backSlash.m_width = width;
				m_backSlash.m_height = height; 
				break;
			case '=':
				m_equal.m_xPos = x;
				m_equal.m_yPos = y;
				m_equal.m_width = width;
				m_equal.m_height = height; 
				break;
			case '+':
				m_plus.m_xPos = x;
				m_plus.m_yPos = y;
				m_plus.m_width = width;
				m_plus.m_height = height; 
				break;
			case '-':
				m_dash.m_xPos = x;
				m_dash.m_yPos = y;
				m_dash.m_width = width;
				m_dash.m_height = height; 
				break;
			case '*':
				m_asterisk.m_xPos = x;
				m_asterisk.m_yPos = y;
				m_asterisk.m_width = width;
				m_asterisk.m_height = height; 
				break;
			case '%':
				m_percent.m_xPos = x;
				m_percent.m_yPos = y;
				m_percent.m_width = width;
				m_percent.m_height = height; 
				break;
			case '\'':
				m_apostrophe.m_xPos = x;
				m_apostrophe.m_yPos = y;
				m_apostrophe.m_width = width;
				m_apostrophe.m_height = height; 
				break;
			case '#':
				m_poundSign.m_xPos = x;
				m_poundSign.m_yPos = y;
				m_poundSign.m_width = width;
				m_poundSign.m_height = height; 
				break;
			case '$':
				m_dollarSign.m_xPos = x;
				m_dollarSign.m_yPos = y;
				m_dollarSign.m_width = width;
				m_dollarSign.m_height = height; 
				break;
			case '&':
				m_ampersand.m_xPos = x;
				m_ampersand.m_yPos = y;
				m_ampersand.m_width = width;
				m_ampersand.m_height = height; 
				break;
			case '\"':
				m_doubleQuote.m_xPos = x;
				m_doubleQuote.m_yPos = y;
				m_doubleQuote.m_width = width;
				m_doubleQuote.m_height = height; 
				break;
			case '<':
				m_lessThan.m_xPos = x;
				m_lessThan.m_yPos = y;
				m_lessThan.m_width = width;
				m_lessThan.m_height = height; 
				break;
			case '>':
				m_greaterThan.m_xPos = x;
				m_greaterThan.m_yPos = y;
				m_greaterThan.m_width = width;
				m_greaterThan.m_height = height; 
				break;
			case '@':
				m_atSign.m_xPos = x;
				m_atSign.m_yPos = y;
				m_atSign.m_width = width;
				m_atSign.m_height = height; 
				break;
			case '[':
				m_openSquareBracket.m_xPos = x;
				m_openSquareBracket.m_yPos = y;
				m_openSquareBracket.m_width = width;
				m_openSquareBracket.m_height = height; 
				break;
			case ']':
				m_closeSquareBracket.m_xPos = x;
				m_closeSquareBracket.m_yPos = y;
				m_closeSquareBracket.m_width = width;
				m_closeSquareBracket.m_height = height; 
				break;
			case '^':
				m_caret.m_xPos = x;
				m_caret.m_yPos = y;
				m_caret.m_width = width;
				m_caret.m_height = height; 
				break;
			case '_':
				m_underscore.m_xPos = x;
				m_underscore.m_yPos = y;
				m_underscore.m_width = width;
				m_underscore.m_height = height; 
				break;
			case '{':
				m_openCurlyBrace.m_xPos = x;
				m_openCurlyBrace.m_yPos = y;
				m_openCurlyBrace.m_width = width;
				m_openCurlyBrace.m_height = height; 
				break;
			case '}':
				m_closeCurlyBrace.m_xPos = x;
				m_closeCurlyBrace.m_yPos = y;
				m_closeCurlyBrace.m_width = width;
				m_closeCurlyBrace.m_height = height; 
				break;
			case '|':
				m_verticalBar.m_xPos = x;
				m_verticalBar.m_yPos = y;
				m_verticalBar.m_width = width;
				m_verticalBar.m_height = height; 
				break;
			}
		}
		while(true);

		//For right now, set space width to be equal to this letter
		m_spaceWidth = m_t.m_width;

		f.close();
	}
	
	
	return X_OK;
}



//*****************************************************************************
/**
Destroys a character set by freeing all of its allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XCharSet::destroy()
{
	//if (m_pX)
	//{
		if (m_texHandle != X_INVALID_HANDLE)
		{
			XVP::GraphicsMgr.destroyTexture(m_texHandle);
			m_texHandle = X_INVALID_HANDLE;
		}
	//}

	XResource::destroy();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XCharSet::getLineHeight()
{
	return m_lineHeight;
}



//*****************************************************************************
//*****************************************************************************
XU32 XCharSet::getSpaceWidth()
{
	return m_spaceWidth;
}