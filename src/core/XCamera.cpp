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
//	Module Name:	XCamera.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-16-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the camera.  The camera is initialized with the following
default values:<br><br>

<ul>
<li> Horizontal FOV = X_MATH_PI/4 </li>
<li> Near Clipping Plane = 1.0 </li> 
<li> Far Clipping Plane = 20000 </li> 
</ul>
*/
//*****************************************************************************
XCamera::XCamera()
{
	m_horizontalFOV = (X_MATH_PI/4.0f);
	m_nearClipPlane = 0.001f;
	m_farClipPlane = 10000;
	m_dirtyProj = true;
	m_projType = PERSPECTIVE;
	m_viewportWidth = 32;
	m_viewportHeight = 32;
	m_ID = 0;
	m_orthoZoom = 1.0f;
#ifndef XVP_FINAL
	m_pDebugScene = NULL;
#endif
	m_editorOrthoType = 1;
}



//*****************************************************************************
/**
Destructor for the camera.
*/
//*****************************************************************************
XCamera::~XCamera()
{
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XCamera::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_MODEL,X_SNID_CAMERA);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the camera.  This is the
description that appears in log files and other debug output.

@return A string description of the camera.
*/
//*****************************************************************************
XS8* XCamera::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XCamera";
	return description; 
}



//*****************************************************************************
/**
Initializes the camera.

@return	A status code of type XStatus.
*/
//*****************************************************************************
XStatus XCamera::init()
{
	XCamera();
	return X_OK;
}



