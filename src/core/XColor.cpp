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
//	Module Name:	XColor.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-27-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//Some common colors
XColor XColor::AliceBlue(0xf0f8ffff);       
XColor XColor::AntiqueWhite(0xfaebd7ff);         
XColor XColor::Aqua(0x00ffffff);
XColor XColor::Aquamarine(0x7fffd4ff);
XColor XColor::Azure(0xf0ffffff);
XColor XColor::Beige(0xf5f5dcff);
XColor XColor::Bisque(0xffe4c4ff);
XColor XColor::Black(0x000000ff);
XColor XColor::BlanchedAlmond(0xffebcdff);
XColor XColor::Blue(0x0000ffff);
XColor XColor::BlueViolet(0x8a2be2ff);
XColor XColor::Brown(0xa52a2aff);
XColor XColor::Burlywood(0xdeb887ff);
XColor XColor::CadetBlue(0x5f9ea0ff);
XColor XColor::Chartreuse(0x7fff00ff);
XColor XColor::Chocolate(0xd2691eff);
XColor XColor::Coral(0xff7f50ff);
XColor XColor::CornflowerBlue(0x6495edff);
XColor XColor::Cornsilk(0xfff8dcff);
XColor XColor::Cyan(0x00ffffff);
XColor XColor::DarkBlue(0x00008bff);
XColor XColor::DarkCyan(0x008b8bff);
XColor XColor::DarkGoldenrod(0xb8860bff);
XColor XColor::DarkGray(0xa9a9a9ff);
XColor XColor::DarkGreen(0x006400ff);
XColor XColor::DarkKhaki(0xbdb76bff);
XColor XColor::DarkMagenta(0x8b008bff);
XColor XColor::DarkOliveGreen(0x556b2fff);
XColor XColor::DarkOrange(0xff8c00ff);
XColor XColor::DarkOrchid(0x9932ccff);
XColor XColor::DarkRed(0x8b0000ff);
XColor XColor::DarkSalmon(0xe9967aff);
XColor XColor::DarkSeaGreen(0x8fbc8fff);
XColor XColor::DarkSlateBlue(0x483d8bff);
XColor XColor::DarkSlateGray(0x2f4f4fff);
XColor XColor::DarkTurquoise(0x00ced1ff);
XColor XColor::DarkViolet(0x9400d3ff);
XColor XColor::DeepPink(0xff1493ff);
XColor XColor::DeepSkyBlue(0x00bfffff);
XColor XColor::DimGray(0x696969ff);
XColor XColor::DodgerBlue(0x1e90ffff);
XColor XColor::Firebrick(0xb22222ff);
XColor XColor::FloralWhite(0xfffaf0ff);
XColor XColor::ForestGreen(0x228b22ff);
XColor XColor::Fuschia(0xff00ffff);
XColor XColor::Gainsboro(0xdcdcdcff);
XColor XColor::GhostWhite(0xf8f8ffff);
XColor XColor::Gold(0xffd700ff);
XColor XColor::Goldenrod(0xdaa520ff);
XColor XColor::Gray(0x808080ff);
XColor XColor::Green(0x008000ff);
XColor XColor::GreenYellow(0xadff2fff);
XColor XColor::Honeydew(0xf0fff0ff);
XColor XColor::HotPink(0xff69b4ff);
XColor XColor::IndianRed(0xcd5c5cff);
XColor XColor::Ivory(0xfffff0ff);
XColor XColor::Khaki(0xf0e68cff);
XColor XColor::Lavender(0xe6e6faff);
XColor XColor::LavenderBlush(0xfff0f5ff);
XColor XColor::LawnGreen(0x7cfc00ff);
XColor XColor::LemonChiffon(0xfffacdff);
XColor XColor::LightBlue(0xadd8e6ff);
XColor XColor::LightCoral(0xf08080ff);
XColor XColor::LightCyan(0xe0ffffff);
XColor XColor::LightGoldenrod(0xeedd82ff);
XColor XColor::LightGoldenrodYellow(0xfafad2ff);
XColor XColor::LightGray (0xd3d3d3ff);
XColor XColor::LightGreen(0x90ee90ff);
XColor XColor::LightPink(0xffb6c1ff);
XColor XColor::LightSalmon(0xffa07aff);
XColor XColor::LightSeaGreen(0x20b2aaff);
XColor XColor::LightSkyBlue(0x87cefaff);
XColor XColor::LightSlateBlue(0x8470ffff);
XColor XColor::LightSlateGray(0x778899ff);
XColor XColor::LightSteelBlue(0xb0c4deff);
XColor XColor::LightYellow(0xffffe0ff);
XColor XColor::Lime(0x00ff00ff);
XColor XColor::LimeGreen(0x32cd32ff);
XColor XColor::Linen(0xfaf0e6ff);
XColor XColor::Magenta(0xff00ffff);
XColor XColor::Maroon(0x800000ff);
XColor XColor::MediumAquamarine(0x66cdaaff);
XColor XColor::MediumBlue(0x0000cdff);
XColor XColor::MediumOrchid(0xba55d3ff);
XColor XColor::MediumPurple(0x9370dbff);
XColor XColor::MediumSeaGreen(0x3cb371ff);
XColor XColor::MediumSlateBlue(0x7b68eeff);
XColor XColor::MediumpringGreen(0x00fa9aff);
XColor XColor::MediumTurquoise(0x48d1ccff);
XColor XColor::MediumVioletRed(0xc71585ff);
XColor XColor::MidnightBlue(0x191970ff);
XColor XColor::MintCream(0xf5fffaff);
XColor XColor::MistyRose(0xe1e4e1ff);
XColor XColor::Moccasin(0xffe4b5ff);
XColor XColor::NavajoWhite(0xffdeadff);
XColor XColor::Navy(0x000080ff);
XColor XColor::OldLace(0xfdf5e6ff);
XColor XColor::Olive(0x808000ff);
XColor XColor::OliveDrab(0x6b8e23ff);
XColor XColor::Orange(0xffa500ff);
XColor XColor::OrangeRed(0xff4500ff);
XColor XColor::Orchid(0xda70d6ff);
XColor XColor::PaleGoldenrod(0xeee8aaff);
XColor XColor::PaleGreen(0x98fb98ff);
XColor XColor::PaleTurquoise(0xafeeeeff);
XColor XColor::PaleVioletRed(0xdb7093ff);
XColor XColor::PapayaWhip(0xffefd5ff);
XColor XColor::PeachPuff(0xffdab9ff);
XColor XColor::Peru(0xcd853fff);
XColor XColor::Pink(0xffc0cbff);
XColor XColor::Plum(0xdda0ddff);
XColor XColor::PowderBlue(0xb0e0e6ff);
XColor XColor::Purple(0x800080ff);
XColor XColor::Red(0xff0000ff);
XColor XColor::RosyBrown(0xbc8f8fff);
XColor XColor::RoyalBlue(0x4169e1ff);
XColor XColor::SaddleBrown(0x8b4513ff);
XColor XColor::Salmon(0xfa8072ff);
XColor XColor::SandyBrown(0xf4a460ff);
XColor XColor::SeaGreen(0x2e8b57ff);
XColor XColor::Seashell(0xfff5eeff);
XColor XColor::Sienna(0xa0522dff);
XColor XColor::Silver(0xc0c0c0ff);
XColor XColor::SkyBlue(0x87ceebff);
XColor XColor::SlateBlue(0x6a5acdff);
XColor XColor::SlateGray(0x708090ff);
XColor XColor::Snow(0xfffafaff);
XColor XColor::SpringGreen(0x00ff7fff);
XColor XColor::SteelBlue(0x4682b4ff);
XColor XColor::Tan(0xd2b48cff);
XColor XColor::Teal(0x008080ff);
XColor XColor::Thistle(0xd8bfd8ff);
XColor XColor::Tomato(0xff6347ff);
XColor XColor::Turquoise(0x40e0d0ff);
XColor XColor::VioletRed(0xd02090ff);
XColor XColor::Wheat(0xf5deb3ff);
XColor XColor::White(0xffffffff);
XColor XColor::WhiteSmoke(0xf5f5f5ff);
XColor XColor::Yellow(0xffff00ff);


