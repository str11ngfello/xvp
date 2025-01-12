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
//	Module Name:	XIntersectLib.h
//	Author:			Lynn Duke
//	Creation Date:	11-13-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_INTERSECTLIB_H__
#define __X_INTERSECTLIB_H__





//*****************************************************************************
//! Object intersection types.
/**
These defines indicate different types of intersections between objects.
*/
//*****************************************************************************
enum X_INTERSECTION_TYPE
{
	X_IT_OUTSIDE,		/*!< No intersection. */
	X_IT_INSIDE,		/*!< One object is completely inside the other object. */
	X_IT_INTERSECTS		/*!< One object is both inside and outside the other object. */
};


//*****************************************************************************
//*****************************************************************************
struct XSphere
{
	XVector3D center;
	XF32 radius;
	
	XSphere::XSphere()
	{
		radius = 0.0f;
	}		

	XSphere::XSphere(XVector3D& c,XF32 r)
	{
		center = c;
		radius = r;
	}		
};



//*****************************************************************************
//*****************************************************************************
struct XAABB
{
    XVector3D min;            
	XVector3D max;           
};




//*****************************************************************************
//Volume Generation 
//*****************************************************************************
XStatus XIGenBoundingSphere(XSphere& sphere,XU32 numVerts,XVector3D* pVerts,XU32 stride);
XStatus XISphereEnclosingSpheres(XSphere& newSphere,XSphere& s0,XSphere& s1);
XStatus XIGenBoundingAABB(XAABB& AABB,XU32 numVerts,XVector3D* pVerts,XU32 stride);
XStatus XIAABBEnclosingAABBs(XAABB& a,XAABB& a0,XAABB& a1);
void XIGenUniformSpherePoints(XU32 numPoints,XVector3D* pPoints);
XU32 XIGenUniformHemiSpherePoints(XU32 numPoints,XVector3D* pPoints,XF32 threshold);



//*****************************************************************************
//Volume Transformation 
//*****************************************************************************
void XITransformSphere(XSphere& outSphere,XSphere& inSphere,XMatrix4x4& matrix);
void XITransformAABB(XAABB& outAABB,XAABB& inAABB,XMatrix4x4& matrix); 



//*****************************************************************************
//Ray Intersections 
//*****************************************************************************
XStatus XIRayPlane(XVector3D& rayOrigin,XVector3D& rayNormal,XVector3D& planePoint,
						XVector3D& planeNormal,XF32& distance); 
XBOOL XIRaySphere(XVector3D& rayOrigin,XVector3D& rayNormal,XVector3D& sphereCenter,
						  XF32 sphereRadius,XVector3D& pointOfIntersection,XF32& time);
XBOOL XIRaySphereQuick(XVector3D& rayOrigin,XVector3D& rayNormal,XVector3D& sphereCenter,
							   XF32 sphereRadius);
XBOOL XIRayAABB(XVector3D& rayOrigin,XVector3D& rayNormal,XVector3D& min,
						XVector3D& max,XVector3D& pointOfIntersection,XF32& time);
XBOOL XIRayTriangle(XVector3D& rayOrigin,XVector3D& rayNormal,
							XVector3D& a,XVector3D& b,XVector3D& c,
							XF32& baryU,XF32 &baryV,XF32& baryW,XF32& time);
XBOOL XIRayTriangleQuick(XVector3D& rayOrigin,XVector3D& rayNormal,
							XVector3D& a,XVector3D& b,XVector3D& c);




//*****************************************************************************
//Line Segment Intersections
//*****************************************************************************
XBOOL XILinePlane(XVector3D& lineStart,XVector3D& lineEnd,XVector3D& planePoint,
						  XVector3D& planeNormal,XVector3D& pointOfIntersection,XF32& time);
XBOOL XILineSphere(XVector3D& lineStart,XVector3D& lineEnd,XVector3D& sphereCenter,
						  XF32 sphereRadius,XVector3D& pointOfIntersection,XF32& time);
XBOOL XILineAABB(XVector3D& lineStart,XVector3D& lineEnd,XVector3D& min,
						 XVector3D& max);
XBOOL XILineTriangle(XVector3D& rayOrigin,XVector3D& rayNormal,
							XVector3D& a,XVector3D& b,XVector3D& c,
							XF32& baryU,XF32 &baryV,XF32& baryW,XF32& time);
XBOOL XILineTriangleQuick(XVector3D& rayOrigin,XVector3D& rayNormal,
							XVector3D& a,XVector3D& b,XVector3D& c);



//*****************************************************************************
//AABB Intersections 
//*****************************************************************************
XBOOL XIAABBAABB(XVector3D& min1,XVector3D& max1,XVector3D& min2,XVector3D& max2);
X_INTERSECTION_TYPE XIAABBPlane(XVector3D& min,XVector3D& max,XVector3D& planePoint,
								XVector3D& planeNormal);