XStatus XCamera::initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize)
{
	X_ASSERTM(pMemoryChunk,"Invalid memory chunk");

	XCamera();
	m_nearClipPlane = *(XF32*)pMemoryChunk;	pMemoryChunk += sizeof(XF32);
	m_farClipPlane = *(XF32*)pMemoryChunk;	pMemoryChunk += sizeof(XF32);
	m_horizontalFOV = *(XF32*)pMemoryChunk;	pMemoryChunk += sizeof(XF32);
	m_projType = (X_PROJECTION_TYPE)*(XU32*)pMemoryChunk;	pMemoryChunk += sizeof(XU32);
	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XStatus XCamera::destroy()
{
#ifndef XVP_FINAL
	//m_refModel.destroy();
#endif
	m_postEffects.clear();
	return X_OK;
}



//*****************************************************************************
/**
Gets the horizontal field of view (FOV) angle of the camera.

@return	The horizontal field of view (FOV) angle in radians.
*/
//*****************************************************************************
XF32 XCamera::getHorizontalFOV()
{
	return m_horizontalFOV;
}



//*****************************************************************************
/**
Sets the horizontal field of view (FOV) angle of the camera.

@param	angle	Horizontal field of view FOV angle in radians.
@return	None.
*/
//*****************************************************************************
void XCamera::setHorizontalFOV(XF32 angle)
{
	if (m_horizontalFOV == angle)
		return;

	m_horizontalFOV = angle;
	m_dirtyProj = true;
}



//*****************************************************************************
/**
Gets the aspect ratio of the camera.

@return	Aspect Ratio.  
*/
//*****************************************************************************
XF32 XCamera::getAspectRatio()
{
	return m_viewportWidth / (XF32)m_viewportHeight;
}



//*****************************************************************************
/**
Gets the near clipping plane's z depth.

@return	The z coordinate of the near clipping plane.  
*/
//*****************************************************************************
XF32 XCamera::getNearClipPlane()
{
	return m_nearClipPlane;
}



//*****************************************************************************
/**
Sets the near clipping plane's z depth.

@param	nearClipPlane	The z coordinate of the near clipping plane.  
@return	None.
*/
//*****************************************************************************
void XCamera::setNearClipPlane(XF32 nearClipPlane)
{
	X_ASSERTM(nearClipPlane != 0.0f,"Near clip plane cannot be 0");
	if (nearClipPlane == m_nearClipPlane)
		return;

	m_nearClipPlane = nearClipPlane;
	m_dirtyProj = true;
}



//*****************************************************************************
/**
Gets the far clipping plane's z depth.

@return	The z coordinate of the far clipping plane.  
*/
//*****************************************************************************
XF32 XCamera::getFarClipPlane()
{
	return m_farClipPlane;
}



//*****************************************************************************
/**
Sets the far clipping plane's z depth.

@param	farClipPlane	The z coordinate of the far clipping plane.  
@return	None.
*/
//*****************************************************************************
void XCamera::setFarClipPlane(XF32 farClipPlane)
{
	if (m_farClipPlane == farClipPlane)
		return;

	m_farClipPlane = farClipPlane;
	m_dirtyProj = true;
}


 
//*****************************************************************************
/**
Updates the camera.  X-VP calls this method to allow the camera to update any
time dependent data.  

@param	dt	The time, in seconds, that has passed since the last update.
@param	pMatrixStack	The scene graph's matrix stack.  This parameter will
						never be NULL.
@return	A status code of type XStatus.
*/
//*****************************************************************************
XStatus XCamera::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{	
	m_prevViewMatrix = m_viewMatrix;
	m_viewMatrix.createView(getWorldTransform());

	m_prevProjMatrix = m_projMatrix;

	if (m_projType == PERSPECTIVE)
	{
		//proj.setIdentity();
		m_projMatrix.createProjLH(m_nearClipPlane,
		 							 m_farClipPlane,
									 m_horizontalFOV,
									 (XF32)m_viewportWidth / (XF32)m_viewportHeight);
	}
	else 
	{  
		//Calculate a zoomed width and height trying to minimize floating point error 
		//while keeping the correct aspect ratio
		XF32 newWidth = ((XF32)m_viewportWidth * m_orthoZoom);
		XF32 newHeight = ((newWidth) / ((XF32)m_viewportWidth/(XF32)m_viewportHeight));
		newWidth = ((newHeight * ((XF32)m_viewportWidth/(XF32)m_viewportHeight)));

/*#ifdef XVP_EDITOR
		//For the editor, we just force the whole scene viewable regardless of camera
		//position or clipping planes.  Take the largest component of the position and
		//maximize it 
		m_projMatrix.setOrthoProjectionLH(.00001f,4000000.0f,newWidth,newHeight);
		XMatrix4x4 newTransform = m_pNodeArray[0]->getWorldTransform();
		switch (m_editorOrthoType)
		{
		case 1:
			newTransform._41 = -2000000.0f;
			break;
		case 2:
			newTransform._42 = 2000000.0f;
			break;
		case 3:
			newTransform._41 = 2000000.0f;
			break;
		case 4:
			newTransform._42 = -2000000.0f;
			break;
		case 5:
			newTransform._43 = 2000000.0f;
			break;
		case 6:
			newTransform._43 = -2000000.0f;
			break;
		}

		m_viewMatrix.createView(newTransform);
#else*/
		m_projMatrix.setOrthoProjectionLH(m_nearClipPlane,m_farClipPlane,newWidth,newHeight);
//#endif
	}

	//Update the viewing frustum
	extractFrustumPlanes(m_frustum);

	//Update Post Effects
	XMemBlockList<XPostEffect*>::iterator postEffectItr(m_postEffects.begin());
	while (postEffectItr.isValid())
	{
		postEffectItr.getValue()->prepForUpdate(dt,pScene);
		postEffectItr.getNext();
	}

	return X_OK;
}


void XCamera::setEditorOrthoType(XU8 orthoType)
{
	m_editorOrthoType = orthoType;
}

XMatrix4x4& XCamera::getPrevViewMatrix()
{
	
	return m_prevViewMatrix;
	//view.createView(m_pNodeArray[0]->getWorldTransform());
}

//*****************************************************************************
/**
Creates a 4x4 view matrix from the camera's current view.

@param	view	A variable to be set equal to the newly created view matrix.
@return	None.
*/
//*****************************************************************************
XMatrix4x4& XCamera::getViewMatrix()
{
	
	return m_viewMatrix;
	//view.createView(m_pNodeArray[0]->getWorldTransform());
}


XMatrix4x4& XCamera::getPrevProjMatrix()
{
	
	return m_prevProjMatrix;
	//view.createView(m_pNodeArray[0]->getWorldTransform());
}
//*****************************************************************************
/**
Creates a 4x4 projection matrix from the camera's current settings.

@param	proj	A variable to be set equal to the newly created projection matrix.
@return	None.
*/
//*****************************************************************************
XMatrix4x4& XCamera::getProjMatrix()
{
	//X_ASSERTM(m_viewportHeight > 0,"Viewport height is 0.  Set a non-zero viewport height.");
	//Can't have a viewport height of 0 so force it to 1 if that's the case.
	if (m_viewportHeight == 0)
		m_viewportHeight = 1;

	//if (m_dirtyProj)
	{
		

		m_dirtyProj = false;
	}

	return m_projMatrix;
}



//*****************************************************************************
//*****************************************************************************
XS32 XCamera::getID()
{
	return m_ID;
}
	


//*****************************************************************************
//*****************************************************************************
void XCamera::setID(XS32 ID)
{
	m_ID = ID;
}


	
//*****************************************************************************
/**
Extracts frustum planes from the camera's current view.

@param	frustum	A frustum object that will be set equal to the camera's
				frustum planes.
@return	None.
*/
//*****************************************************************************
void XCamera::extractFrustumPlanes(XViewingFrustum& frustum)
{
	//Pull out the frustum planes.
	XMatrix4x4 viewProj;
	viewProj = getViewMatrix() * getProjMatrix();
	XF32 t = 0;

	// Left clipping plane
	frustum.frustumPlanes[0].normal.x = viewProj._14 + viewProj._11;
	frustum.frustumPlanes[0].normal.y = viewProj._24 + viewProj._21;
	frustum.frustumPlanes[0].normal.z = viewProj._34 + viewProj._31;
	frustum.frustumPlanes[0].distance  = viewProj._44 + viewProj._41;
	t = frustum.frustumPlanes[0].normal.mag();
	frustum.frustumPlanes[0].distance  /= t;
	frustum.frustumPlanes[0].normal.normalize();

	// Right clipping plane
	frustum.frustumPlanes[1].normal.x = viewProj._14 - viewProj._11;
	frustum.frustumPlanes[1].normal.y = viewProj._24 - viewProj._21;
	frustum.frustumPlanes[1].normal.z = viewProj._34 - viewProj._31;
	frustum.frustumPlanes[1].distance  = viewProj._44 - viewProj._41;
	t = frustum.frustumPlanes[1].normal.mag();
	frustum.frustumPlanes[1].distance  /= t;
	frustum.frustumPlanes[1].normal.normalize();

	// Top clipping plane
	frustum.frustumPlanes[2].normal.x = viewProj._14 - viewProj._12;
	frustum.frustumPlanes[2].normal.y = viewProj._24 - viewProj._22;
	frustum.frustumPlanes[2].normal.z = viewProj._34 - viewProj._32;
	frustum.frustumPlanes[2].distance  = viewProj._44 - viewProj._42;
	t = frustum.frustumPlanes[2].normal.mag();
	frustum.frustumPlanes[2].distance  /= t;
	frustum.frustumPlanes[2].normal.normalize();
	
	// Bottom clipping plane
	frustum.frustumPlanes[3].normal.x = viewProj._14 + viewProj._12;
	frustum.frustumPlanes[3].normal.y = viewProj._24 + viewProj._22;
	frustum.frustumPlanes[3].normal.z = viewProj._34 + viewProj._32;
	frustum.frustumPlanes[3].distance  = viewProj._44 + viewProj._42;
	t = frustum.frustumPlanes[3].normal.mag();
	frustum.frustumPlanes[3].distance  /= t;
	frustum.frustumPlanes[3].normal.normalize();
	
	// Near clipping plane
	frustum.frustumPlanes[4].normal.x = viewProj._14 + viewProj._13;
	frustum.frustumPlanes[4].normal.y = viewProj._24 + viewProj._23;
	frustum.frustumPlanes[4].normal.z = viewProj._34 + viewProj._33;
	frustum.frustumPlanes[4].distance  = viewProj._44 + viewProj._43;
	t = frustum.frustumPlanes[4].normal.mag();
	frustum.frustumPlanes[4].distance  /= t;
	frustum.frustumPlanes[4].normal.normalize();
	
	// Far clipping plane
	frustum.frustumPlanes[5].normal.x = viewProj._14 - viewProj._13;
	frustum.frustumPlanes[5].normal.y = viewProj._24 - viewProj._23;
	frustum.frustumPlanes[5].normal.z = viewProj._34 - viewProj._33;
	frustum.frustumPlanes[5].distance  = viewProj._44 - viewProj._43;
	t = frustum.frustumPlanes[5].normal.mag();
	frustum.frustumPlanes[5].distance  /= t;
	frustum.frustumPlanes[5].normal.normalize();
}

	
	
//*****************************************************************************
/**
Determines if a point is inside of the camera's current viewing frustum.

@param	point	The point to check.
@return	True if the point is inside the viewing frustum, false otherwise.
*/
//*****************************************************************************
XBOOL XCamera::pointInFrustum(XVector3D point)
{
	if(m_frustum.frustumPlanes[0].normal.dot(point) <= 0)
		return false;
	if(m_frustum.frustumPlanes[1].normal.dot(point) <= 0)
		return false;
	if(m_frustum.frustumPlanes[2].normal.dot(point) <= 0)
		return false;
	if(m_frustum.frustumPlanes[3].normal.dot(point) <= 0)
		return false;
	if(m_frustum.frustumPlanes[4].normal.dot(point) <= 0)
		return false;
	if(m_frustum.frustumPlanes[5].normal.dot(point) <= 0)
		return false;

	return true;
}



//*****************************************************************************
/**
Determines if a sphere is completely inside, completely outside, or intersecting
with the camera's current viewing frustum.

@param	center	The center of the sphere in world coordinates.
@param	radius	The radius of the sphere.
@return	Intersection information about the sphere and the viewing frustum
		with a type from X_INTERSECTION_TYPE.
*/
//*****************************************************************************
X_INTERSECTION_TYPE XCamera::sphereInFrustum(XSphere& sphere)
{
   int c = 0;
   XF32 d;
   for(int p = 0; p < 6; ++p )
   {
      d = m_frustum.frustumPlanes[p].normal.dot(sphere.center) + m_frustum.frustumPlanes[p].distance;

      if(d <= -sphere.radius)
		return X_IT_OUTSIDE;

      if(d > sphere.radius)
         ++c;
   }

   return (c == 6) ? X_IT_INSIDE : X_IT_INTERSECTS;
}



//*****************************************************************************
/**
Determines if a cube is completely inside, completely outside, or intersecting
with the camera's current viewing frustum.

@param	center	The center of the cube in world coordinates.
@param	size	The size of the cube.  The size should be set to half of the
				cube's width/length/height.
@return	Intersection information about the cube and the viewing frustum
		with a type from X_INTERSECTION_TYPE.
*/
//*****************************************************************************
X_INTERSECTION_TYPE XCamera::cubeInFrustum(XVector3D center,XF32 size)
{
	XF32 x = center.x;
	XF32 y = center.y;
	XF32 z = center.z;
	int c  = 0;
	int c2 = 0;

	for(int p = 0;p < 6;++p)
	{
		c = 0;
		
		if(m_frustum.frustumPlanes[p].normal.x * (x - size) + m_frustum.frustumPlanes[p].normal.y * (y - size) + m_frustum.frustumPlanes[p].normal.z * (z - size) + m_frustum.frustumPlanes[p].distance > 0 )
			++c;
		if(m_frustum.frustumPlanes[p].normal.x * (x + size) + m_frustum.frustumPlanes[p].normal.y * (y - size) + m_frustum.frustumPlanes[p].normal.z * (z - size) + m_frustum.frustumPlanes[p].distance > 0 )
			++c;
		if(m_frustum.frustumPlanes[p].normal.x * (x - size) + m_frustum.frustumPlanes[p].normal.y * (y + size) + m_frustum.frustumPlanes[p].normal.z * (z - size) + m_frustum.frustumPlanes[p].distance > 0 )
			++c;
		if(m_frustum.frustumPlanes[p].normal.x * (x + size) + m_frustum.frustumPlanes[p].normal.y * (y + size) + m_frustum.frustumPlanes[p].normal.z * (z - size) + m_frustum.frustumPlanes[p].distance > 0 )
			++c;
		if(m_frustum.frustumPlanes[p].normal.x * (x - size) + m_frustum.frustumPlanes[p].normal.y * (y - size) + m_frustum.frustumPlanes[p].normal.z * (z + size) + m_frustum.frustumPlanes[p].distance > 0 )
			++c;
		if(m_frustum.frustumPlanes[p].normal.x * (x + size) + m_frustum.frustumPlanes[p].normal.y * (y - size) + m_frustum.frustumPlanes[p].normal.z * (z + size) + m_frustum.frustumPlanes[p].distance > 0 )
			++c;
		if(m_frustum.frustumPlanes[p].normal.x * (x - size) + m_frustum.frustumPlanes[p].normal.y * (y + size) + m_frustum.frustumPlanes[p].normal.z * (z + size) + m_frustum.frustumPlanes[p].distance > 0 )
			++c;
		if(m_frustum.frustumPlanes[p].normal.x * (x + size) + m_frustum.frustumPlanes[p].normal.y * (y + size) + m_frustum.frustumPlanes[p].normal.z * (z + size) + m_frustum.frustumPlanes[p].distance > 0 )
			++c;

      if(c == 0) 
		  return X_IT_OUTSIDE;

      if(c == 8) 
		  ++c2;
   }

   return (c2 == 6) ? X_IT_INSIDE : X_IT_INTERSECTS;
}



//*****************************************************************************
/**
Determines if an axis-aligned bounding box is completely inside, completely outside,
or intersecting with the camera's current viewing frustum.

@param	min	The mininum point of the AABB in world coordinates.  
@param	max	The maximum point of the AABB in world coordinates.
@return	Intersection information about the AABB and the viewing frustum
		with a type from X_INTERSECTION_TYPE.
*/
//*****************************************************************************
X_INTERSECTION_TYPE XCamera::AABBInFrustum(XVector3D min,XVector3D max)
{
	XVector3D Points[8];

	//if frustum not in worldspace, convert box points to viewspace
	Points[0] = min;
	Points[1] = XVector3D(max.x,min.y,min.z);
	Points[2] = XVector3D(min.x,max.y,min.z);
	Points[3] = XVector3D(max.x,max.y,min.z);
	Points[4] = XVector3D(min.x,min.y,max.z);
	Points[5] = XVector3D(max.x,min.y,max.z);
	Points[6] = XVector3D(min.x,max.y,max.z);
	Points[7] = max;

	int inside = 0;
	int cntr = 0;
	for(int i = 2; i < 6; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			XF32 dot = (m_frustum.frustumPlanes[i].normal.dot(Points[j]));
			if((dot >= m_frustum.frustumPlanes[i].distance))
				++inside;
		}

		if(inside == 0)
			return X_IT_OUTSIDE;

		if(inside == 8)
			++cntr;
		inside = 0;
	}

	if(cntr == 4)
		return X_IT_INSIDE;
	else
		return X_IT_INTERSECTS;
}



