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
//	Module Name:	XBoundingBox.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//Defines
//*****************************************************************************
static enum
{
	INSIDE,
    OUTSIDE
};

static enum
{
	PT_MIN,
    PT_MAX
};

#define BB_ABS(x)			((x)<0?(-x):(x))
#define BB_APPROX_EQ(x,v)	((BB_ABS((x)-(v)) <= 0.001f)?true:false)


//*****************************************************************************
//	Normals for each box side
//*****************************************************************************
static const XF32 SideNormals[6][3]= 
{
	// Top
	{  0.0,  1.0,  0.0 },
	// Bottom
	{  0.0, -1.0,  0.0 },
	// Right
	{  1.0,  0.0,  0.0 },
	// Left
	{ -1.0,  0.0,  0.0 },
	// Front
	{  0.0,  0.0,  1.0 },
	// Back
	{  0.0,  0.0, -1.0 }
};



//*****************************************************************************
//	Types of points for each vert of the box relative to the Min/Max
//*****************************************************************************
static const int PtTypes[8][3]=
{
	{ PT_MIN, PT_MIN, PT_MIN },
	{ PT_MAX, PT_MIN, PT_MIN },
	{ PT_MAX, PT_MIN, PT_MAX },
	{ PT_MIN, PT_MIN, PT_MAX },
	{ PT_MIN, PT_MAX, PT_MIN },
	{ PT_MAX, PT_MAX, PT_MIN },
	{ PT_MAX, PT_MAX, PT_MAX },
	{ PT_MIN, PT_MAX, PT_MAX }
};



//*****************************************************************************
//	Indices of points for each box face
//*****************************************************************************
static const int PtIndices[6][4]=
{
	// Top
	{ 7, 6, 5, 4 },
	// Bottom
	{ 0, 1, 2, 3 },
	// Right
	{ 1, 5, 7, 3 },
	// Left
	{ 0, 4, 6, 2 },
	// Front
	{ 7, 6, 2, 3 },
	// Back
	{ 0, 1, 5, 4 }
};



//*****************************************************************************
/**
Constructor for the bounding box side.
*/
//*****************************************************************************
XBoundingBoxSide::XBoundingBoxSide()
{
}



//*****************************************************************************
/**
Constructor for the bounding box.
*/
//*****************************************************************************
XBoundingBox::XBoundingBox()
{
}



//*****************************************************************************
/**
Constructor for the bounding box.
*/
//*****************************************************************************
XBoundingBox::XBoundingBox(XVector3D vert1,XVector3D vert2,XVector3D vert3)
{
	//Enlarge the bounding box to encompass the passed in points.

	//Max x component
	Max.x = X_MATH_MAX3(vert1.x,vert2.x,vert3.x);

	//Max y component
	Max.y = X_MATH_MAX3(vert1.y,vert2.y,vert3.y);

	//Max z component
	Max.z = X_MATH_MAX3(vert1.z,vert2.z,vert3.z);
	
	//Min x component
	Min.x = X_MATH_MIN3(vert1.x,vert2.x,vert3.x);
	
	//Min y component
	Min.y = X_MATH_MIN3(vert1.y,vert2.y,vert3.y);

	//Min z component
	Min.z = X_MATH_MIN3(vert1.z,vert2.z,vert3.z);
}



//*****************************************************************************
/**
Destructor for the bounding box side.
*/
//*****************************************************************************
XBoundingBoxSide::~XBoundingBoxSide()
{
}



//*****************************************************************************
/**
*/
//*****************************************************************************
XVector3D XBoundingBoxSide::GetBoxPt(XBoundingBox BBox, int Index)
{
	XVector3D Pt;

	if(PtTypes[Index][0] == PT_MIN)
		Pt.x = BBox.Min.x;
	else
		Pt.x = BBox.Max.x;

	if(PtTypes[Index][1] == PT_MIN)
		Pt.y = BBox.Min.y;
	else
		Pt.y = BBox.Max.y;

	if(PtTypes[Index][2] == PT_MIN)
		Pt.z = BBox.Min.z;
	else
		Pt.z = BBox.Max.z;

	return(Pt);
}



//*****************************************************************************
//*****************************************************************************
XBOOL XBoundingBoxSide::CoPlanar(XBoundingBoxSide TestSide)
{
	//Check to see if all points satisfy plane equation
	//for TestSide: Ax + By + Cz + D = 0
	for(int i = 0; i < 4; i++)
	{
		XF32 Result;

		Result = TestSide.Normal.x * Points[i].x + 
			 TestSide.Normal.y * Points[i].y + 
			 TestSide.Normal.z * Points[i].z +
			 TestSide.D;

		if(!BB_APPROX_EQ(Result, 0))
			return(false);
	}

	return(true);
}



//*****************************************************************************
//Check to see if all the points lie inside of TestSide
//*****************************************************************************
XBOOL XBoundingBoxSide::PointsInside(XBoundingBoxSide TestSide){
	
	for(int i= 0; i < 4; i++){
		
		XBOOL LessEq    = false,
		     GreaterEq = false;

		for(int j = 0; j < 4 || (!LessEq && !GreaterEq); j++){
			
			if(Points[i].x >= TestSide.Points[j].x)
				if(Points[i].y >= TestSide.Points[j].y)
					if(Points[i].z >= TestSide.Points[j].z)
						GreaterEq = true;

			if(Points[i].x <= TestSide.Points[j].x)
				if(Points[i].y <= TestSide.Points[j].y)
					if(Points[i].z <= TestSide.Points[j].z)
						LessEq= true;
		}

		if(!LessEq || !GreaterEq)
			return(false);
	}

	return(true);
}



//*****************************************************************************
//Determine all properties of the box side given a BBox and index (0 - 5) of
//the side
//*****************************************************************************
void XBoundingBoxSide::SetFromBBox(XBoundingBox BBox, int Side)
{
	if(Side >= 0 && Side <= 5)
	{
		int i;

		Normal.x = SideNormals[Side][0];
		Normal.y = SideNormals[Side][1];
		Normal.z = SideNormals[Side][2];

		for(i= 0; i < 4; i++)
			Points[i] = GetBoxPt(BBox, PtIndices[Side][i]);

		D= -(Points[0].x * Normal.x + Points[0].y * Normal.y + Points[0].z * Normal.z);
		
		CurSide = Side;
		
		Size = (BBox.Max.x - BBox.Min.x);
	}
}



//*****************************************************************************
//Determine if a plane neighbors this plane
//*****************************************************************************
XBOOL XBoundingBoxSide::Neighbors(XBoundingBoxSide TestSide)
{
	//Quick rejection: planes don't face each other
	if((CurSide % 2) == (TestSide.CurSide % 2))
		return(false);

	//Check to see if planes are coplanar
	if(CoPlanar(TestSide))
		if(PointsInside(TestSide))
			return(true);

	return(false);
}



//*****************************************************************************
//GetSize()
//*****************************************************************************
XF32 XBoundingBoxSide::GetSize()
{
	return(Size);
}