//*****************************************************************************
/**
Constructor for the color.  Initializes the color object to transparent black.
<br><br>

<ul>
<li> Red Component = 0 </li>
<li> Green Component = 0 </li> 
<li> Blue Component = 0 </li> 
<li> Alpha Component = 0 </li> 
</ul>
*/
//*****************************************************************************
XColor::XColor()
{
	m_R = 0;
	m_G = 0;
	m_B = 0;
	m_A = 0;
}



//*****************************************************************************
/**
Constructs an XColor object by using four floating point values in the range
of 0 to 1.0.  0 represents the absence of this component in the final color
whereas 1.0 represents full intensity.  

@param	r	Red component of the color.  Value can range from 0 to 1.0. 
@param	g	Green component of the color.  Value can range from 0 to 1.0.
@param	b	Blue component of the color.  Value can range from 0 to 1.0.
@param	a	Alpha component of the color.  Value can range from 0 to 1.0.
*/
//*****************************************************************************
XColor::XColor(XF32 r,XF32 g,XF32 b,XF32 a)
{
	m_R = r;
	m_G = g;
	m_B = b;
	m_A = a;
	clamp();
}



XColor::XColor(XU32 RGBA)
{
	m_R = (XF32)((RGBA&0xFF000000)>>24)/255.0f;
	m_G = (XF32)((RGBA&0x00FF0000)>>16)/255.0f;
	m_B = (XF32)((RGBA&0x0000FF00)>>8)/255.0f;
	m_A = (XF32)(RGBA&0x000000FF)/255.0f;
}