//*****************************************************************************
/**
Determines if a sphere intersects with the camera's current viewing frustum.
This method is much quicker compared to sphereInFrustum() but returns less
information.

@param	center	The center of the sphere in world coordinates.
@param	radius	The radius of the sphere.
@return	True if the sphere intersects the viewing frustum, false
		otherwise.
*/
//*****************************************************************************
XBOOL XCamera::sphereInFrustumQuick(XSphere& sphere)
{
	if((m_frustum.frustumPlanes[0].normal.dot(sphere.center) + m_frustum.frustumPlanes[0].distance) <= -sphere.radius)
		return false;
	if((m_frustum.frustumPlanes[1].normal.dot(sphere.center) + m_frustum.frustumPlanes[1].distance) <= -sphere.radius)
		return false;
	if((m_frustum.frustumPlanes[2].normal.dot(sphere.center) + m_frustum.frustumPlanes[2].distance) <= -sphere.radius)
		return false;
	if((m_frustum.frustumPlanes[3].normal.dot(sphere.center) + m_frustum.frustumPlanes[3].distance) <= -sphere.radius)
		return false;
	if((m_frustum.frustumPlanes[4].normal.dot(sphere.center) + m_frustum.frustumPlanes[4].distance) <= -sphere.radius)
		return false;
	if((m_frustum.frustumPlanes[5].normal.dot(sphere.center) + m_frustum.frustumPlanes[5].distance) <= -sphere.radius)
		return false;

	return true;
}



