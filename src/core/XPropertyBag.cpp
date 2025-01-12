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
//	Module Name:	XPropertyBag.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-15-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XS32 XPropertyBag::getIntProperty(XS8* pPropertyName)
{
	return 0;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setIntProperty(XS8* pPropertyName,XS32 value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XU32 XPropertyBag::getUIntProperty(XS8* pPropertyName)
{
	return 0;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setUIntProperty(XS8* pPropertyName,XU32 value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XF32 XPropertyBag::getFloatProperty(XS8* pPropertyName)
{
	return 0.0f;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XF64 XPropertyBag::getDoubleProperty(XS8* pPropertyName)
{
	return 0.0f;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setDoubleProperty(XS8* pPropertyName,XF64 value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XPropertyBag::getBoolProperty(XS8* pPropertyName)
{
	return false;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setBoolProperty(XS8* pPropertyName,XBOOL value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XColor XPropertyBag::getColorProperty(XS8* pPropertyName)
{
	return XColor(0,0,0,0);
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setColorProperty(XS8* pPropertyName,XColor& value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XS32 XPropertyBag::getEnumProperty(XS8* pPropertyName)
{
	return 0;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setEnumProperty(XS8* pPropertyName,XS32 value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XVector2D XPropertyBag::getVector2DProperty(XS8* pPropertyName)
{
	return XVector2D(0,0);
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setVector2DProperty(XS8* pPropertyName,XVector2D& value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XVector3D XPropertyBag::getVector3DProperty(XS8* pPropertyName)
{
	return XVector3D(0,0,0);
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setVector3DProperty(XS8* pPropertyName,XVector3D& value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XVector4D XPropertyBag::getVector4DProperty(XS8* pPropertyName)
{
	return XVector4D(0,0,0,0);
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setVector4DProperty(XS8* pPropertyName,XVector4D& value)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XMatrix4x4 XPropertyBag::getMatrix4x4Property(XS8* pPropertyName)
{
	return XMatrix4x4();
}



//*****************************************************************************
//*****************************************************************************
XStatus XPropertyBag::setMatrix4x4Property(XS8* pPropertyName,XMatrix4x4& value)
{
	return X_ERR_NO_SUCH_OBJECT;
}