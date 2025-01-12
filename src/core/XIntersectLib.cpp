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
//	Module Name:	XIntersectLib.cpp
//	Author:			Lynn Duke
//	Creation Date:	11-13-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XStatus XIGenBoundingSphere(XSphere& sphere,XU32 numVerts,XVector3D* pVerts,XU32 stride)
{
    XVector3D minX,maxX;
	XVector3D minY,maxY;
	XVector3D minZ,maxZ;
    
    minX = *pVerts;
	maxX = minX;
	minY = minX;
	maxY = minX;
	minZ = minX;
	maxZ = minX;
    
	XVector3D point;
    for (XU32 i = 1;i < numVerts;++i)
    {
        point = *((XVector3D*)((XS8*)pVerts + i*stride));
        
        if (point.x < minX.x)
            minX = point;
        if (point.x > maxX.x)
            maxX = point;
        if (point.y < minY.y)
            minY = point;
        if (point.y > maxY.y)
            maxY = point;
        if (point.z < minZ.z)
            minZ = point;
        if (point.z > maxZ.z)
            maxZ = point;
    }
    
	XVector3D center;
    XF32 radius;

	//Compute X distance.
	XF32 distanceX;
    distanceX = (maxX - minX).mag();

	//Compute X distance.
	XF32 distanceY;
	distanceY = (maxY - minY).mag();

	//Compute X distance.
	XF32 distanceZ;
	distanceZ = (maxZ - minZ).mag();
    
	if (distanceX > distanceY)
    {
        if (distanceX > distanceZ)
        {
            center = (maxX + minX) * 0.5f;
            radius = distanceX * 0.5f;
        }
        else
        {
            center = (maxZ + minZ) * 0.5f;
            radius = distanceZ * 0.5f;
        }
    }
    else 
    {
        if (distanceY >  distanceZ)
        {
            center = (maxY + minY) * 0.5f;
            radius = distanceY * 0.5f;
        }
        else
        {
            center = (maxZ + minZ) * 0.5f;
            radius = distanceZ * 0.5f;
        }
    }
    
    //Just to be sure, loop through the verts and make sure they are
	//all inside.  This isn't necessary but ensures that our sphere
	//encompasses all verts.
	XVector3D delta;
    XF32 distance;

	for (XU32 i = 0;i < numVerts;++i)
    {
        point = *((XVector3D*)((XS8*)pVerts + i*stride));
        delta = point - center;
		distance = delta.mag();
        
        if (distance > radius)
        {
            //Include this point in the sphere
            radius = (radius + distance) * 0.5f;	
            center += ((1.0f - radius * (1.0f/distance)) * delta);
        }
    }
    
	//Assign the final values
    sphere.center = center;
    sphere.radius = radius;
    
    return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XISphereEnclosingSpheres(XSphere& newSphere,XSphere& s0,XSphere& s1)
{
    //Compute the squared distance between the sphere centers
    XVector3D d = s1.center - s0.center;
    XF32 dist2 = d.dot(d);
	
	XF32 delta = s1.radius - s0.radius;
    if ((delta*delta) >= dist2) 
	{
        //The sphere with the larger radius encloses the other;
        //just set newSphere to be the larger of the two spheres
        if (s1.radius >= s0.radius)
            newSphere = s1;
        else
            newSphere = s0;
    } 
	else
	{
        //Spheres partially overlapping or disjoint
        XF32 dist = (XF32)sqrt(dist2);
        newSphere.radius = (dist + s0.radius + s1.radius) * 0.5f;
        newSphere.center = s0.center;
        if (dist > X_MATH_EPSILON)
            newSphere.center += ((newSphere.radius - s0.radius) / dist) * d;
    }

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XIGenBoundingAABB(XAABB& AABB,XU32 numVerts,XVector3D* pVerts,XU32 stride)
{
    XVector3D vMin, vMax;
    vMin = vMax = *pVerts;
    
	//Spin through each vertex and record the largest and smallest one found.
    for (XU32 i = 1; i < numVerts;++i)
    {
        XVector3D Point = *((XVector3D*)((XS8*)pVerts + i*stride));

		vMin.x = (vMin.x < Point.x) ? vMin.x : Point.x;
		vMin.y = (vMin.y < Point.y) ? vMin.y : Point.y;
		vMin.z = (vMin.z < Point.z) ? vMin.z : Point.z;
		vMax.x = (vMax.x > Point.x) ? vMax.x : Point.x;
		vMax.y = (vMax.y > Point.y) ? vMax.y : Point.y;
		vMax.z = (vMax.z > Point.z) ? vMax.z : Point.z;
    }

    //Record the final information.
	AABB.min = vMin;
	AABB.max = vMax;
    
    return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XIAABBEnclosingAABBs(XAABB& newAABB,XAABB& a0,XAABB& a1)
{
    for (XU32 i = 0;i < 2;++i) 
	{
        newAABB.min.array[i] = X_MATH_MIN(a0.min.array[i],a1.min.array[i]);
        newAABB.max.array[i] = X_MATH_MAX(a0.max.array[i],a1.max.array[i]);
    }

	return X_OK;
}

void XIGenUniformSpherePoints(XU32 numPoints,XVector3D* pPoints)
{
	X_ASSERT(numPoints);
	X_ASSERT(pPoints);

	//Golden Spiral method
	XF32 inc = X_MATH_PI * (3.0f - sqrt(5.0f));
    XF32 off = 2.0f / numPoints;
    for (XU32 i = 0;i < numPoints; ++i)
	{
        XF32 y = i * off - 1.0f + (off / 2.0f);
        XF32 r = sqrt(1.0f - y*y);
        XF32 phi = i * inc;
        pPoints[i] = XVector3D(cos(phi)*r,y,sin(phi)*r);
	}
}


XU32 XIGenUniformHemiSpherePoints(XU32 numPoints,XVector3D* pPoints,XF32 threshold)
{
	X_ASSERT(numPoints);
	X_ASSERT(pPoints);

	//Golden Spiral method
	XF32 inc = X_MATH_PI * (3.0f - sqrt(5.0f));
    XF32 off = 2.0f / numPoints;
	XU32 numGenPoints = 0;
    for (XU32 i = 0;i < numPoints; ++i)
	{
        XF32 y = i * off - 1.0f + (off / 2.0f);
        XF32 r = sqrt(1.0f - y*y);
        XF32 phi = i * inc;
		if (y >= threshold)
		{
			pPoints[numGenPoints] = XVector3D(cos(phi)*r,y,sin(phi)*r);
			numGenPoints++;
		}
	}

	return numGenPoints;
}

//*****************************************************************************
//*****************************************************************************
void XITransformSphere(XSphere& outSphere,XSphere& inSphere,XMatrix4x4& matrix)
{
	//Transform the center.
	outSphere.center = matrix * inSphere.center;

	//Update the radius with the current scale
	XF32 xScale,yScale,zScale;
	XVector3D scale;
	scale.x = matrix._11;
	scale.y = matrix._12;
	scale.z = matrix._13;
	xScale = scale.mag();

	scale.x = matrix._21;
	scale.y = matrix._22;
	scale.z = matrix._23;
	yScale = scale.mag();

	scale.x = matrix._31;
	scale.y = matrix._32;
	scale.z = matrix._33;
	zScale = scale.mag();

	outSphere.radius = inSphere.radius * X_MATH_MAX3(xScale,yScale,zScale);
}



//*****************************************************************************
//*****************************************************************************
void XITransformAABB(XAABB& outAABB,XAABB& inAABB,XMatrix4x4& matrix) 
{
	static XVector3D offset[8] = { XVector3D(-1.0f, -1.0f, -1.0f),
                                   XVector3D(-1.0f, -1.0f,  1.0f),
                                   XVector3D(-1.0f,  1.0f, -1.0f),
                                   XVector3D(-1.0f,  1.0f,  1.0f),
                                   XVector3D( 1.0f, -1.0f, -1.0f),
                                   XVector3D( 1.0f, -1.0f,  1.0f),
                                   XVector3D( 1.0f,  1.0f, -1.0f),
			                       XVector3D( 1.0f,  1.0f,  1.0f) };
	   
	//Compute and transform the corners and find new min/max bounds.
	//boxFactor is the center + extents used to determine each corner
	XVector3D min,max;
	XVector3D center = (inAABB.max + inAABB.min) * .5f;
	XVector3D extents = (inAABB.max - inAABB.min) * .5f;

	XVector3D corner = center + (extents * offset[0]);
    corner = matrix * corner;
    min = max = corner;
    
    for (XU32 i = 1;i < 8;++i)
    {
        corner = center + (extents * offset[i]);
        corner = matrix * corner;

		min.x = (min.x < corner.x) ? min.x : corner.x;
		min.y = (min.y < corner.y) ? min.y : corner.y;
		min.z = (min.z < corner.z) ? min.z : corner.z;
		max.x = (max.x > corner.x) ? max.x : corner.x;
		max.y = (max.y > corner.y) ? max.y : corner.y;
		max.z = (max.z > corner.z) ? max.z : corner.z;
    }

    // Store center and extents.
    outAABB.min = min;
	outAABB.max = max;
}



//*****************************************************************************
// Input : rayOrigin - origin of ray in world space
//         rayNormal - normal describing direction of ray in world space
//         planePoint - Origin of plane 
//         planeNormal - Normal to plane
// Notes : Normalized directional vectors expected
// Return: distance to plane in world units, positive if in front of ray,
//negative if behind ray.  This function protects against division by 0 by returning
//zero.  In this case the value returned is not technically valid, since the 
//ray is parallel to the plane
//*****************************************************************************
XStatus XIRayPlane(XVector3D& rayOrigin,XVector3D& rayNormal,XVector3D& planePoint,
						XVector3D& planeNormal,XF32& distance)
{  
	XF32 d = -(planeNormal.dot(planePoint));
	XF32 numerator = planeNormal.dot(rayOrigin) + d;
	XF32 denomenator = planeNormal.dot(rayNormal);
  
	//If plane and ray are parallel there is no valid intersection point.
	if (X_MATH_IS_XF32_ZERO(denomenator))  
		return X_ERR_OPERATION_FAILED;
   
	distance = -(numerator/denomenator);
	return X_OK;	
}



//*****************************************************************************
// Input : rO - origin of ray in world space
//         rV - vector describing direction of ray in world space
//         sO - Origin of sphere 
//         sR - radius of sphere
// Notes : Normalized directional vectors expected
// Return: distance to sphere in world units, -1 if no intersection.
//*****************************************************************************
XBOOL XIRaySphere(XVector3D& rayOrigin,XVector3D& rayNormal,XVector3D& sphereCenter,
						  XF32 sphereRadius,XVector3D& pointOfIntersection,XF32& time)
{	
	XVector3D m = rayOrigin - sphereCenter;
	XF32 b = m.dot(rayNormal);
	XF32 c = m.dot(m) - (sphereRadius * sphereRadius);

	if (c > 0.0f && b > 0.0f)
		return false;

	XF32 discriminant = (b * b) - c; 

	//If discriminant is less than 0, ray misses sphere.
	if (discriminant < 0.0f)
		return false;

	//At this point we konw that the ray intersects the sphere, we just
	//need to find when and where.
	time = -b - sqrt(discriminant);

	//Remember, if the time is negative, that means the ray starts inside
	//the sphere so we should clamp time to 0.
	if (time < 0.0f)
		time = 0.0f;
	pointOfIntersection = rayOrigin + (time * rayNormal);
	
	return true;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XIRaySphereQuick(XVector3D& rayOrigin,XVector3D& rayNormal,XVector3D& sphereCenter,
							   XF32 sphereRadius)
{
	XVector3D m = rayOrigin - sphereCenter;
	XF32 c = m.dot(m) - (sphereRadius * sphereRadius);

	if (c <= 0.0f)
		return true;

	XF32 b = m.dot(rayNormal);
	
	//If ray is pointing away from sphere, return false.
	if (b > 0.0f)
		return false;

	XF32 discriminant = (b * b) - c;

	//If the discriminant is negative, that means the ray does not intersect the sphere.
	if (discriminant < 0.0f)
		return false;

	//If we make it here, there is an intersection.
	return true;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XIRayAABB(XVector3D& rayOrigin,XVector3D& rayNormal,XVector3D& min,
						XVector3D& max,XVector3D& pointOfIntersection,XF32& time)
{
	time = 0.0f;          
    XF32 tmax = FLT_MAX; 
	XF32 ood,t1,t2;

    //Using the slabs method.
	for (XU32 i = 0;i < 3;++i)
	{
		if (fabs(rayNormal.array[i]) < X_MATH_EPSILON)
		{
            //The ray is parallel to slab. There's no hit if origin is not within slab
            if (rayOrigin.array[i] < min.array[i] || rayOrigin.array[i] > max.array[i]) 
				return false;
        } 
		else 
		{
            //Compute the intersection value of the ray with near and far plane of slab
            ood = 1.0f / rayNormal.array[i];
            t1 = (min.array[i] - rayOrigin.array[i]) * ood;
            t2 = (max.array[i] - rayOrigin.array[i]) * ood;
            
			//Make t1 be intersection with near plane, t2 with far plane
            if (t1 > t2) 
			{
				XF32 temp;
				temp = t2;
				t2 = t1;
				t1 = temp;
			}
            
			//Compute the intersection of slab intersections intervals
            if (t1 > time) 
				time = t1;
            if (t2 < tmax) 
				tmax = t2;
            
			//Exit with no collision as soon as slab intersection becomes empty
            if (time > tmax)
				return false;
        }
    }
    
	//Ray intersects all 3 slabs. Return intersection point and time value 
    pointOfIntersection = rayOrigin + (rayNormal * time);
    return true;
}


//*****************************************************************************
//a b c are counter clockwise...
//*****************************************************************************
XBOOL XIRayTriangle(XVector3D& rayOrigin,XVector3D& rayNormal,
							XVector3D& a,XVector3D& b,XVector3D& c,
							XF32& baryU,XF32 &baryV,XF32& baryW,XF32& time)
{
    XVector3D ab = b - a;
    XVector3D ac = c - a;
    XVector3D qp = -rayNormal;

    //Compute triangle normal. Can be precalculated or cached if
    //intersecting multiple segments against the same triangle
    XVector3D n = ab.cross(ac);

    //Compute denominator d. If d <= 0, segment is parallel to or points
    //away from triangle, so exit early
    XF32 d = qp.dot(n);
    if (d <= 0.0f)
		return false;

    //Compute intersection ttime value of pq with plane of triangle. A ray
    //intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
    //dividing by d until intersection has been found to pierce triangle
    XVector3D ap = rayOrigin - a;
    time = ap.dot(n);
    if (time < 0.0f) 
		return false;

    //Compute barycentric coordinate components and test if within bounds
    XVector3D e = qp.cross(ap);
    baryV = ac.dot(e);
    if (baryV < 0.0f || baryV > d)
		return false;
    baryW = -ab.dot(e);
    if (baryW < 0.0f || baryV + baryW > d) 
		return false;

    //Line intersects triangle. Perform delayed division and
    //compute the last barycentric coordinate component
    XF32 ood = 1.0f / d;
    time *= ood;
    baryV *= ood;
    baryW *= ood;
    baryU = 1.0f - baryV - baryW;
    return true;
}


//*****************************************************************************
//a b c are counter clockwise...
//*****************************************************************************
XBOOL XIRayTriangleQuick(XVector3D& rayOrigin,XVector3D& rayNormal,
							XVector3D& a,XVector3D& b,XVector3D& c)
{
    XVector3D ab = b - a;
    XVector3D ac = c - a;
    XVector3D qp = -rayNormal;

    //Compute triangle normal. Can be precalculated or cached if
    //intersecting multiple segments against the same triangle
    XVector3D n = ab.cross(ac);

	XF32 time,baryV,baryW;

    //Compute denominator d. If d <= 0, segment is parallel to or points
    //away from triangle, so exit early
    XF32 d = qp.dot(n);
    if (d <= 0.0f)
		return false;

    //Compute intersection ttime value of pq with plane of triangle. A ray
    //intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
    //dividing by d until intersection has been found to pierce triangle
    XVector3D ap = rayOrigin - a;
    time = ap.dot(n);
    if (time < 0.0f) 
		return false;

    //Compute barycentric coordinate components and test if within bounds
    XVector3D e = qp.cross(ap);
    baryV = ac.dot(e);
    if (baryV < 0.0f || baryV > d)
		return false;
    baryW = -ab.dot(e);
    if (baryW < 0.0f || baryV + baryW > d) 
		return false;

    return true;
}

//*****************************************************************************
// Input : c1,c2    - sphere centers
//         r1,r2    - sphere radii
// Notes : 
// Return: true/false
//*****************************************************************************
XBOOL XISphereSphere(XVector3D& sphereCenter1,XF32 radius1,XVector3D& sphereCenter2,
					 XF32 radius2)
{
	XF32 dx = sphereCenter1.x - sphereCenter2.x;
	XF32 dy = sphereCenter1.y - sphereCenter2.y;
	XF32 dz = sphereCenter1.z - sphereCenter2.z;
	XF32 dist = (XF32)sqrt(dx*dx + dy*dy + dz*dz);
	
	if (dist <= (radius1 + radius2))
		return true;
	else
		return false;
}



//*****************************************************************************
//*****************************************************************************
X_INTERSECTION_TYPE XISpherePlane(XVector3D& sphereCenter,XF32 sphereRadius,
								  XVector3D& planePoint,XVector3D& planeNormal)
{
    //For a normalized plane (|p.n| = 1), evaluating the plane equation
    //for a point gives the signed distance of the point to the plane
    XF32 dist = sphereCenter.dot(planeNormal) - (-(planeNormal.dot(planePoint)));

    // If sphere center within +/-radius from plane, plane intersects sphere
	if (dist < -sphereRadius)
		return X_IT_INSIDE;
	else if (dist > sphereRadius)
		return X_IT_OUTSIDE;
	else 
		return X_IT_INTERSECTS;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XISphereAABB(XVector3D& sphereCenter,XF32 sphereRadius,XVector3D& min,XVector3D& max)
{
    //Compute squared distance between sphere center and AABB
    XF32 sqDist = XIPointAABBDistanceSquared(sphereCenter,min,max);

    //Sphere and AABB intersect if the (squared) distance
    //between them is less than the (squared) sphere radius
    return sqDist <= sphereRadius * sphereRadius;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XISphereTriangle(XVector3D& sphereCenter,XF32 sphereRadius,XVector3D& a,XVector3D& b,
					   XVector3D& c,XVector3D& closest)
{
    //Find point P on triangle ABC closest to sphere center
    XIClosestPointOnTriangle(a,b,c,sphereCenter,closest);

    //Sphere and triangle intersect if the (squared) distance from sphere
    //center to point p is less than the (squared) sphere radius
    XVector3D v = closest - sphereCenter;
    return v.dot(v) <= sphereRadius * sphereRadius;
}



//*****************************************************************************
// Input : min,max    - AABB extents
//         a,b,c      - triangle vertices
// Notes : 
// Return: true/false
//*****************************************************************************
XS32 XIAABBTriangle(XVector3D& min,XVector3D& max,XVector3D& a,XVector3D& b,XVector3D& c)
{
	XVector3D trans;
	XVector3D scale;
	XVector3D transMax;	
	XVector3D newA = a,newB = b,newC = c;

	//Compute the scale and transform required to make BBox
	//a voxel
	trans.x = (max.x + min.x) * .5f;
	trans.y = (max.y + min.y) * .5f;
	trans.z = (max.z + min.z) * .5f;

	transMax = max - trans;
	
	if(transMax.x != 0)
		scale.x = 0.5f / transMax.x;
	if(transMax.y != 0)
		scale.y = 0.5f / transMax.y;
	if(transMax.z != 0)
		scale.z = 0.5f / transMax.z;

	//Put the triangle in voxel space
	newA.x = (newA.x - trans.x) * scale.x;
	newA.y = (newA.y - trans.y) * scale.y;
	newA.z = (newA.z - trans.z) * scale.z;
	
	newB.x = (newB.x - trans.x) * scale.x;
	newB.y = (newB.y - trans.y) * scale.y;
	newB.z = (newB.z - trans.z) * scale.z;

	newC.x = (newC.x - trans.x) * scale.x;
	newC.y = (newC.y - trans.y) * scale.y;
	newC.z = (newC.z - trans.z) * scale.z;

	//Test against the voxel
	return (XITriangleCube(newA,newB,newC));	
}



//*****************************************************************************
//*****************************************************************************
XBOOL XIPointInAABB(XVector3D& p,XVector3D& min,XVector3D& max)
{
	if(p.x <= max.x && p.y <= max.y && p.z <= max.z &&
	   p.x >= min.x && p.y >= min.y && p.z >= min.z)
		return true;
	else
		return false;
}



//*****************************************************************************
//counter clockwise tri
//*****************************************************************************
XBOOL XIPointInTriangle(XVector3D& point,XVector3D& a,XVector3D& b,XVector3D& c)
{  
	XF32 total_angles = 0.0f;
       
	//make the 3 vectors
	XVector3D v1 = point - a;
	XVector3D v2 = point - b;
	XVector3D v3 = point - c;

	v1.normalize();
	v2.normalize();
	v3.normalize();

	XF32 temp = v1.dot(v2);
	temp = acos(temp);
	total_angles += temp;
  
	temp = v2.dot(v3);
	temp = acos(temp);
	total_angles += temp;

	temp = v3.dot(v1); 
	temp = acosf(temp);
	total_angles += temp;

	if (fabs(total_angles - (2*X_MATH_PI)) <= X_MATH_EPSILON)
		return true;
	  	     
	return false;
}



//*****************************************************************************
//*****************************************************************************
void XIClosestPointOnTriangle(XVector3D& a,XVector3D& b,XVector3D& c,XVector3D& p,
							 XVector3D& closest)
{
    // Check if P in vertex region outside A
    XVector3D ab = b - a;
    XVector3D ac = c - a;
    XVector3D ap = p - a;
    XF32 d1 = ab.dot(ap);
    XF32 d2 = ac.dot(ap);
    if (d1 <= 0.0f && d2 <= 0.0f) 
	{
		closest = a; // barycentric coordinates (1,0,0)
		return;
	}

    //Check if P in vertex region outside B
    XVector3D bp = p - b;
    XF32 d3 = ab.dot(bp);
    XF32 d4 = ac.dot(bp);
    if (d3 >= 0.0f && d4 <= d3) 
	{
		closest = b; // barycentric coordinates (0,1,0)
		return;
	}

    //Check if P in edge region of AB, if so return projection of P onto AB
    XF32 vc = d1*d4 - d3*d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) 
	{
        XF32 v = d1 / (d1 - d3);
        closest = a + v * ab; // barycentric coordinates (1-v,v,0)
		return;
    }

    //Check if P in vertex region outside C
    XVector3D cp = p - c;
    XF32 d5 = ab.dot(cp);
    XF32 d6 = ac.dot(cp);
    if (d6 >= 0.0f && d5 <= d6) 
	{
		closest = c; // barycentric coordinates (0,0,1)
		return;
	}

    //Check if P in edge region of AC, if so return projection of P onto AC
    XF32 vb = d5*d2 - d1*d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) 
	{
        XF32 w = d2 / (d2 - d6);
        closest = a + w * ac; // barycentric coordinates (1-w,0,w)
		return;
    }

    //Check if P in edge region of BC, if so return projection of P onto BC
    XF32 va = d3*d6 - d5*d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
        XF32 w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        closest = b + w * (c - b); // barycentric coordinates (0,1-w,w)
		return;
    }

    //P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    XF32 denom = 1.0f / (va + vb + vc);
    XF32 v = vb * denom;
    XF32 w = vc * denom;
    closest = a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
}



//*****************************************************************************
// Input : a - first end of line segment
//         b - second end of line segment
//         p - point we wish to find closest point on line from 
// Notes : Helper function for XGetClosestPointOnTriangle()
// Return: closest point on line segment
//*****************************************************************************
void XIClosestPointOnLine(XVector3D& a,XVector3D& b,XVector3D& p,XVector3D& closest,XF32& time)
{
    XVector3D ab = b - a;
    
	// Project p onto ab, computing parameterized position d(t) = a + t*(b – a)
	time = (p - a).dot(ab) / ab.dot(ab);
    
	//If outside segment, clamp time (and therefore closest point) to the closest endpoint
    if (time < 0.0f) 
		time = 0.0f;
    if (time > 1.0f) 
		time = 1.0f;
    
	//Compute projected position from the clamped t
    closest = a + (time * ab);
}



//*****************************************************************************
// Input : a - first end of line segment
//         b - second end of line segment
//         p - point we wish to find closest point on line from 
// Notes : Helper function for XGetClosestPointOnTriangle()
// Return: closest point on line segment
//*****************************************************************************
void XIClosestPointOnRay(XVector3D& rayOrigin,XVector3D& rayNormal,XVector3D& p,
						 XVector3D& closest,XF32& time)
{   
	// Project p onto rayNormal, computing parameterized position d(t) = a + t*(b – a)
	time = (p - rayOrigin).dot(rayNormal) / rayNormal.dot(rayNormal);
    
	//If outside segment, clamp time (and therefore closest point) to the closest endpoint
    if (time < 0.0f) 
		time = 0.0f;
    
	//Compute projected position from the clamped t
    closest = rayOrigin + (time * rayNormal);
}



//*****************************************************************************
//*****************************************************************************
void XIClosestPointOnPlane(XVector3D& planePoint,XVector3D& planeNormal,XVector3D& p,
						   XVector3D& closest)
{
	XF32 t = planeNormal.dot(p) - (-(planeNormal.dot(planePoint)));
    closest = p - (t * planeNormal);
}



//*****************************************************************************
//*****************************************************************************
void XIClosestPointOnAABB(XVector3D& min,XVector3D& max,XVector3D& p,XVector3D& closest)
{
    //For each coordinate axis, if the point coordinate value is
    //outside box,clamp it to the box, else keep it as is
	XF32 v;
    for (XU32 i = 0;i < 3;++i)
	{
        v = p.array[i];

        // v = max(v, b.min[i])
		if (v < min.array[i]) 
			v = min.array[i]; 

        // v = min(v, b.max[i])
		if (v > max.array[i]) 
			v = max.array[i]; 

        closest.array[i] = v;
    }
}



//*****************************************************************************
// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
// distance between between S1(s) and S2(t)
//*****************************************************************************
XF32 XIClosestPointBetweenTwoLines(XVector3D& a1,XVector3D& b1,XVector3D& a2,XVector3D& b2,
                              XF32& time1,XF32& time2,XVector3D& closest1,XVector3D& closest2)
{
    XVector3D d1 = b1 - a1; // Direction vector of segment S1
    XVector3D d2 = b2 - a2; // Direction vector of segment S2
    XVector3D r = a1 - a2;
    XF32 a = d1.dot(d1); // Squared length of segment S1, always nonnegative
    XF32 e = d2.dot(d2); // Squared length of segment S2, always nonnegative
    XF32 f = d2.dot(r);

    // Check if either or both segments degenerate into points
    if (a <= X_MATH_EPSILON && e <= X_MATH_EPSILON)
	{
        //Both segments degenerate into points
        time1 = time2 = 0.0f;
        closest1 = a1;
        closest2 = a2;
        return (closest1 - closest2).dot(closest1 - closest2);
    }
    if (a <= X_MATH_EPSILON) 
	{
        //First segment degenerates into a point
        time1 = 0.0f;
        time2 = f / e; // time1 = 0 => time2 = (b*time1 + f) / e = f / e
        X_MATH_CLAMP(time2, 0.0f, 1.0f);
    } 
	else
	{
        XF32 c = d1.dot(r);
        if (e <= X_MATH_EPSILON)
		{
            //Second segment degenerates into a point
            time2 = 0.0f;
			time1 = -c / a;
            X_MATH_CLAMP(time1, 0.0f, 1.0f); // time2 = 0 => time1 = (b*time2 - c) / a = -c / a
        } 
		else
		{
            //The general nondegenerate case starts here
			XF32 b = d1.dot(d2);
            XF32 denom = a*e-b*b; // Always nonnegative

            // If segments not parallel, compute closest point on L1 to L2, and
            // clamp to segment S1. Else pick arbitrary time1 (here 0)
            if (denom != 0.0f)
			{
				time1 = (b*f - c*e) / denom;
                X_MATH_CLAMP(time1, 0.0f, 1.0f);
			}
			else 
				time1 = 0.0f;

            // Compute point on L2 closest to S1(time1) using
            // time2 = Dot((P1+D1*time1)-P2,D2) / Dot(D2,D2) = (b*time1 + f) / e
            time2 = (b*time1 + f) / e;

            // If time2 in [0,1] done. Else clamp time2, recompute time1 for the new value
            // of time2 using time1 = Dot((P2+D2*time2)-P1,D1) / Dot(D1,D1)= (time2*b - c) / a
            // and clamp time1 to [0, 1]
            if (time2 < 0.0f)
			{
                time2 = 0.0f;
				time1 = -c / a;
                X_MATH_CLAMP(time1, 0.0f, 1.0f);
            } 
			else if (time2 > 1.0f) 
			{
                time2 = 1.0f;
				time1 = (b - c) / a;
                X_MATH_CLAMP(time1, 0.0f, 1.0f);
            }
        }
    }

    closest1 = a1 + d1 * time1;
    closest2 = a2 + d2 * time2;
    return (closest1 - closest2).dot(closest1 - closest2);
}



//*****************************************************************************
//*****************************************************************************
XBOOL XIPointOnLine(XVector3D& a, XVector3D& b, XVector3D& p,XF32 lineHalfWidth)
{
	XVector3D closest;
	XF32 time;
	XIClosestPointOnLine(a,b,p,closest,time);
	if((closest - p).mag() < lineHalfWidth)
		return true;
	else
		return false;
}



//*****************************************************************************
//*****************************************************************************
XF32 XIPointLineDistanceSquared(XVector3D& a,XVector3D& b,XVector3D& p)
{
    XVector3D ab = b - a, ap = p - a, bp = p - b;
    XF32 e = ap.dot(ab);
    
	//Handle cases where p projects outside ab
    if (e <= 0.0f) 
		return ap.dot(ap);

    XF32 f = ab.dot(ab);
    if (e >= f) 
		return bp.dot(bp);
    
	//Handle case where c projects onto ab
    return ap.dot(ap) - (e * e) / f;
}



//*****************************************************************************
//*****************************************************************************
XF32 XIPointPlaneDistance(XVector3D& planePoint,XVector3D& planeNormal,XVector3D& p)
{
    return p.dot(planeNormal) - (-(planeNormal.dot(planePoint))); 
}



//*****************************************************************************
//*****************************************************************************
XF32 XIPointAABBDistanceSquared(XVector3D& min,XVector3D& max,XVector3D& p)
{
    XF32 sqDist = 0.0f;
	XF32 v;
    for (XU32 i = 0;i < 3;++i) 
	{
        //For each axis count any excess distance outside box extents
        v = p.array[i];
        if (v < min.array[i]) 
			sqDist += (min.array[i] - v) * (min.array[i] - v);
        if (v > max.array[i]) 
			sqDist += (v - max.array[i]) * (v - max.array[i]);
    }
    return sqDist;
}



//*****************************************************************************
// Input : a - first vertex in triangle
//         b - second vertex in triangle 
//         c - third vertex in triangle
//         p - point we wish to find closest point on triangle from 
// Notes : 
// Return: closest point on line triangle edge
//*****************************************************************************
/*XVector3D XIClosestPointOnTriangle(XVector3D a, XVector3D b, XVector3D c, XVector3D p)
{	
	XVector3D Rab = XIClosestPointOnLine(a, b, p);
	XVector3D Rbc = XIClosestPointOnLine(b, c, p);
	XVector3D Rca = XIClosestPointOnLine(c, a, p);
			
	XF64 dAB = (p-Rab).mag();
	XF64 dBC = (p-Rbc).mag();
	XF64 dCA = (p-Rca).mag();
	  
	XF64 min = dAB;
	XVector3D result = Rab;
	  
	if (dBC < min) {
		min = dBC;
		result = Rbc;
		}
	 
	if (dCA < min)
	result = Rca;
	     
	return (result);	
}*/



//*****************************************************************************
// Input : point - point we wish to check for inclusion
//         sO - Origin of sphere
//         sR - radius of sphere 
// Notes : 
// Return: true if point is in sphere, false if not.
//*****************************************************************************
XBOOL XIPointInSphere(XVector3D& p,XVector3D& sphereCenter,XF32 sphereRadius)
{	
	if(((p - sphereCenter).mag()) <= sphereRadius)
		return true;
	else
		return false;	
}



//*****************************************************************************
// Input : point - point we wish to compute normal at 
//         eO - Origin of ellipsoid
//         eR - radius vector of ellipsoid 
// Notes : 
// Return: a unit normal vector to the tangent plane of the ellipsoid in the point.
//*****************************************************************************
XVector3D XITangentPlaneNormalOfEllipsoid(XVector3D point, XVector3D eO, XVector3D eR)
{
	XVector3D p = point - eO;

	XF64 a2 = eR.x * eR.x;
	XF64 b2 = eR.y * eR.y;
	XF64 c2 = eR.z * eR.z;

	XVector3D res;
	res.x = p.x / (XF32)a2;
	res.y = p.y / (XF32)b2;
	res.z = p.z / (XF32)c2;

	res.normalize();	
	return (res);	
}



//*****************************************************************************
//Which of the six face-plane(s) is point P outside of? 
//*****************************************************************************
XS32 XIFacePlane(XVector3D& p)
{
	XS32 outcode;

	outcode = 0;
	if (p.x >  .5f) outcode |= 0x01;
	if (p.x < -.5f) outcode |= 0x02;
	if (p.y >  .5f) outcode |= 0x04;
	if (p.y < -.5f) outcode |= 0x08;
	if (p.z >  .5f) outcode |= 0x10;
	if (p.z < -.5f) outcode |= 0x20;
   
	return(outcode);
}



//*****************************************************************************
//Which of the twelve edge plane(s) is point P outside of? 
//*****************************************************************************
XS32 XIBevel2D(XVector3D& p)
{	
	XS32 outcode;

	outcode = 0;
	if ( p.x + p.y > 1.0f) outcode |= 0x001;
	if ( p.x - p.y > 1.0f) outcode |= 0x002;
	if (-p.x + p.y > 1.0f) outcode |= 0x004;
	if (-p.x - p.y > 1.0f) outcode |= 0x008;
	if ( p.x + p.z > 1.0f) outcode |= 0x010;
	if ( p.x - p.z > 1.0f) outcode |= 0x020;
	if (-p.x + p.z > 1.0f) outcode |= 0x040;
	if (-p.x - p.z > 1.0f) outcode |= 0x080;
	if ( p.y + p.z > 1.0f) outcode |= 0x100;
	if ( p.y - p.z > 1.0f) outcode |= 0x200;
	if (-p.y + p.z > 1.0f) outcode |= 0x400;
	if (-p.y - p.z > 1.0f) outcode |= 0x800;

	return(outcode);
}



//*****************************************************************************
//Which of the eight corner plane(s) is point P outside of? 
//*****************************************************************************
XS32 XIBevel3D(XVector3D& p)
{
	XS32 outcode;

	outcode = 0;
	if (( p.x + p.y + p.z) > 1.5f) outcode |= 0x01;
	if (( p.x + p.y - p.z) > 1.5f) outcode |= 0x02;
	if (( p.x - p.y + p.z) > 1.5f) outcode |= 0x04;
	if (( p.x - p.y - p.z) > 1.5f) outcode |= 0x08;
	if ((-p.x + p.y + p.z) > 1.5f) outcode |= 0x10;
	if ((-p.x + p.y - p.z) > 1.5f) outcode |= 0x20;
	if ((-p.x - p.y + p.z) > 1.5f) outcode |= 0x40;
	if ((-p.x - p.y - p.z) > 1.5f) outcode |= 0x80;
	return(outcode);
}

//*****************************************************************************
//Test the point "alpha" of the way from P1 to P2.  See if it is on a face
//of the cube.  Consider only faces in "mask"                   
//*****************************************************************************
XS32 XICheckPoint(XVector3D& p1,XVector3D& p2,XF32 alpha,XS32 mask)
{
	XVector3D plane_point;
	plane_point.x = ((p1.x)+(alpha)*((p2.x)-(p1.x))); 
	plane_point.y = ((p1.y)+(alpha)*((p2.y)-(p1.y)));
	plane_point.z = ((p1.z)+(alpha)*((p2.z)-(p1.z)));

	return(XIFacePlane(plane_point) & mask);
}



//*****************************************************************************
//Compute intersection of P1 --> P2 line segment with face planes 
//Then test intersection point to see if it is on cube face       
//Consider only face planes in "outcode_diff"                     
//Note: Zero bits in "outcode_diff" means face line is outside of */
//*****************************************************************************
XS32 XICheckLine(XVector3D& p1,XVector3D& p2,XS32 outcode_diff)
{
   if ((0x01 & outcode_diff) != 0)
      if (XICheckPoint(p1,p2,( .5f-p1.x)/(p2.x-p1.x),0x3e) == 0) 
		  return(0);
   if ((0x02 & outcode_diff) != 0)
      if (XICheckPoint(p1,p2,(-.5f-p1.x)/(p2.x-p1.x),0x3d) == 0) 
		  return(0);
   if ((0x04 & outcode_diff) != 0) 
      if (XICheckPoint(p1,p2,( .5f-p1.y)/(p2.y-p1.y),0x3b) == 0) 
		  return(0);
   if ((0x08 & outcode_diff) != 0) 
      if (XICheckPoint(p1,p2,(-.5f-p1.y)/(p2.y-p1.y),0x37) == 0) 
		  return(0);
   if ((0x10 & outcode_diff) != 0) 
      if (XICheckPoint(p1,p2,( .5f-p1.z)/(p2.z-p1.z),0x2f) == 0) 
		  return(0);
   if ((0x20 & outcode_diff) != 0) 
      if (XICheckPoint(p1,p2,(-.5f-p1.z)/(p2.z-p1.z),0x1f) == 0) 
		  return(0);
      
   return(1);
}



//*****************************************************************************
//Triangle t is compared with a unit cube,  
//centered on the origin.                    
//It returns INSIDE (0) or OUTSIDE(1) if t   
//Intersects or does not intersect the cube.
//*****************************************************************************
XS32 XITriangleCube(XVector3D& a,XVector3D& b,XVector3D& c)
{	
	XS32 v1_test,v2_test,v3_test;
	XF32 d;
	XVector3D vect12,vect13,norm;
	XVector3D hitpp,hitpn,hitnp,hitnn;

	//First compare all three vertexes with all six face-planes 
	//If any vertex is inside the cube, return immediately!     
	if ((v1_test = XIFacePlane(a)) == 0) 
		return(0);
	if ((v2_test = XIFacePlane(b)) == 0) 
		return(0);
	if ((v3_test = XIFacePlane(c)) == 0) 
		return(0);

	//If all three vertexes were outside of one or more face-planes,
	//return immediately with a trivial rejection!              
	if ((v1_test & v2_test & v3_test) != 0) 
		return(1);

	//Now do the same trivial rejection test for the 12 edge planes 
	v1_test |= XIBevel2D(a) << 8; 
	v2_test |= XIBevel2D(b) << 8; 
	v3_test |= XIBevel2D(c) << 8;
	if ((v1_test & v2_test & v3_test) != 0) 
		return(1);  

	//Now do the same trivial rejection test for the 8 corner planes
	v1_test |= XIBevel3D(a) << 24; 
	v2_test |= XIBevel3D(b) << 24; 
	v3_test |= XIBevel3D(c) << 24; 
	if ((v1_test & v2_test & v3_test) != 0) 
		return(1);   

	//If vertex 1 and 2, as a pair, cannot be trivially rejected 
	//by the above tests, then see if the v1-->v2 triangle edge  
	//intersects the cube.  Do the same for v1-->v3 and v2-->v3. 
	//Pass to the intersection algorithm the "OR" of the outcode 
	//bits, so that only those cube faces which are spanned by   
	//each triangle edge need be tested.                         
	if ((v1_test & v2_test) == 0)
		if (XICheckLine(a,b,v1_test|v2_test) == 0) 
			return(0);
	if ((v1_test & v3_test) == 0)
		if (XICheckLine(a,c,v1_test|v3_test) == 0)
			return(0);
	if ((v2_test & v3_test) == 0)
		if (XICheckLine(b,c,v2_test|v3_test) == 0) 
			return(0);

	//By now, we know that the triangle is not off to any side,     
	//and that its sides do not penetrate the cube.  We must now   
	//test for the cube intersecting the interior of the triangle. 
	//We do this by looking for intersections between the cube     
	//diagonals and the triangle...first finding the intersection  
	//of the four diagonals with the plane of the triangle, and    
	//then if that intersection is inside the cube, pursuing       
	//whether the intersection point is inside the triangle itself. 

	//To find plane of the triangle, first perform crossproduct on  
	//two triangle side vectors to compute the normal vector.  
	vect12 = a - b;
	vect13 = a - c;
	norm = vect12.cross(vect13);

	//The normal vector "norm" X,Y,Z components are the coefficients 
	//of the triangles AX + BY + CZ + D = 0 plane equation.  If we   
	//solve the plane equation for X=Y=Z (a diagonal), we get        
	//-D/(A+B+C) as a metric of the distance from cube center to the 
	//diagonal/plane intersection.  If this is between -0.5 and 0.5, 
	//the intersection is inside the cube.  If so, we continue by    
	//doing a point/triangle intersection.                           
	//Do this for all four diagonals.                                
	d = norm.x * a.x + norm.y * a.y + norm.z * a.z;
	hitpp.x = hitpp.y = hitpp.z = d / (norm.x + norm.y + norm.z);
	if (fabs(hitpp.x) <= 0.5f)
		if (!XIPointInTriangle(hitpp,a,b,c))
			return(0);
	hitpn.z = -(hitpn.x = hitpn.y = d / (norm.x + norm.y - norm.z));
	if (fabs(hitpn.x) <= 0.5f)
		if (!XIPointInTriangle(hitpn,a,b,c)) 
			return(0);
	hitnp.y = -(hitnp.x = hitnp.z = d / (norm.x - norm.y + norm.z));
	if (fabs(hitnp.x) <= 0.5f)
		if (!XIPointInTriangle(hitnp,a,b,c)) 
			return(0);
	hitnn.y = hitnn.z = -(hitnn.x = d / (norm.x - norm.y - norm.z));
	if (fabs(hitnn.x) <= 0.5f)
		if (!XIPointInTriangle(hitnn,a,b,c)) 
			return(0);

	//No edge touched the cube; no cube diagonal touched the triangle. 
	//We're done...there was no intersection.                          
	return(1);
}




//*****************************************************************************
//*****************************************************************************
XBOOL XILinePlane(XVector3D& lineStart,XVector3D& lineEnd,XVector3D& planePoint,
						  XVector3D& planeNormal,XVector3D& pointOfIntersection,XF32& time)
{
	//Compute time of intersection
	XVector3D lineVec = lineEnd - lineStart;
	XF32 lineLen = planeNormal.dot(lineVec);

	//If plane and line are parallel there is no valid intersection point.
	if (X_MATH_IS_XF32_ZERO(lineLen))  
		return false;

	time = ((planeNormal.dot(planePoint)) - planeNormal.dot(lineStart)) / lineLen;

	//An intersection only occurs between 0 and 1 time interval
	if (time >= 0.0f && time <= 1.0f)
	{
		pointOfIntersection = lineStart + (time * lineVec);
		return true;
	}
	else
		return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XILineSphere(XVector3D& lineStart,XVector3D& lineEnd,XVector3D& sphereCenter,
						  XF32 sphereRadius,XVector3D& pointOfIntersection,XF32& time)
{	
	XVector3D lineDir = lineEnd - lineStart;
	XF32 lineLen = lineDir.mag();
	lineDir = lineDir.normalize();

	XVector3D m = lineStart - sphereCenter;
	XF32 b = m.dot(lineDir);
	XF32 c = m.dot(m) - (sphereRadius * sphereRadius);

	if (c > 0.0f && b > 0.0f)
		return false;

	XF32 discriminant = (b * b) - c; 

	//If discriminant is less than 0, ray misses sphere.
	if (discriminant < 0.0f)
		return false;

	//At this point we konw that the ray intersects the sphere, we just
	//need to find when and where.
	time = -b - sqrt(discriminant);

	//If time is > than the length of the line, the ray intersects but the
	//line does not.
	if (time > lineLen)
		return false;

	//Remember, if the time is negative, that means the line starts inside
	//the sphere so we should clamp time to 0.
	if (time < 0.0f)
		time = 0.0f;
	pointOfIntersection = lineStart + (time * lineDir);
	
	return true;
}




//*****************************************************************************
//*****************************************************************************
XBOOL XILineAABB(XVector3D& lineStart,XVector3D& lineEnd,XVector3D& min,
						 XVector3D& max)
{
    XVector3D c = (min + max) * 0.5f;				// Box center-point
    XVector3D e = max - c;							// Box halflength extents
    XVector3D m = (lineStart + lineEnd) * 0.5f;		// Segment midpoint
    XVector3D d = lineEnd - m;						// Segment halflength vector
    m = m - c;										// Translate box and segment to origin

    // Try world coordinate axes as separating axes
    XF32 adx = fabs(d.x);
    if (fabs(m.x) > e.x + adx)
		return false;
    XF32 ady = fabs(d.y);
    if (fabs(m.y) > e.y + ady) 
		return false;
    XF32 adz = fabs(d.z);
    if (fabs(m.z) > e.z + adz) 
		return false;

    //Add in an epsilon term to counteract arithmetic errors when segment is
    //(near) parallel to a coordinate axis
    adx += X_MATH_EPSILON; 
	ady += X_MATH_EPSILON; 
	adz += X_MATH_EPSILON;

    //Try cross products of segment direction vector with coordinate axes
    if (fabs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) 
		return false;
    if (fabs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) 
		return false;
    if (fabs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) 
		return false;

    // No separating axis found; segment must be overlapping AABB
    return true;
}


//*****************************************************************************
//*****************************************************************************
XBOOL XILineTriangle(XVector3D& lineStart,XVector3D& lineEnd,
							XVector3D& a,XVector3D& b,XVector3D& c,
							XF32& baryU,XF32 &baryV,XF32& baryW,XF32& time)
{
    XVector3D ab = b - a;
    XVector3D ac = c - a;
    XVector3D qp = lineStart - lineEnd;

    //Compute triangle normal. Can be precalculated or cached if
    //intersecting multiple segments against the same triangle
    XVector3D n = ab.cross(ac);

    //Compute denominator d. If d <= 0, segment is parallel to or points
    //away from triangle, so exit early
    XF32 d = qp.dot(n);
    if (d <= 0.0f)
		return false;

    //Compute intersection ttime value of pq with plane of triangle. A ray
    //intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
    //dividing by d until intersection has been found to pierce triangle
    XVector3D ap = lineStart - a;
    time = ap.dot(n);
    if (time < 0.0f || time > d) 
		return false;

    //Compute barycentric coordinate components and test if within bounds
    XVector3D e = qp.cross(ap);
    baryV = ac.dot(e);
    if (baryV < 0.0f || baryV > d)
		return false;
    baryW = -ab.dot(e);
    if (baryW < 0.0f || baryV + baryW > d) 
		return false;

    //Line intersects triangle. Perform delayed division and
    //compute the last barycentric coordinate component
    XF32 ood = 1.0f / d;
    time *= ood;
    baryV *= ood;
    baryW *= ood;
    baryU = 1.0f - baryV - baryW;
    return true;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XILineTriangleQuick(XVector3D& lineStart,XVector3D& lineEnd,
							XVector3D& a,XVector3D& b,XVector3D& c)
{
    XVector3D ab = b - a;
    XVector3D ac = c - a;
    XVector3D qp = lineStart - lineEnd;
	
	XF32 baryV,baryW;
	XF32 time;

    //Compute triangle normal. Can be precalculated or cached if
    //intersecting multiple segments against the same triangle
    XVector3D n = ab.cross(ac);

    //Compute denominator d. If d <= 0, segment is parallel to or points
    //away from triangle, so exit early
    XF32 d = qp.dot(n);
    if (d <= 0.0f)
		return false;

    //Compute intersection ttime value of pq with plane of triangle. A ray
    //intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
    //dividing by d until intersection has been found to pierce triangle
    XVector3D ap = lineStart - a;
    time = ap.dot(n);
    if (time < 0.0f || time > d) 
		return false;

    //Compute barycentric coordinate components and test if within bounds
    XVector3D e = qp.cross(ap);
    baryV = ac.dot(e);
    if (baryV < 0.0f || baryV > d)
		return false;
    baryW = -ab.dot(e);
    if (baryW < 0.0f || baryV + baryW > d) 
		return false;

	return true;
}

/*
//*****************************************************************************
//*****************************************************************************
XBOOL XIPointInTriangle(XVector3D point, XVector3D triangle[3])
{
	XVector3D Lines[3];
	XF32 total_angles;

	try{

		//loop through all the vertices in triangle
		for(XS32 i = 0; i < 3; i++)
		{
			//get direction Vector from intersection to vertex[a]
			Lines[i] = point - triangle[i]; 
			Lines[i].normalize();
		}

		XF64 ang1 = acos(Lines[0].dot(Lines[1]));
		XF64 ang2 = acos(Lines[1].dot(Lines[2]));
		XF64 ang3 = acos(Lines[2].dot(Lines[0]));

		if(ang1 < 0 || ang2 < 0 || ang3 < 0)
			return false;

		XF32 ang1deg = (XF32)X_MATH_RAD_TO_DEG(ang1);
		XF32 ang2deg = (XF32)X_MATH_RAD_TO_DEG(ang2);
		XF32 ang3deg = (XF32)X_MATH_RAD_TO_DEG(ang3);

		total_angles = (XF32)(ang1 + ang2 + ang3);

		//lets take 6.28 radians(360 degrees) away from total_angles
		//if its Zero (with tolerence) we've hit it
		if(fabsf(total_angles - X_MATH_2PI) < 0.00001f)
			return true; 
	}catch(...){
		//LogError("XIsPointInTriangle exception");
	}
	return false; //we have not hit the polygon
}
*/


//*****************************************************************************
//*****************************************************************************
XBOOL XIAABBAABB(XVector3D& min1,XVector3D& max1,XVector3D& min2,XVector3D& max2)
{
	if ((max1.x < min2.x) || (max1.y < min2.y) || (max1.z < min2.z) ||
		(min1.x > max2.x) || (min1.y > max2.y) || (min1.z > max2.z))
		return false;
	else
		return true;
}



//*****************************************************************************
//*****************************************************************************
X_INTERSECTION_TYPE XIAABBPlane(XVector3D& min,XVector3D& max,XVector3D& planePoint,XVector3D& planeNormal)
{
    XVector3D c = (max + min) * 0.5f; //Compute AABB center
    XVector3D e = max - c; //Compute positive extents

    //Compute the projection interval radius of box onto L(t) = b.c + t * p.n
    XF32 r = e.x*fabs(planeNormal.x) + e.y*fabs(planeNormal.y) + e.z*fabs(planeNormal.z);
    
	//Compute distance of box center from plane
    XF32 s = planeNormal.dot(c) - (-(planeNormal.dot(planePoint)));
    
	//Intersection occurs when distance s falls within [-r,+r] interval
	if (s < -r)
		return X_IT_INSIDE;
	else if (s > r)
		return X_IT_OUTSIDE;
	else 
		return X_IT_INTERSECTS;
}


//*****************************************************************************
// Test if quadrilateral (a, b, c, d) is convex
//*****************************************************************************
XBOOL XIConvexQuad(XVector3D& a,XVector3D& b,XVector3D& c,XVector3D& d)
{
    // uad is nonconvex if Dot(Cross(bd, ba), Cross(bd, bc)) >= 0
	XVector3D temp = d - b;
    XVector3D v1 = temp.cross(a - b);
    XVector3D v2 = temp.cross(c - b);

    if (v1.dot(v2) >= 0.0f)
		return false;

    //Quad is now convex iff Dot(Cross(ac, ad), Cross(ac, ab)) < 0
	temp = c - a;
    v1 = temp.cross(d - a);
    v2 = temp.cross(b - a);

    return v1.dot(v2) < 0.0f;
}


//*****************************************************************************
//*****************************************************************************
inline XF32 XITriArea2DProj(XF32 x1,XF32 y1,XF32 x2,XF32 y2,XF32 x3,XF32 y3)
{
    return (x1-x2)*(y2-y3) - (x2-x3)*(y1-y2);
}

//*****************************************************************************
//*****************************************************************************
XF32 XITriArea3D(XVector3D& a,XVector3D& b,XVector3D& c)
{
    return 0.5f * (b-a).cross(c-b).mag();
}

//*****************************************************************************
// Compute barycentric coordinates (u, v, w) for 
// point p with respect to triangle (a, b, c)
//*****************************************************************************
void XIComputeBaryCentric(XVector3D& point,XVector3D& a, XVector3D& b, XVector3D& c,
						  XF32& u,XF32& v,XF32& w)
{
    //Unnormalized triangle normal
    XVector3D m = (b - a).cross(c - a);
    
	//Nominators and one-over-denominator for u and v ratios
    XF32 nu, nv, ood;

    //Absolute components for determining projection plane
    XF32 x = fabs(m.x);
	XF32 y = fabs(m.y);
	XF32 z = fabs(m.z);

    //Compute areas in plane of largest projection
    if (x >= y && x >= z) 
	{
        //x is largest, project to the yz plane
        nu = XITriArea2DProj(point.y,point.z,b.y,b.z,c.y,c.z); // Area of PBC in yz plane
        nv = XITriArea2DProj(point.y,point.z,c.y,c.z,a.y,a.z); // Area of PCA in yz plane
        ood = 1.0f / m.x;                        // 1/(2*area of ABC in yz plane)
    } 
	else if (y >= x && y >= z) 
	{
        //y is largest, project to the xz plane
        nu = XITriArea2DProj(point.x,point.z,b.x,b.z,c.x,c.z);
        nv = XITriArea2DProj(point.x,point.z,c.x,c.z,a.x,a.z);
        ood = 1.0f / -m.y;
    } 
	else
	{
        //z is largest, project to the xy plane
        nu = XITriArea2DProj(point.x,point.y,b.x,b.y,c.x,c.y);
        nv = XITriArea2DProj(point.x,point.y,c.x,c.y,a.x,a.y);
        ood = 1.0f / m.z;
    }

    u = nu * ood;
    v = nv * ood;
    w = 1.0f - u - v;
}

/*XVector3D XI3Planes(XPlane& p0,XPlane& p1,XPlane& p2)
{
	XVector3D numerator = (p0.normal.dot(p0.refPoint) * (p1.normal.cross(p2.normal))) +
					 (p1.normal.dot(p1.refPoint) * (p2.normal.cross(p0.normal))) +
					 (p2.normal.dot(p2.refPoint) * (p0.normal.cross(p1.normal)));
	XF32 denominator = p0.normal.dot(p1.normal.cross((p2.normal)));

	return numerator/denominator;

}*/


//*****************************************************************************
//*****************************************************************************
XRay3D XICreatePickRay(XS32 vpX,XS32 vpY,XU32 vpWidth,XU32 vpHeight,
					 XMatrix4x4& viewMatrix,XMatrix4x4& projMatrix)
{
    //Compute the start and end points of a line.  
    XVector4D start,end;

	//Get points in clip space
    start.x =  (((2.0f * vpX) / (vpWidth)) - 1);
    start.y = -(((2.0f * vpY) / (vpHeight)) - 1);
    start.z =  -.99999f;
	start.w =  1.0f;
	end = start;
	end.z = .99999f;

    //Get the inverse view projection
    XMatrix4x4 invViewProj = viewMatrix * projMatrix;
	invViewProj.invert();

	//Take points to world space
	start = start * invViewProj;


	//Finish by undoing the w divide
	XF32 startW = 1.0f / start.w;
	start.x *= startW;
	start.y *= startW;
	start.z *= startW;
	start.w *= startW;

	end = end * invViewProj;
	XF32 endW = 1.0f / end.w;
	end.x *= endW;
	end.y *= endW;
	end.z *= endW;
	end.w *= endW;

	//Create a ray from the line segment in world space
	XVector4D vPickRayDir = end - start;
	vPickRayDir.normalize();

	return XRay3D(XVector3D(start),XVector3D(vPickRayDir));
}


//*****************************************************************************
//*****************************************************************************
void XICreatePickLine(XS32 vpX,XS32 vpY,XU32 vpWidth,XU32 vpHeight,
					 XMatrix4x4& viewMatrix,XMatrix4x4& projMatrix,XVector3D& lineStart,
					 XVector3D& lineEnd)
{
    //Compute the start and end points of a line.  
    XVector4D tempStart,tempEnd;

	//Get points in clip space
    tempStart.x =  (((2.0f * vpX) / (vpWidth)) - 1);
    tempStart.y = -(((2.0f * vpY) / (vpHeight)) - 1);
    tempStart.z =  -.99999f;
	tempStart.w =  1.0f;
	tempEnd = tempStart;
	tempEnd.z = .99999f;

    //Get the inverse view projection
    XMatrix4x4 invViewProj = viewMatrix * projMatrix;
	invViewProj.invert();

	//Take points to world space
	tempStart = tempStart * invViewProj;

	//Finish by undoing the w divide
	XF32 startW = 1.0f / tempStart.w;
	tempStart.x *= startW;
	tempStart.y *= startW;
	tempStart.z *= startW;
	tempStart.w *= startW;

	tempEnd = tempEnd * invViewProj;
	XF32 endW = 1.0f / tempEnd.w;
	tempEnd.x *= endW;
	tempEnd.y *= endW;
	tempEnd.z *= endW;
	tempEnd.w *= endW;

	//Set the line start and end 
	lineStart = tempStart;
	lineEnd = tempEnd;
}



//*****************************************************************************
//*****************************************************************************
XVector2D XITransformPointWorldToViewport(XVector3D& worldPoint,XU32 vpWidth,XU32 vpHeight,
										  XMatrix4x4& viewMatrix,XMatrix4x4& projMatrix)
{
	XVector4D orgPoint;
	orgPoint.x = worldPoint.x;
	orgPoint.y = worldPoint.y;
	orgPoint.z = worldPoint.z;
	orgPoint.w = 1.0f;
	
	XVector2D viewportPoint;
	
	//Push the point through view and proj
	orgPoint = orgPoint * viewMatrix * projMatrix;

	//Perspective divide
	X_ASSERTM(orgPoint.w != 0.0f,"Divide by zero");
	orgPoint /= orgPoint.w;

	viewportPoint.x = ((orgPoint.x * 0.5f) + 0.5f) * vpWidth;
	viewportPoint.y = ((orgPoint.y * 0.5f) + 0.5f) * vpHeight;
	viewportPoint.y = vpHeight - viewportPoint.y;

	return viewportPoint;
}