//*****************************************************************************
/**
Destructor for the color.
*/
//*****************************************************************************
XColor::~XColor()
{
}



//*****************************************************************************
/**
Adds two XColor objects together by summing the red, green and blue color
components.  The alpha component is not included in the summation. The
color components are clamped to the range 0 to 1.0 and no "rollover" is performed. 

@param color The XColor object to add.
@return An XColor object that has the red, green and blue components of
		the two operands summed.  
*/
//*****************************************************************************
XColor XColor::operator + (const XColor &color)
{
	return XColor(m_R + color.m_R,m_G + color.m_G,m_B + color.m_B,m_A);
}



//*****************************************************************************
/**
Subtracts two XColor objects by subtracting the red, green and blue color
components of the operand.  The alpha component is not included in the 
subtraction.  The color components are clamped to the range 0 to 1.0 and no
"rollover" is performed.

@param color The XColor object to subtract.
@return An XColor object that has the difference in red, green and blue
		components of the two operands.  
*/
//*****************************************************************************
XColor XColor::operator - (const XColor &color)
{
	return XColor(m_R - color.m_R,m_G - color.m_G,m_B - color.m_B,m_A);
}



//*****************************************************************************
/**
Adds a floating point constant to the red, green and blue components and assigns
the result to this XColor object.  It does not add this constant to the alpha
component.  The	color components are clamped to the range 0 to 1.0 and no
"rollover" is performed.

@param val	The floating point constant to add to the red, green and blue
			components.
@return An XColor object that has the red, green and blue components
		incremented by the constant.    
*/
//*****************************************************************************
XColor XColor::operator += (XF32 val)
{
	m_R += val;
	m_G += val;
	m_B += val;
	clamp();
	return (*this);
}



//*****************************************************************************
/**
Subtracts a floating point constant from the red, green and blue components and
assigns the result to this XColor object.  It does not subtract this constant
from the alpha component.  The color components are clamped to the range 0 to
1.0 and no "rollover" is performed.

@param val	The floating point constant to subtract from the red, green and blue
			components.
@return An XColor object that has the red, green and blue components 
		decremented by the constant.    
*/
//*****************************************************************************
XColor XColor::operator -= (XF32 val)
{
	m_R -= val;
	m_G -= val;
	m_B -= val;
	clamp();
	return (*this);
}



//*****************************************************************************
/**
Adds two XColor objects together by summing the red, green and blue color
components and assigns the result to this XColor object.  The alpha component
is not included in the summation.  The alpha component remains unchanged.  The
color components are clamped to the range 0 to 1.0 and no "rollover" is performed. 

@param color The XColor object to add.
@return An XColor object that has the red, green and blue components of
		the two operands summed.  
*/
//*****************************************************************************
XColor XColor::operator += (const XColor& color)
{
	m_R += color.m_R;
	m_G += color.m_G;
	m_B += color.m_B;
	clamp();
	return (*this);
}



//*****************************************************************************
/**
Subtracts two XColor objects by subtracting the red, green and blue color
components of the operand and assigns the result to this XColor object.
The alpha component is not included in the subtraction.  The color components
are clamped to the range 0 to 1.0 and no "rollover" is performed.

@param color The XColor object to subtract.
@return An XColor object that has the difference in red, green and blue
		components of the two operands.  
*/
//*****************************************************************************
XColor XColor::operator -= (const XColor& color)
{
	m_R -= color.m_R;
	m_G -= color.m_G;
	m_B -= color.m_B;
	clamp();
	return (*this);
}
	


