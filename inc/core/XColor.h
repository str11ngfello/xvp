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
//	Module Name:	XColor.h
//	Author:			Lynn Duke
//	Creation Date:	6-27-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_COLOR_H__
#define __X_COLOR_H__




//*****************************************************************************
//!Represents color in X-VP.
/**
The XColor object is used by X-VP to represent color.  You'll notice
that many of the API functions accept or return color information and that they
do this through the use of the XColor object.

Internally the XColor object uses four color components, namely red, green,
blue and alpha.  Each component is specified by a number from the range of 0
to 1.0 that represents the intensity of that particular component.  For example,
a red component value of 1.0 specifies that the color has maximum red intensity.
Likewise a green component value of 0 specifies that the color has no green.

Similarily, the alpha component represents how transparent the color is.  If a
0 is used for the alpha component then the color is completely transparent where
as a value of 1.0 means the color is completely opaque.

Internally, the color object maintains 4 floating point values, one for each of
the components.  You can access these directly for speed and ease of use.  They
are: 

<ul>
<li> m_R - Red component.  </li>
<li> m_G - Green component.  </li>
<li> m_B - Blue component.  </li>
<li> m_A - Alpha component.  </li>
</ul>

There are also member functions that allow you to perform operations on the color
object besides getting and setting individual components.  See the member function
documentation for more details.
*/
//*****************************************************************************
class XColor
{
public:
	XF32 m_R;	//Red
	XF32 m_G;	//Green
	XF32 m_B;	//Blue
	XF32 m_A;	//Alpha

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XColor();
	XColor(XF32 r,XF32 g,XF32 b,XF32 a);
	XColor(XU32 RGBA);
	~XColor();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XColor operator + (const XColor& color);
	XColor operator - (const XColor& color);
	XColor operator += (XF32 val);
	XColor operator -= (XF32 val);
	XColor operator += (const XColor& color);
	XColor operator -= (const XColor& color);
	XColor operator * (XF32 val);
	XColor operator * (const XColor& color);
	XColor operator *= (const XColor& color);
	XColor operator *= (XF32 val);
	//XColor &operator = (XColor& color);
	XBOOL operator == (XColor& color);
	void clamp();
	static void colorLerp(XColor& finalColor,XColor startColor,XColor endColor,XF32 t);
	XU32 getARGB32Bit();
	XU32 getRGBA32Bit();
	XU32 getABGR32Bit();
	void setARGB32Bit(XU32 color);
	void setRGBA32Bit(XU32 color);
	void setABGR32Bit(XU32 color);

	//Some common colors
	static XColor AliceBlue;      
	static XColor AntiqueWhite;     
	static XColor Aqua;
	static XColor Aquamarine;
	static XColor Azure;
	static XColor Beige;
	static XColor Bisque;
	static XColor Black;
	static XColor BlanchedAlmond;
	static XColor Blue;
	static XColor BlueViolet;
	static XColor Brown;
	static XColor Burlywood;
	static XColor CadetBlue;
	static XColor Chartreuse;
	static XColor Chocolate;
	static XColor Coral;
	static XColor CornflowerBlue;
	static XColor Cornsilk;
	static XColor Cyan;
	static XColor DarkBlue;
	static XColor DarkCyan;
	static XColor DarkGoldenrod;
	static XColor DarkGray;
	static XColor DarkGreen;
	static XColor DarkKhaki;
	static XColor DarkMagenta;
	static XColor DarkOliveGreen;
	static XColor DarkOrange;
	static XColor DarkOrchid;
	static XColor DarkRed;
	static XColor DarkSalmon;
	static XColor DarkSeaGreen;
	static XColor DarkSlateBlue;
	static XColor DarkSlateGray;
	static XColor DarkTurquoise;
	static XColor DarkViolet;
	static XColor DeepPink;
	static XColor DeepSkyBlue;
	static XColor DimGray;
	static XColor DodgerBlue;
	static XColor Firebrick;
	static XColor FloralWhite;
	static XColor ForestGreen;
	static XColor Fuschia;
	static XColor Gainsboro;
	static XColor GhostWhite;
	static XColor Gold;
	static XColor Goldenrod;
	static XColor Gray;
	static XColor Green;
	static XColor GreenYellow;
	static XColor Honeydew;
	static XColor HotPink;
	static XColor IndianRed;
	static XColor Ivory;
	static XColor Khaki;
	static XColor Lavender;
	static XColor LavenderBlush;
	static XColor LawnGreen;
	static XColor LemonChiffon;
	static XColor LightBlue;
	static XColor LightCoral;
	static XColor LightCyan;
	static XColor LightGoldenrod;
	static XColor LightGoldenrodYellow;
	static XColor LightGray;
	static XColor LightGreen;
	static XColor LightPink;
	static XColor LightSalmon;
	static XColor LightSeaGreen;
	static XColor LightSkyBlue;
	static XColor LightSlateBlue;
	static XColor LightSlateGray;
	static XColor LightSteelBlue;
	static XColor LightYellow;
	static XColor Lime;
	static XColor LimeGreen;
	static XColor Linen;
	static XColor Magenta;
	static XColor Maroon;
	static XColor MediumAquamarine;
	static XColor MediumBlue;
	static XColor MediumOrchid;
	static XColor MediumPurple;
	static XColor MediumSeaGreen;
	static XColor MediumSlateBlue;
	static XColor MediumpringGreen;
	static XColor MediumTurquoise;
	static XColor MediumVioletRed;
	static XColor MidnightBlue;
	static XColor MintCream;
	static XColor MistyRose;
	static XColor Moccasin;
	static XColor NavajoWhite;
	static XColor Navy;
	static XColor OldLace;
	static XColor Olive;
	static XColor OliveDrab;
	static XColor Orange;
	static XColor OrangeRed;
	static XColor Orchid;
	static XColor PaleGoldenrod;
	static XColor PaleGreen;
	static XColor PaleTurquoise;
	static XColor PaleVioletRed;
	static XColor PapayaWhip;
	static XColor PeachPuff;
	static XColor Peru;
	static XColor Pink;
	static XColor Plum;
	static XColor PowderBlue;
	static XColor Purple;
	static XColor Red;
	static XColor RosyBrown;
	static XColor RoyalBlue;
	static XColor SaddleBrown;
	static XColor Salmon;
	static XColor SandyBrown;
	static XColor SeaGreen;
	static XColor Seashell;
	static XColor Sienna;
	static XColor Silver;
	static XColor SkyBlue;
	static XColor SlateBlue;
	static XColor SlateGray;
	static XColor Snow;
	static XColor SpringGreen;
	static XColor SteelBlue;
	static XColor Tan;
	static XColor Teal;
	static XColor Thistle;
	static XColor Tomato;
	static XColor Turquoise;
	static XColor VioletRed;
	static XColor Wheat;
	static XColor White;
	static XColor WhiteSmoke;
	static XColor Yellow;
	static XColor YellowGreen;      


};

#endif