//*****************************************************************************
/**
Determines if a cube intersects with the camera's current viewing frustum.
This method is much quicker compared to cubeInFrustum() but returns less detailed
information.

@param	center	The center of the cube in world coordinates.
@param	size	The size of the cube.  The size should be set to half of the
				cube's width/length/height.
@return	True if the cube intersects the viewing frustum, false
		otherwise.
*/
//*****************************************************************************
XBOOL XCamera::cubeInFrustumQuick(XVector3D center,XF32 size)
{
	XF32 x = center.x;
	XF32 y = center.y;
	XF32 z = center.z;

	for(int p = 0; p < 6;++p)
	{
		if(m_frustum.frustumPlanes[p].normal.x * (x - size) + m_frustum.frustumPlanes[p].normal.y * (y - size) + m_frustum.frustumPlanes[p].normal.z * (z - size) + m_frustum.frustumPlanes[p].distance > 0 )
			continue;
		if(m_frustum.frustumPlanes[p].normal.x * (x + size) + m_frustum.frustumPlanes[p].normal.y * (y - size) + m_frustum.frustumPlanes[p].normal.z * (z - size) + m_frustum.frustumPlanes[p].distance > 0 )
			continue;
		if(m_frustum.frustumPlanes[p].normal.x * (x - size) + m_frustum.frustumPlanes[p].normal.y * (y + size) + m_frustum.frustumPlanes[p].normal.z * (z - size) + m_frustum.frustumPlanes[p].distance > 0 )
			continue;
		if(m_frustum.frustumPlanes[p].normal.x * (x + size) + m_frustum.frustumPlanes[p].normal.y * (y + size) + m_frustum.frustumPlanes[p].normal.z * (z - size) + m_frustum.frustumPlanes[p].distance > 0 )
			continue;
		if(m_frustum.frustumPlanes[p].normal.x * (x - size) + m_frustum.frustumPlanes[p].normal.y * (y - size) + m_frustum.frustumPlanes[p].normal.z * (z + size) + m_frustum.frustumPlanes[p].distance > 0 )
			continue;
		if(m_frustum.frustumPlanes[p].normal.x * (x + size) + m_frustum.frustumPlanes[p].normal.y * (y - size) + m_frustum.frustumPlanes[p].normal.z * (z + size) + m_frustum.frustumPlanes[p].distance > 0 )
			continue;
		if(m_frustum.frustumPlanes[p].normal.x * (x - size) + m_frustum.frustumPlanes[p].normal.y * (y + size) + m_frustum.frustumPlanes[p].normal.z * (z + size) + m_frustum.frustumPlanes[p].distance > 0 )
			continue;
		if(m_frustum.frustumPlanes[p].normal.x * (x + size) + m_frustum.frustumPlanes[p].normal.y * (y + size) + m_frustum.frustumPlanes[p].normal.z * (z + size) + m_frustum.frustumPlanes[p].distance > 0 )
			continue;

		return false;
	}

	return true;
}