XS32 XIAABBTriangle(XVector3D& min,XVector3D& max,XVector3D& a,XVector3D& b,XVector3D& c);


//*****************************************************************************
//Sphere Intersections 
//*****************************************************************************
XBOOL XISphereSphere(XVector3D& sphereCenter1,XF32 radius1,XVector3D& sphereCenter2,
					 XF32 radius2);
X_INTERSECTION_TYPE XISpherePlane(XVector3D& sphereCenter,XF32 sphereRadius,XVector3D& planePoint,
								XVector3D& planeNormal);
XBOOL XISphereAABB(XVector3D& sphereCenter,XF32 sphereRadius,XVector3D& min,XVector3D& max);
XBOOL XISphereTriangle(XVector3D& sphereCenter,XF32 sphereRadius,XVector3D& a,XVector3D& b,
					   XVector3D& c,XVector3D& closest);



//*****************************************************************************
//Point Intersections
//*****************************************************************************
XBOOL XIPointOnLine(XVector3D& a, XVector3D& b, XVector3D& p,XF32 lineHalfWidth);
XBOOL XIPointInTriangle(XVector3D& p,XVector3D& a,XVector3D& b,XVector3D& c);
XBOOL XIPointInSphere(XVector3D& p,XVector3D& sphereCenter,XF32 sphereRadius);
XBOOL XIPointInAABB(XVector3D& p,XVector3D& min,XVector3D& max);



//*****************************************************************************
//Closest Point Intersections 
//*****************************************************************************
void XIClosestPointOnTriangle(XVector3D& a,XVector3D& b,XVector3D& c,XVector3D& p,
							 XVector3D& closest);
void XIClosestPointOnLine(XVector3D& a,XVector3D& b,XVector3D& p,XVector3D& closest,XF32& time);
void XIClosestPointOnRay(XVector3D& a,XVector3D& b,XVector3D& p,XVector3D& closest,XF32& time);
void XIClosestPointOnPlane(XVector3D& planePoint,XVector3D& planeNormal,XVector3D& p,
						   XVector3D& closest);
void XIClosestPointOnAABB(XVector3D& min,XVector3D& max,XVector3D& p,XVector3D& closest);
XF32 XIClosestPointBetweenTwoLines(XVector3D& a1,XVector3D& b1,XVector3D& a2,XVector3D& b2,
                              XF32& time1,XF32& time2,XVector3D& closest1,XVector3D& closest2);



//*****************************************************************************
//Distance  
//*****************************************************************************
XF32 XIPointLineDistanceSquared(XVector3D& a,XVector3D& b,XVector3D& c);
XF32 XIPointAABBDistanceSquared(XVector3D& min,XVector3D& max,XVector3D& p);
XF32 XIPointPlaneDistance(XVector3D& planePoint,XVector3D& planeNormal,XVector3D& p);



//*****************************************************************************
//Miscellaeous Helper Functions
//*****************************************************************************
XBOOL XIQuadConvex(XVector3D& a,XVector3D& b,XVector3D& c,XVector3D& d);
XS32 XIFacePlane(XVector3D& p);
XS32 XIBevel2D(XVector3D& p);
XS32 XIBevel3D(XVector3D& p);
XS32 XICheckPoint(XVector3D& p1,XVector3D& p2,XF32 alpha,XS32 mask);
XS32 XICheckLine(XVector3D& p1,XVector3D& p2,XS32 outcode_diff);
XS32 XITriangleCube(XVector3D& a,XVector3D& b,XVector3D& c);
XVector3D XITangentPlaneNormalOfEllipsoid(XVector3D point,XVector3D eO, XVector3D eR);
XF32 XITriArea2DProj(XF32 x1,XF32 y1,XF32 x2,XF32 y2,XF32 x3,XF32 y3);
XF32 XITriArea3D(XVector3D& a,XVector3D& b,XVector3D& c);
void XIComputeBaryCentric(XVector3D& point,XVector3D& a, XVector3D& b, XVector3D& c,
						  XF32& u,XF32& v,XF32& w);
XRay3D XICreatePickRay(XS32 vpX,XS32 vpY,XU32 vpWidth,XU32 vpHeight,
					 XMatrix4x4& viewMatrix,XMatrix4x4& projMatrix);
void XICreatePickLine(XS32 vpX,XS32 vpY,XU32 vpWidth,XU32 vpHeight,
					 XMatrix4x4& viewMatrix,XMatrix4x4& projMatrix,XVector3D& lineStart,
					 XVector3D& lineEnd);
XVector2D XITransformPointWorldToViewport(XVector3D& worldPoint,XU32 vpWidth,XU32 vpHeight,
										  XMatrix4x4& viewMatrix,XMatrix4x4& projMatrix);

#endif