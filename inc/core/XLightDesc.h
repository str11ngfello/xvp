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
//	Module Name:	XLightDesc.h
//	Author:			Lynn Duke
//	Creation Date:	5-7-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_LIGHT_DESC_H__
#define __X_LIGHT_DESC_H__



#define DEFAULT_LIGHT_PRIORITY	(100)			//Higher numbers equal higher priority
#define SYSTEM_LIGHT_PRIORITY	(0xFFFFFFFF)	//Highest priority is reserved by the system


//*****************************************************************************
//!XLight description structure.
/**
This structure is used to describe the settings of a light.  Not all members
of the structure apply to all types of lights.  For example, the "direction"
member does not apply to point lights and the innerConeAngle and outerConeAngle
only apply to spot lights. 
*/
//*****************************************************************************
struct XLightDesc
{
	X_LIGHT_TYPE type;			/*!< The type of light. */	
	XColor diffuse;				/*!< The diffuse color component of the light */
	XColor specular;			/*!< The specular color component of the light */
	XF32 cutoffDistance;		
	XF32 hotSpotDistance;		
	XF32 cosHalfAngleCutoff;	
	XF32 cosHalfAngleDropoff;	
	XU32 priority;				/*!< Light priority where higher numbers = higher priority */
	XBOOL enableOcclusion;	

	XLightDesc::XLightDesc()
	{
		type = X_LT_POINT;
		diffuse = XColor(1.0f,1.0f,1.0f,1.0f);	//a = diffuse intensity
		specular = XColor(1.0f,1.0f,1.0f,1.0f);	//a = specular intensity
		cutoffDistance = 10.0f;
		hotSpotDistance = 5.0f;
		cosHalfAngleCutoff = .7f;
		cosHalfAngleDropoff = 1.7f;
		priority = DEFAULT_LIGHT_PRIORITY;
		enableOcclusion = false;
	}
};


#endif