X_PROJECTION_TYPE XCamera::getProjectionType()
{
	return m_projType;
}



//*****************************************************************************
//*****************************************************************************
void XCamera::setProjectionType(X_PROJECTION_TYPE type)
{
	m_projType = type;
	m_dirtyProj = true;
}



//*****************************************************************************
//*****************************************************************************
XF32 XCamera::getOrthoZoom()
{
	return m_orthoZoom;
}



//*****************************************************************************
//*****************************************************************************
void XCamera::setOrthoZoom(XF32 orthoZoom)
{
	if (m_orthoZoom == orthoZoom)
		return;
	else
		m_dirtyProj = true;

	//Keep above 0...
	if (orthoZoom < .00001f)
		m_orthoZoom = .00001f;
	else
		m_orthoZoom = orthoZoom;
}



//*****************************************************************************
//*****************************************************************************
XU32 XCamera::getViewportWidth()
{
	return m_viewportWidth;
}



//*****************************************************************************
//*****************************************************************************
void XCamera::setViewportWidth(XU32 viewportWidth)
{
	if (m_viewportWidth == viewportWidth)
		return;

	m_viewportWidth = viewportWidth;
	m_dirtyProj = true;
}



//*****************************************************************************
//*****************************************************************************
XU32 XCamera::getViewportHeight()
{
	return m_viewportHeight;
}



