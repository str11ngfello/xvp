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
//	Module Name:	XCharSet.h
//	Author:			Lynn Duke
//	Creation Date:	9-6-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_CHAR_SET_H__
#define __X_CHAR_SET_H__




//*****************************************************************************
//This structure represents coorinates for each char in a charset
//*****************************************************************************
class XCharCoords
{
public:
	XU32 m_xPos;	//x coordinate relative to upper left corner of the character
	XU32 m_yPos;	//y coordinate relative to upper left corner of the character	
	XU32 m_width;	//width of the character on the texture
	XU32 m_height;	//height of the character on the texture

	XCharCoords()
	{
		m_xPos = 0;
		m_yPos = 0;
		m_width = 0;
		m_height = 0;
	}
};



//*****************************************************************************
//! Represents a font including image data and character dimensions.
/**
The XCharSet object represents a font including image data and character
dimensions.  Character sets can be used to render text to the screen in
conjuction with the X_TEXT_BOX object.

See below for details.
*/
//*****************************************************************************
class XCharSet : public XResource
{
private:
	XU32				m_lineHeight;
	XU32				m_spaceWidth;

public:
	XHandle				m_texHandle;
	XCharCoords			m_a;
	XCharCoords			m_b;
	XCharCoords			m_c;
	XCharCoords			m_d;
	XCharCoords			m_e;
	XCharCoords			m_f;
	XCharCoords			m_g;
	XCharCoords			m_h;
	XCharCoords			m_i;
	XCharCoords			m_j;
	XCharCoords			m_k;
	XCharCoords			m_l;
	XCharCoords			m_m;
	XCharCoords			m_n;
	XCharCoords			m_o;
	XCharCoords			m_p;
	XCharCoords			m_q;
	XCharCoords			m_r;
	XCharCoords			m_s;
	XCharCoords			m_t;
	XCharCoords			m_u;
	XCharCoords			m_v;
	XCharCoords			m_w;
	XCharCoords			m_x;
	XCharCoords			m_y;
	XCharCoords			m_z;
	XCharCoords			m_A;
	XCharCoords			m_B;
	XCharCoords			m_C;
	XCharCoords			m_D;
	XCharCoords			m_E;
	XCharCoords			m_F;
	XCharCoords			m_G;
	XCharCoords			m_H;
	XCharCoords			m_I;
	XCharCoords			m_J;
	XCharCoords			m_K;
	XCharCoords			m_L;
	XCharCoords			m_M;
	XCharCoords			m_N;
	XCharCoords			m_O;
	XCharCoords			m_P;
	XCharCoords			m_Q;
	XCharCoords			m_R;
	XCharCoords			m_S;
	XCharCoords			m_T;
	XCharCoords			m_U;
	XCharCoords			m_V;
	XCharCoords			m_W;
	XCharCoords			m_X;
	XCharCoords			m_Y;
	XCharCoords			m_Z;
	XCharCoords			m_num0;
	XCharCoords			m_num1;
	XCharCoords			m_num2;
	XCharCoords			m_num3;
	XCharCoords			m_num4;
	XCharCoords			m_num5;
	XCharCoords			m_num6;
	XCharCoords			m_num7;
	XCharCoords			m_num8;
	XCharCoords			m_num9;
	XCharCoords			m_period;
	XCharCoords			m_comma;
	XCharCoords			m_questionMark;
	XCharCoords			m_colon;
	XCharCoords			m_semicolon;
	XCharCoords			m_openParen;
	XCharCoords			m_closeParen;
	XCharCoords			m_exclamation;
	XCharCoords			m_forwardSlash;
	XCharCoords			m_backSlash;
	XCharCoords			m_equal;
	XCharCoords			m_plus;
	XCharCoords			m_dash;
	XCharCoords			m_asterisk;
	XCharCoords			m_percent;
	XCharCoords			m_apostrophe;
	XCharCoords			m_poundSign;
	XCharCoords			m_dollarSign;
	XCharCoords			m_ampersand;
	XCharCoords			m_singleQuote;
	XCharCoords			m_doubleQuote;
	XCharCoords			m_lessThan;
	XCharCoords			m_greaterThan;
	XCharCoords			m_atSign;
	XCharCoords			m_openSquareBracket;
	XCharCoords			m_closeSquareBracket;
	XCharCoords			m_caret;
	XCharCoords			m_underscore;
	XCharCoords			m_openCurlyBrace;
	XCharCoords			m_closeCurlyBrace;
	XCharCoords			m_verticalBar;

	

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XCharSet();
	~XCharSet();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pTextureFilename,XS8* pCharDataFilename);
	XStatus destroy();
	XU32 getLineHeight();
	XU32 getSpaceWidth();
};

#endif