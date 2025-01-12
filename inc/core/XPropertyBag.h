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
//	Module Name:	XPropertyBag.h
//	Author:			Lynn Duke
//	Creation Date:	7-15-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PROPERTY_BAG_H__
#define __X_PROPERTY_BAG_H__



//Convenience macros for creating properties
#define PB_MAKE_PROPERTY(propertyName,function) {if (strcmp(pPropertyName,propertyName) == 0) { function; return X_OK;	}}


//*****************************************************************************
//*****************************************************************************
class XPropertyBag
{
protected:
	

public:

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XS32 getIntProperty(XS8* pPropertyName);
	virtual XStatus setIntProperty(XS8* pPropertyName,XS32 value);
	virtual XU32 getUIntProperty(XS8* pPropertyName);
	virtual XStatus setUIntProperty(XS8* pPropertyName,XU32 value);
	virtual XF32 getFloatProperty(XS8* pPropertyName);
	virtual XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
	virtual XF64 getDoubleProperty(XS8* pPropertyName);
	virtual XStatus setDoubleProperty(XS8* pPropertyName,XF64 value);
	virtual XBOOL getBoolProperty(XS8* pPropertyName);
	virtual XStatus setBoolProperty(XS8* pPropertyName,XBOOL value);
	virtual XColor getColorProperty(XS8* pPropertyName);
	virtual XStatus setColorProperty(XS8* pPropertyName,XColor& value);
	virtual XS32 getEnumProperty(XS8* pPropertyName);
	virtual XStatus setEnumProperty(XS8* pPropertyName,XS32 value);
	virtual XVector2D getVector2DProperty(XS8* pPropertyName);
	virtual XStatus setVector2DProperty(XS8* pPropertyName,XVector2D& value);
	virtual XVector3D getVector3DProperty(XS8* pPropertyName);
	virtual XStatus setVector3DProperty(XS8* pPropertyName,XVector3D& value);
	virtual XVector4D getVector4DProperty(XS8* pPropertyName);
	virtual XStatus setVector4DProperty(XS8* pPropertyName,XVector4D& value);
	virtual XMatrix4x4 getMatrix4x4Property(XS8* pPropertyName);
	virtual XStatus setMatrix4x4Property(XS8* pPropertyName,XMatrix4x4& value);

};

#endif