//*****************************************************************************
//*****************************************************************************
void XCamera::setViewportHeight(XU32 viewportHeight)
{
	if (m_viewportHeight == viewportHeight)
		return;

	m_viewportHeight = viewportHeight;
	m_dirtyProj = true;
}



//*****************************************************************************
/**
X-VP calls this method to allow the camera to perform any rendering actions.  

@return	A status code of type XStatus.
*/
//*****************************************************************************
XStatus XCamera::render()
{
	//m_nodeTree.render();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XCamera::handleViewportResize(XU32 width,XU32 height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	XMemBlockList<XPostEffect*>::iterator postEffectItr(m_postEffects.begin());
	while (postEffectItr.isValid())
	{
		postEffectItr.getValue()->handleResizeEvent(width,height);
		postEffectItr.getNext();
	}

	m_dirtyProj = true;

	return X_OK;
}

#ifndef XVP_FINAL
void XCamera::setDebugScene(XScene* pScene)
{
	/*if (m_pDebugScene)
	{
		if (m_pDebugScene->containsUpdateNode(&m_refModel))
		{
			m_pDebugScene->removeUpdateNode(&m_refModel);
			m_pDebugScene->removeRenderNode(&m_refModel);
		}
	}

	if (pScene)
	{
		pScene->addUpdateNode(&m_refModel);
		pScene->addRenderNode(&m_refModel);
	}*/
	
	m_pDebugScene = pScene;

	
}
#endif