//*****************************************************************************
/**
Multiplies a floating point constant with the red, green and blue components and
assigns the result to this XColor object.  It does not multiply this constant
with the alpha component.  The color components are clamped to the range 0 to
1.0 and no "rollover" is performed.

@param val	The floating point constant to multiply with the red, green and blue
			components.
@return An XColor object that has the red, green and blue components 
		multiplied by the constant.    
*/
//*****************************************************************************
XColor XColor::operator * (XF32 val)
{	
	return XColor(m_R * val,m_G * val,m_B * val,m_A);
}	         



//*****************************************************************************
/**
Multiplies two XColor objects by multiplying the red, green and blue color
components of the operand.  The red components are multiplied together, the
green components are multiplied together and the blue components are multiplied
together.  The alpha component is not included in the multiplication.  
The color components are clamped to the range 0 to 1.0 and no "rollover" is
performed.

@param color The XColor object to multiply.
@return An XColor object that has the products of the red, green and blue
		components.  
*/
//*****************************************************************************
XColor XColor::operator * (const XColor& color)
{
	return XColor(m_R * color.m_R, m_G * color.m_G, m_B * color.m_B,m_A);
}



//*****************************************************************************
/**
Multiplies two XColor objects by multiplying the red, green and blue color
components of the operand and assigns the result to this XColor object.
The alpha component is not included in the multiplication.  The color components
are clamped to the range 0 to 1.0 and no "rollover" is performed.

@param color The XColor object to multiply.
@return An XColor object that has the product of the red, green and blue
		components.  
*/
//*****************************************************************************
XColor XColor::operator *= (const XColor& color)
{
	m_R *= color.m_R;
	m_G *= color.m_G;
	m_B *= color.m_B;
	m_A *= color.m_A;
	clamp();
	return (*this);
}

XColor XColor::operator *= (XF32 val)
{
	m_R *= val;
	m_G *= val;
	m_B *= val;
	m_A *= val;
	clamp();
	return (*this);
}


//*****************************************************************************
/**
Assigns an XColor object to another XColor object.  All four color components
are assigned including the red, green, blue and alpha components.

@param color XColor object to set this XColor object equal to.
@return The assigned XColor object.
*/
//*****************************************************************************
/*XColor &XColor::operator = (XColor& color)
{
	m_R = color.m_R;
	m_G = color.m_G;
	m_B = color.m_B;
	m_A = color.m_A;

	return *this;
}*/



//*****************************************************************************
/**
Determines if two XColor objects are equal.  Two XColor objects are equal if
and only if each component is equal.  In other words, the red components match,
the green components match, the blue components match and the alpha components
match.

@param	color	The XColor to compare.
@return	True if the XColor objects are equal, false otherwise.
*/
//*****************************************************************************
XBOOL XColor::operator == (XColor& color)
{
	if((m_R == color.m_R) && (m_G == color.m_G) && (m_B == color.m_B) && (m_A == color.m_A))
		return true;
	else
		return false;
}



//*****************************************************************************
/**
Clamps each color component to the range of 0 to 1.0.

@return None.
*/
//*****************************************************************************
void XColor::clamp()
{
	X_MATH_CLAMP(m_R,0,1.0f);
	X_MATH_CLAMP(m_G,0,1.0f);
	X_MATH_CLAMP(m_B,0,1.0f);
	X_MATH_CLAMP(m_A,0,1.0f);
}



//*****************************************************************************
/**
Performs linear interpolation between two XColor objects.  Linear interpolation
is only performed between the red, green and blue color components, not the
alpha component.  The final alpha component will be equal to the start color's
alpha component after this method returns.

@param finalColor	XColor object to hold the final interpolated color.
@param startColor	XColor object to start from.
@param endColor	XColor object to end with.
@param t	The interpolation factor.  This value must be between 0 and 1.0.  0
			represents the startColor where as 1.0 represents the endColor.  For
			example, a value of .5 would result in a color that is exactly half
			way between the startColor and endColor.
@return None.
*/
//*****************************************************************************
void XColor::colorLerp(XColor& finalColor,XColor startColor,XColor endColor,
						XF32 t)
{
	XF32 R1 = startColor.m_R;
	XF32 G1 = startColor.m_G;
	XF32 B1 = startColor.m_B;
	XF32 R2 = endColor.m_R;
	XF32 G2 = endColor.m_G;
	XF32 B2 = endColor.m_B;
	finalColor.m_R = X_MATH_LERP(t,R1,R2);
	finalColor.m_G = X_MATH_LERP(t,G1,G2);
	finalColor.m_B = X_MATH_LERP(t,B1,B2);
	finalColor.m_A = startColor.m_A;
}
	