XStatus XCamera::addPostEffect(XPostEffect* pPostEffect)
{

	return m_postEffects.insertBack(pPostEffect);
}


XStatus XCamera::removePostEffect(XPostEffect* pPostEffect)
{
	return m_postEffects.remove(pPostEffect);
}

XStatus XCamera::removeAllPostEffects()
{
	return m_postEffects.clear();
}

XBOOL XCamera::containsPostEffect(XPostEffect* pPostEffect)
{
	XMemBlockList<XPostEffect*>::iterator i(m_postEffects.begin());
	while (i.isValid())
	{
		if (i.getValue() == pPostEffect)
			return true;
		i.getNext();
	}

	return false;
}

XMemBlockList<XPostEffect*>& XCamera::getPostEffects()
{
	return m_postEffects;
}
/*
#ifndef XVP_FINAL
XStatus XCamera::onAddedToScene(XScene* pScene,XBOOL updateNode)
{
	if (m_pDebugScene)
	{
		m_pDebugScene->addUpdateNode(&m_refModel);
		m_pDebugScene->addRenderNode(&m_refModel);
	}
	
	return X_OK;
}
XStatus XCamera::onRemovedFromScene(XScene* pScene,XBOOL updateNode)
{
	if (m_pDebugScene)
	{
		m_pDebugScene->removeUpdateNode(&m_refModel);
		m_pDebugScene->removeRenderNode(&m_refModel);
	}
	return X_OK;
}
#endif
*/


XStatus XCamera::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("HorizontalFOV",setHorizontalFOV(value))
	PB_MAKE_PROPERTY("NearClipPlane",setNearClipPlane(value))
	PB_MAKE_PROPERTY("FarClipPlane",setFarClipPlane(value))
	
	return X_ERR_NO_SUCH_OBJECT;
}


XStatus XCamera::setEnumProperty(XS8* pPropertyName,XS32 value)
{
	PB_MAKE_PROPERTY("ProjectionType",setProjectionType((X_PROJECTION_TYPE)value))
	
	return X_ERR_NO_SUCH_OBJECT;
}