//*****************************************************************************
/**
Returns the XColor object in a 32-bit packed ARGB format where each color
component is described by 8 bits.

@return A 32-bit value where bits 0-7 describe the blue component, bits 
		8-15 describe the green component, bits 16-23 describe the red
		component and bits 24-31 describe the alpha component.
*/
//*****************************************************************************
XU32 XColor::getARGB32Bit() 
{		
	return ((XU32)((((XU32(m_A*255))&0xff)<<24)|
					(((XU32(m_R*255))&0xff)<<16)|
					(((XU32(m_G*255))&0xff)<<8)|
					((XU32(m_B*255))&0xff)));
}



//*****************************************************************************
/**
Returns the XColor object in a 32-bit packed RGBA format where each color
component is described by 8 bits.

@return A 32-bit value where bits 0-7 describe the alpha component, bits 
		8-15 describe the blue component, bits 16-23 describe the green
		component and bits 24-31 describe the red component.
*/
//*****************************************************************************
XU32 XColor::getRGBA32Bit() 
{
	return ((XU32)((((XU32(m_R*255))&0xff)<<24)|
					(((XU32(m_G*255))&0xff)<<16)|
					(((XU32(m_B*255))&0xff)<<8)|
					((XU32(m_A*255))&0xff)));
}



//*****************************************************************************
/**
Returns the XColor object in a 32-bit packed RGBA format where each color
component is described by 8 bits.

@return A 32-bit value where bits 0-7 describe the alpha component, bits 
		8-15 describe the blue component, bits 16-23 describe the green
		component and bits 24-31 describe the red component.
*/
//*****************************************************************************
XU32 XColor::getABGR32Bit() 
{
	return ((XU32)((((XU32(m_A*255))&0xff)<<24)|
					(((XU32(m_B*255))&0xff)<<16)|
					(((XU32(m_G*255))&0xff)<<8)|
					((XU32(m_R*255))&0xff)));
}


//*****************************************************************************
/**
Sets the XColor object from a 32-bit packed ARGB format where each color
component is described by 8 bits.

@param	A 32-bit value where bits 0-7 describe the blue component, bits 
		8-15 describe the green component, bits 16-23 describe the red
		component and bits 24-31 describe the alpha component.
@return None.
*/
//*****************************************************************************
void XColor::setARGB32Bit(XU32 color)
{
	m_A = (XF32)((color&0xFF000000)>>24)/255.0f;
	m_R = (XF32)((color&0x00FF0000)>>16)/255.0f;
	m_G = (XF32)((color&0x0000FF00)>>8)/255.0f;
	m_B = (XF32)(color&0x000000FF)/255.0f;
}



//*****************************************************************************
/**
Sets the XColor object from a 32-bit packed RGBA format where each color
component is described by 8 bits.

@param	A 32-bit value where bits 0-7 describe the alpha component, bits 
		8-15 describe the blue component, bits 16-23 describe the green
		component and bits 24-31 describe the red component.
@return None.
*/
//*****************************************************************************
void XColor::setRGBA32Bit(XU32 color)
{
	m_R = (XF32)((color&0xFF000000)>>24)/255.0f;
	m_G = (XF32)((color&0x00FF0000)>>16)/255.0f;
	m_B = (XF32)((color&0x0000FF00)>>8)/255.0f;
	m_A = (XF32)(color&0x000000FF)/255.0f;
}



//*****************************************************************************
/**
Sets the XColor object from a 32-bit packed RGBA format where each color
component is described by 8 bits.

@param	A 32-bit value where bits 0-7 describe the alpha component, bits 
		8-15 describe the blue component, bits 16-23 describe the green
		component and bits 24-31 describe the red component.
@return None.
*/
//*****************************************************************************
void XColor::setABGR32Bit(XU32 color)
{
	m_A = (XF32)((color&0xFF000000)>>24)/255.0f;
	m_B = (XF32)((color&0x00FF0000)>>16)/255.0f;
	m_G = (XF32)((color&0x0000FF00)>>8)/255.0f;
	m_R = (XF32)(color&0x000000FF)/255.0f;
}
