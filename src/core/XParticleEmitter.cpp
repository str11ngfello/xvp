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
//	Module Name:	XParticleEmitter.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//XU32 XParticleEmitter::m_maxVertexBufferSize = 300*6;
//XParticleVertex* XParticleEmitter::m_pParticleVertexBuffer = NULL;
XHandle XParticleEmitter::m_vertexDeclarationHandle = X_INVALID_HANDLE;



//*****************************************************************************
/**
Constructor for the bone.
*/
//*****************************************************************************
XParticleEmitter::XParticleEmitter()
{
	m_matHandle = X_INVALID_HANDLE;
	m_running = true;
	m_emitterType = X_ET_BOX;
	m_emitterLength = 0;
	m_emitterWidth = 0;
	m_emitterHeight = 0;
	m_birthRate = 0;
	m_particleSpeed = 0;
	m_particleSpeedVariation = 0;
	m_particleLife = 0;
	m_particleLifeVariation = 0;
	m_particleSize = 0;
	m_particleSizeVariation = 0;
	m_particleRotSpeed = 0.0f;
	m_particleRotSpeedVariation = 0.0f;
	m_directionVec = XVector3D(0,0,0);
	m_directionVecVariation = 0;
	m_particleResidue = 0;
	m_particleGrowTime = 0;
	m_particleShrinkTime = 0;
	m_numActiveParticles = 0;
	m_maxParticles = 0;
	m_pParticlesInUse = NULL;
	m_vbHandle = X_INVALID_HANDLE;
	m_oneShot = false;
	m_totalParticlesEmitted = 0;
}



//*****************************************************************************
/**
Destructor for the bone.
*/
//*****************************************************************************
XParticleEmitter::~XParticleEmitter()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleEmitter::destroy()
{
	//Destroy any remaining particles
	for (XU32 i = 0;i < m_numActiveParticles;++i)
		m_particleFreeList.freeItem(m_pParticlesInUse[i]);
	m_particleFreeList.destroy();

	XDeleteArray(m_pParticlesInUse);
	m_numActiveParticles = 0;

	XVP::GraphicsMgr.destroyVertexBuffer(m_vbHandle);
	m_vbHandle = X_INVALID_HANDLE;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XParticleEmitter::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_MODEL_OBJECT,X_SNID_PARTICLE_EMITTER);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the bone.  This is the
description that appears in log files and other debug output.

@return A string description of the bone.
*/
//*****************************************************************************
XS8* XParticleEmitter::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XParticleEmitter";
	return description; 
}



//*****************************************************************************
/**
Updates the bones world space transform and updates the skin transform used
during the skinning process.

@param dt	The amount of time, in seconds, that has elapsed since the last
					update.
@param pMatrixStack	The scene graph matrix stack.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XParticleEmitter::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)	
{
	//First, update the model.  Since the particle system uses the
	//local transform in the first node of the model tree, we want
	//to update it first before the code below trys to get the transform.
	//Also assign the render state from the first node to the model
	//so that the correct render state is used when adding a render client.
   // m_matHandle = m_nodeArray[0]->getMaterialHandle();	

	//Keep the direction vec translated by the final transform of the
	//particle system.  Do not include translation because this is
	//a direction vector...we don't want to translate it.
	XMatrix4x4 temp = getWorldTransform();
	temp._41 = temp._42 = temp._43 = 0;
	m_translatedDirectionVec = temp * m_directionVec;

	//Determine how many particles need to be created if the particle system is
	//running
	XU32 numParticlesToGenerate = 0;
	if (m_running)
	{
		if (m_oneShot)
		{
			if (m_totalParticlesEmitted == 0)
			{
				numParticlesToGenerate = (XU32)m_birthRate;
				m_particleResidue = 0.0f;
			}
		}
		else
		{
			XF32 particlesThisFrame = dt * m_birthRate;
			XF32 particlesThisFramePlusResidue = particlesThisFrame + m_particleResidue;
			numParticlesToGenerate = (XU32)(particlesThisFramePlusResidue);
			m_particleResidue = particlesThisFramePlusResidue - numParticlesToGenerate;
		}
		m_totalParticlesEmitted += numParticlesToGenerate;
	}

	
	XS32 index = m_numActiveParticles;
	XS32 indexMinus1  = index - 1;
	XS32 i = 0;
	XF32 distance;
	m_boundingSphere.radius = 0.0f;
	m_boundingSphere.center.x = m_worldTransform._41;
	m_boundingSphere.center.y = m_worldTransform._42;
	m_boundingSphere.center.z = m_worldTransform._43;

	for (i = 0;i < index;++i)
	{
		//If the particle is dead...
		if (!(m_pParticlesInUse[i]->update(dt)))
		{
			destroyParticle(m_pParticlesInUse[i]);

			//If there are more particles to process, grab one from the end of the list
			//and fill the hole created by destroying the previous particle
			if (i < (indexMinus1))
			{
				m_pParticlesInUse[i] = m_pParticlesInUse[indexMinus1];
				--i;
			}

			--index;
			--indexMinus1;
		}
		else
		{
			//Now check particle distance from emitter and update bounding sphere
			distance = m_pParticlesInUse[i]->m_position.distance(m_boundingSphere.center);
			if (distance > m_boundingSphere.radius)
				m_boundingSphere.radius = distance;
		}
	}

	m_numActiveParticles = index;

	//If there are still particles that need to be generated, then create
	//new slots for them on the particle list.
	XU32 numAllowedParticles = m_maxParticles - m_numActiveParticles;
	if (numParticlesToGenerate > numAllowedParticles)
		numParticlesToGenerate = numAllowedParticles;
	//if (numParticlesToGenerate > 0 && numAllowedParticles != 0)
		for (XU32 i = 0;i < numParticlesToGenerate;++i)
		{
			XParticle* pNewParticle = generateParticle();
			m_pParticlesInUse[m_numActiveParticles++] = pNewParticle;

			//Now check particle distance from emitter and update bounding sphere
			distance = pNewParticle->m_position.distance(m_boundingSphere.center);
			if (distance > m_boundingSphere.radius)
				m_boundingSphere.radius = distance;			
		}

	//if (m_numActiveParticles != 0) 
	//	pScene->addPreRenderEvent(this);

	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleEmitter::prepForRender(XF32 dt,XScene* pScene)
{
	//XDUL::drawSphere(pScene,m_boundingSphere.center,m_boundingSphere.radius,XColor(0.0f,0.0f,0.0f,1.0f));
	if (m_numActiveParticles == 0) 
		return X_OK;

	if (X_TEST_ULONG_BIT(m_propertyFlags,X_SNP_FRUSTUM_CULL))
	{
		XCamera* pCamera = pScene->getActiveCamera();
		if (pCamera)
		{
			if (!pCamera->sphereInFrustumQuick(m_boundingSphere))
			{
				X_STATS(XVP::stats.numFrustumCullsPerFrame++;)
				return X_OK;
			}
		}
	}	
	
	//Render
#pragma message("Do I really want this null check for shader controller?  Maybe it should never be null.. TODO shader controller table look up")

		pScene->addRenderEvent(this,NULL,m_matHandle);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleEmitter::render(XRenderEvent* pRenderEvent)	
{
	//Run through the active particles and build the vertex buffer
	XVertexBuffer* pVB = XVP::GraphicsMgr.lockVertexBuffer(m_vbHandle,X_LT_DISCARD);
	for (XU32 i = 0;i < m_numActiveParticles;++i)
	{
		XU32 offset = i * 6;

		//Set center of particle
		pVB->setPosition(offset,m_pParticlesInUse[i]->m_position);
		pVB->setPosition(offset+1,m_pParticlesInUse[i]->m_position);
		pVB->setPosition(offset+2,m_pParticlesInUse[i]->m_position);
		pVB->setPosition(offset+3,m_pParticlesInUse[i]->m_position);
		pVB->setPosition(offset+4,m_pParticlesInUse[i]->m_position);
		pVB->setPosition(offset+5,m_pParticlesInUse[i]->m_position);
			
		//Set the size and tex coords of particle as well as rotation.
		pVB->setUV(offset,0,0.0f,1.0f);
		pVB->setUV(offset,1,0.0f,m_pParticlesInUse[i]->m_currentSize);
		pVB->setUV(offset,2,m_pParticlesInUse[i]->m_rotationSpeed,0.0f);
		pVB->setUV(offset+1,0,0.0f,0.0f);
		pVB->setUV(offset+1,1,1.0f,m_pParticlesInUse[i]->m_currentSize);
		pVB->setUV(offset+1,2,m_pParticlesInUse[i]->m_rotationSpeed,0.0f);
		pVB->setUV(offset+2,0,1.0f,1.0f);
		pVB->setUV(offset+2,1,2.0f,m_pParticlesInUse[i]->m_currentSize);
		pVB->setUV(offset+2,2,m_pParticlesInUse[i]->m_rotationSpeed,0.0f);
		pVB->setUV(offset+3,0,1.0f,1.0f);
		pVB->setUV(offset+3,1,2.0f,m_pParticlesInUse[i]->m_currentSize);
		pVB->setUV(offset+3,2,m_pParticlesInUse[i]->m_rotationSpeed,0.0f);
		pVB->setUV(offset+4,0,0.0f,0.0f);
		pVB->setUV(offset+4,1,1.0f,m_pParticlesInUse[i]->m_currentSize);
		pVB->setUV(offset+4,2,m_pParticlesInUse[i]->m_rotationSpeed,0.0f);
		pVB->setUV(offset+5,0,1.0f,0.0f);
		pVB->setUV(offset+5,1,3.0f,m_pParticlesInUse[i]->m_currentSize);
		pVB->setUV(offset+5,2,m_pParticlesInUse[i]->m_rotationSpeed,0.0f);
	
		//Set previous position in normal 0
		pVB->setNormal(offset,0,m_pParticlesInUse[i]->m_prevPosition);
		pVB->setNormal(offset+1,0,m_pParticlesInUse[i]->m_prevPosition);
		pVB->setNormal(offset+2,0,m_pParticlesInUse[i]->m_prevPosition);
		pVB->setNormal(offset+3,0,m_pParticlesInUse[i]->m_prevPosition);
		pVB->setNormal(offset+4,0,m_pParticlesInUse[i]->m_prevPosition);
		pVB->setNormal(offset+5,0,m_pParticlesInUse[i]->m_prevPosition);

		//Set the diffuse color.
		pVB->setColor(offset,0,m_pParticlesInUse[i]->m_diffuseColor);
		pVB->setColor(offset+1,0,m_pParticlesInUse[i]->m_diffuseColor);
		pVB->setColor(offset+2,0,m_pParticlesInUse[i]->m_diffuseColor);
		pVB->setColor(offset+3,0,m_pParticlesInUse[i]->m_diffuseColor);
		pVB->setColor(offset+4,0,m_pParticlesInUse[i]->m_diffuseColor);
		pVB->setColor(offset+5,0,m_pParticlesInUse[i]->m_diffuseColor);


		//Set center of particle
		/*XU32 offset = i * 6;
		m_pParticleVertexBuffer[offset].pos.x = m_pParticlesInUse[i]->m_position.x;
		m_pParticleVertexBuffer[offset].pos.y = m_pParticlesInUse[i]->m_position.y;
		m_pParticleVertexBuffer[offset].pos.z = m_pParticlesInUse[i]->m_position.z;
		m_pParticleVertexBuffer[offset+1].pos.x = m_pParticlesInUse[i]->m_position.x;
		m_pParticleVertexBuffer[offset+1].pos.y = m_pParticlesInUse[i]->m_position.y;
		m_pParticleVertexBuffer[offset+1].pos.z = m_pParticlesInUse[i]->m_position.z;
		m_pParticleVertexBuffer[offset+2].pos.x = m_pParticlesInUse[i]->m_position.x;
		m_pParticleVertexBuffer[offset+2].pos.y = m_pParticlesInUse[i]->m_position.y;
		m_pParticleVertexBuffer[offset+2].pos.z = m_pParticlesInUse[i]->m_position.z;

		m_pParticleVertexBuffer[offset+3].pos.x = m_pParticlesInUse[i]->m_position.x;
		m_pParticleVertexBuffer[offset+3].pos.y = m_pParticlesInUse[i]->m_position.y;
		m_pParticleVertexBuffer[offset+3].pos.z = m_pParticlesInUse[i]->m_position.z;
		m_pParticleVertexBuffer[offset+4].pos.x = m_pParticlesInUse[i]->m_position.x;
		m_pParticleVertexBuffer[offset+4].pos.y = m_pParticlesInUse[i]->m_position.y;
		m_pParticleVertexBuffer[offset+4].pos.z = m_pParticlesInUse[i]->m_position.z;
		m_pParticleVertexBuffer[offset+5].pos.x = m_pParticlesInUse[i]->m_position.x;
		m_pParticleVertexBuffer[offset+5].pos.y = m_pParticlesInUse[i]->m_position.y;
		m_pParticleVertexBuffer[offset+5].pos.z = m_pParticlesInUse[i]->m_position.z;

		//Set the size and tex coords of particle.
		m_pParticleVertexBuffer[offset].texCoords.x = 0.0f;
		m_pParticleVertexBuffer[offset].texCoords.y = 1.0f;
		m_pParticleVertexBuffer[offset].texCoords.z = 0.0f;
		m_pParticleVertexBuffer[offset].texCoords.w = m_pParticlesInUse[i]->m_currentSize;
		m_pParticleVertexBuffer[offset+1].texCoords.x = 0.0f;
		m_pParticleVertexBuffer[offset+1].texCoords.y = 0.0f;
		m_pParticleVertexBuffer[offset+1].texCoords.z = 1.0f;
		m_pParticleVertexBuffer[offset+1].texCoords.w = m_pParticlesInUse[i]->m_currentSize;
		m_pParticleVertexBuffer[offset+2].texCoords.x = 1.0f;
		m_pParticleVertexBuffer[offset+2].texCoords.y = 1.0f;
		m_pParticleVertexBuffer[offset+2].texCoords.z = 2.0f;
		m_pParticleVertexBuffer[offset+2].texCoords.w = m_pParticlesInUse[i]->m_currentSize;

		m_pParticleVertexBuffer[offset+3].texCoords.x = 1.0f;
		m_pParticleVertexBuffer[offset+3].texCoords.y = 1.0f;
		m_pParticleVertexBuffer[offset+3].texCoords.z = 2.0f;
		m_pParticleVertexBuffer[offset+3].texCoords.w = m_pParticlesInUse[i]->m_currentSize;
		m_pParticleVertexBuffer[offset+4].texCoords.x = 0.0f;
		m_pParticleVertexBuffer[offset+4].texCoords.y = 0.0f;
		m_pParticleVertexBuffer[offset+4].texCoords.z = 1.0f;
		m_pParticleVertexBuffer[offset+4].texCoords.w = m_pParticlesInUse[i]->m_currentSize;
		m_pParticleVertexBuffer[offset+5].texCoords.x = 1.0f;
		m_pParticleVertexBuffer[offset+5].texCoords.y = 0.0f;
		m_pParticleVertexBuffer[offset+5].texCoords.z = 3.0f;
		m_pParticleVertexBuffer[offset+5].texCoords.w = m_pParticlesInUse[i]->m_currentSize;
	
		//Set the diffuse color.
		m_pParticleVertexBuffer[offset].diffuse = 
		m_pParticleVertexBuffer[offset+1].diffuse =
		m_pParticleVertexBuffer[offset+2].diffuse =
		m_pParticleVertexBuffer[offset+3].diffuse =
		m_pParticleVertexBuffer[offset+4].diffuse =
		m_pParticleVertexBuffer[offset+5].diffuse = m_pParticlesInUse[i]->m_diffuseColor.getARGB32Bit();
	*/}

	XVP::GraphicsMgr.unlockVertexBuffer(m_vbHandle);

	//Render the active particles
	XVP::GraphicsMgr.renderVertexBufferCount(m_vbHandle,m_numActiveParticles*2);

	return X_OK;
	
}

//*****************************************************************************
//*****************************************************************************
void XParticleEmitter::pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits)
{	
	//First, test the bounding sphere, if we intersect the bounding sphere then
	//we'll do a triangle check
	if (m_boundingSphere.radius > 0)
	{
		if (XIRaySphereQuick(pickRay.origin,pickRay.normal,m_boundingSphere.center,m_boundingSphere.radius))
		{
			HitInfo hi;
			hi.pSceneNode = this;
			hi.intersectionPoint = m_boundingSphere.center;
			hits.insertBack(hi);
		}
	}
	/*else
		return;

	//Transform pick ray from world space to model space
	XMatrix4x4 invWorld = m_worldTransform.getInverse();
	XVector4D v(pickRay.normal.x,pickRay.normal.y,pickRay.normal.z,0);
	v = v * invWorld;
	XVector4D v2(pickRay.origin.x,pickRay.origin.y,pickRay.origin.z,1);
	v2 = v2 * invWorld;
	XRay3D newRay(XVector3D(v2.x,v2.y,v2.z),XVector3D(v.x,v.y,v.z));

	XF32 baryU,baryV,baryW,time;

	//For each geometry primitive in lod 0
	for (XU32 i = 0;i < ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0].size();++i)
	{
		//Discover what type of primitive makes up our vertex buffer then calculate the
		//number of triangles
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_vbHandle);
		if (((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache == NULL)
			continue;

		XU32 numTris = 0;
		X_ASSERTM(pVB->getNumVerts() != 0,"0 length vertex buffer found during pick");
		X_ASSERTM(pVB->getPrimType() == X_PT_TRIANGLE_LIST || pVB->getPrimType() == X_PT_TRIANGLE_STRIP,"Vertex buffer needs to be triangle list or triangle strip");
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) / 3;
		else
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) - 2;
		X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during pick");
			
		XVector3D* pPos = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache;
		XU16* pIdx = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pIndexCache;

		//Test the pick ray agains all triangles in the model object.
		XBOOL hitDetected = false;
		for (XU32 j = 0;j < numTris;++j)
		{
			//Check both front and back facing polys
			if (XIRayTriangle(newRay.origin,newRay.normal,pPos[pIdx[j]],pPos[pIdx[j+1]],pPos[pIdx[j+2]],baryU,baryV,baryW,time))
			{
				HitInfo hi;
				hi.pSceneNode = this;
				hi.intersectionPoint = (baryU*pPos[pIdx[j]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j+2]]);
				hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
				hits.insertBack(hi);
			}
			if (XIRayTriangle(newRay.origin,newRay.normal,pPos[pIdx[j+2]],pPos[pIdx[j+1]],pPos[pIdx[j]],baryU,baryV,baryW,time))
			{
				HitInfo hi;
				hi.pSceneNode = this;
				hi.intersectionPoint = (baryU*pPos[pIdx[j+2]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j]]);
				hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
				hits.insertBack(hi);
			}
		}
	}*/
}


//*****************************************************************************
//*****************************************************************************
XStatus XParticleEmitter::getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared)
{
	return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleEmitter::setLODIndex(XU8 LODIndex)
{
	return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XU32 XParticleEmitter::getMaxParticles()
{
	return m_maxParticles;
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleEmitter::setMaxParticles(XU32 maxParticles)
{
	//First, destroy all particles and resources
	//destroy(); 

	//Destroy any remaining particles
	for (XU32 i = 0;i < m_numActiveParticles;++i)
		m_particleFreeList.freeItem(m_pParticlesInUse[i]);
	m_particleFreeList.destroy();

	XDeleteArray(m_pParticlesInUse);
	m_numActiveParticles = 0;
	m_particleResidue = 0;

	XVP::GraphicsMgr.destroyVertexBuffer(m_vbHandle);
	
	XU32 clampedParticles = 1;
	if (maxParticles > clampedParticles)
		clampedParticles = maxParticles;

	m_particleFreeList.init(clampedParticles,"particles");
	XNewArray(m_pParticlesInUse,XParticle*,clampedParticles,XOSMgr::getDefaultHeap());
	
	//Create the particle vertex buffer
	XVertexDesc desc;
	desc.numColors = 1;
	desc.numUVs = 3;
	desc.numNormals = 1;
	XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_TRIANGLE_LIST,clampedParticles*6,true,m_vbHandle);
	X_ASSERTM(m_vbHandle != X_INVALID_HANDLE,"Failed to create particle vertex buffer");

	m_maxParticles = clampedParticles;

	return X_OK;
}



//*****************************************************************************
/**
Generates a particle for the particle system.  This function is called whenever
the particle system spawns a new particle.

@return A pointer to the newly spawned particle.
*/
//*****************************************************************************
XParticle* XParticleEmitter::generateParticle()
{
	//Create new particle
	XParticle* pNewParticle = m_particleFreeList.allocateItem();
	pNewParticle->m_pParentEmitter = this;

	//Based on the emitter type, generate a starting position.
	if (m_emitterType == X_ET_BOX)
	{
		XMatrix4x4 finalTransform = getWorldTransform();
		XVector3D widthVec = XVector3D(finalTransform.row1) * ((m_emitterWidth/2) * X_MATH_RAND_F_N1_P1);
		XVector3D heightVec = XVector3D(finalTransform.row2) * ((m_emitterHeight/2) * X_MATH_RAND_F_N1_P1);
		XVector3D lengthVec = XVector3D(finalTransform.row3) * ((m_emitterLength/2) * X_MATH_RAND_F_N1_P1);
		pNewParticle->m_position = XVector3D(finalTransform.row4) + widthVec + heightVec + lengthVec;
	}
	else if (m_emitterType == X_ET_SPHERE)
	{ 
		XF32 a = X_MATH_RAND_F_0_P1 * X_MATH_HALFPI; 
		XF32 b = X_MATH_RAND_F_0_P1 * X_MATH_2PI;
		XF32 c = m_emitterLength * X_MATH_RAND_F_N1_P1 * 2;
		pNewParticle->m_position.x = c * (XF32)cos(b) * (XF32)cos(a);
		pNewParticle->m_position.y = c * (XF32)sin(b) * (XF32)cos(a);
		pNewParticle->m_position.z = c * (XF32)sin(a);
		pNewParticle->m_position = getWorldTransform() * pNewParticle->m_position;
	}
	else if (m_emitterType == X_ET_CYLINDER)
	{
		XF32 a = X_MATH_RAND_F_0_P1 * X_MATH_2PI;
		XF32 b = m_emitterLength * X_MATH_RAND_F_0_P1;
		pNewParticle->m_position.x = b * (XF32)cos(a);
		pNewParticle->m_position.z = b * (XF32)sin(a);
		pNewParticle->m_position.y = X_MATH_RAND_F_N1_P1 * m_emitterHeight;
		pNewParticle->m_position = getWorldTransform() * pNewParticle->m_position;
	}

	pNewParticle->m_prevPosition = pNewParticle->m_position;

	//Determine the particle's lifetime
	pNewParticle->m_lifetime = m_particleLife + (m_particleLife * X_MATH_RAND_PERCENT(m_particleLifeVariation));

	//Determine the particle's rotation speed
	pNewParticle->m_rotationSpeed = m_particleRotSpeed + (X_MATH_RAND_SIGN * (m_particleRotSpeed * X_MATH_RAND_PERCENT(m_particleRotSpeedVariation)));

	//Determine the particle's size
	pNewParticle->m_maxSize = m_particleSize + (X_MATH_RAND_SIGN * (m_particleSize * X_MATH_RAND_PERCENT(m_particleSizeVariation)));
	pNewParticle->m_growTime = pNewParticle->m_lifetime * m_particleGrowTime;
	pNewParticle->m_shrinkTime = pNewParticle->m_lifetime * m_particleShrinkTime;
	if (m_particleGrowTime > 0)
		pNewParticle->m_currentSize = 0;
	else
		pNewParticle->m_currentSize = pNewParticle->m_maxSize;

	//Determine the particle's color cycling information.
	pNewParticle->m_diffuseColor = m_startColor;
	pNewParticle->m_alphaDelta = ((XF32)(m_endColor.m_A - m_startColor.m_A))/pNewParticle->m_lifetime;
	pNewParticle->m_redDelta = ((XF32)(m_endColor.m_R - m_startColor.m_R))/pNewParticle->m_lifetime;
	pNewParticle->m_greenDelta = ((XF32)(m_endColor.m_G - m_startColor.m_G))/pNewParticle->m_lifetime;
	pNewParticle->m_blueDelta = ((XF32)(m_endColor.m_B - m_startColor.m_B))/pNewParticle->m_lifetime;

	//Determine the particle's speed
	XF32 speed = m_particleSpeed + (m_particleSpeed * X_MATH_RAND_PERCENT(m_particleSpeedVariation));
	
	//Determine the particle's direction.  If the direction vector is 0, then 
	//gernerate a random direction vector for this particle.  Otherwise,
	//use the translated direction vector.
	if ((m_directionVec.x == 0) && (m_directionVec.y == 0) && (m_directionVec.z == 0))
		pNewParticle->m_velocity = X_MATH_RAND_NORM_VEC * (speed);
	else
	{
		//Get a vector that "varies" from the translated direction vector and use it
		//to calculate the particles velocity.
		pNewParticle->m_velocity = varyVector(m_translatedDirectionVec,m_directionVecVariation,X_MATH_PI) * (speed);		
	}
	
	return pNewParticle;
}



//*****************************************************************************
/**
Destroys a particle when its lifetime has expired.  This function is responsible
for destroying all resources allocated to the particle.

@param pParticle	The particle to destroy.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::destroyParticle(XParticle* pParticle)
{
	//XDelete(pParticle);
	m_particleFreeList.freeItem(pParticle);
}



//*****************************************************************************
/**
Starts the emitter which causes particles to be generated.

@return None.
*/
//*****************************************************************************
void XParticleEmitter::start()
{
	m_running = true;
}


void XParticleEmitter::restart()
{
	//Destroy any remaining particles
	for (XU32 i = 0;i < m_numActiveParticles;++i)
		m_particleFreeList.freeItem(m_pParticlesInUse[i]);
	m_numActiveParticles = 0;

	m_totalParticlesEmitted = 0;
	m_particleResidue = 0.0f;
	m_running = true;
}

//*****************************************************************************
/**
Stops the emitter which causes particles to no longer be generated.

@return None.
*/
//*****************************************************************************
void XParticleEmitter::stop()
{
	m_running = false;
	m_particleResidue = 0;
}



//*****************************************************************************
/**
Determines if the particle system is running.

@return True if the particle system is running, false otherwise.
*/
//*****************************************************************************
XBOOL XParticleEmitter::isRunning()
{
	return m_running;
}



//*****************************************************************************
/**
Gets the number of particles in the particle system.

@return The number of particles currently in the particle system.
*/
//*****************************************************************************
XU32 XParticleEmitter::getNumParticles()
{
	return m_numActiveParticles;
}



//*****************************************************************************
/**
Sets the emitter type for the particle system.  The emitter type specifies
what type of volume is spawning the particles.  

@param emitterType	The type of emitter the particle system spawns particles
					from.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setEmitterType(X_EMITTER_TYPE emitterType)
{
	m_emitterType = emitterType;
}



//*****************************************************************************
/**
Gets the emitter type for the particle system.  The emitter type specifies
what type of volume is spawning the particles.  

@return emitterType	The type of emitter the particle system is using to spawn
					particles.
*/
//*****************************************************************************
X_EMITTER_TYPE XParticleEmitter::getEmitterType()
{
	return m_emitterType;
}



//*****************************************************************************
/**
Sets the emitter length.  The length of the emitter is defined by what type of
emitter is set.  For example, if the emitter type is a sphere, then length
is the radius of the sphere.  

@param emitterLength	The length of the emitter.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setEmitterLength(XF32 emitterLength)
{
	m_emitterLength = emitterLength;
}



//*****************************************************************************
/**
Gets the emitter length.  The length of the emitter is defined by what type of
emitter is set.  For example, if the emitter type is a sphere, then length
is the radius of the sphere.  

@return The length of the emitter.
*/
//*****************************************************************************
XF32 XParticleEmitter::getEmitterLength()
{
	return m_emitterLength;
}



//*****************************************************************************
/**
Sets the emitter width.  The width of the emitter is defined by what type of
emitter is set.  For example, if the emitter type is a sphere, then width
is ignored.  

@param emitterWidth	The width of the emitter.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setEmitterWidth(XF32 emitterWidth)
{
	m_emitterWidth = emitterWidth;
}



//*****************************************************************************
/**
Gets the emitter width.  The width of the emitter is defined by what type of
emitter is set.  For example, if the emitter type is a sphere, then width
is ignored.  

@return The width of the emitter.
*/
//*****************************************************************************
XF32 XParticleEmitter::getEmitterWidth()
{
	return m_emitterWidth;
}



//*****************************************************************************
/**
Sets the emitter height.  The height of the emitter is defined by what type of
emitter is set.  For example, if the emitter type is a sphere, then height
is ignored.  

@param emitterHeight	The height of the emitter.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setEmitterHeight(XF32 emitterHeight)
{
	m_emitterHeight = emitterHeight;
}



//*****************************************************************************
/**
Gets the emitter height.  The height of the emitter is defined by what type of
emitter is set.  For example, if the emitter type is a sphere, then height
is ignored.  

@return The height of the emitter.
*/
//*****************************************************************************
XF32 XParticleEmitter::getEmitterHeight()
{
	return m_emitterHeight;
}



//*****************************************************************************
/**
Sets the birthrate for particles.  The birthrate controls
how often the particle system spawns a new particle.

@param birthRate	The time, in seconds, between consecutive particle spawns.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setBirthRate(XF32 birthRate)
{
	m_birthRate = birthRate;
}



//*****************************************************************************
/**
Gets the birthrate for particles.  The birthrate controls
how often the particle system spawns a new particle.

@return The time, in seconds, between consecutive particle spawns.
*/
//*****************************************************************************
XF32 XParticleEmitter::getBirthRate()
{
	return m_birthRate;
}



//*****************************************************************************
/**
Sets the grow time for particles.  The grow time is the time it takes for the
particle to grow from a size of 0 to its final maximum size.

@param particleGrowTime	The time it takes for a particle to grow from a size
						of 0 to its final maximum size.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleGrowTime(XF32 particleGrowTime)
{
	m_particleGrowTime = particleGrowTime;
}



//*****************************************************************************
/**
Gets the grow time for particles.  The grow time is the time it takes for the
particle to grow from a size of 0 to its final maximum size.

@return The time it takes for a particle to grow from a size
		of 0 to its final maximum size.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleGrowTime()
{
	return m_particleGrowTime;
}



//*****************************************************************************
/**
Sets the shrink time for particles.  The shrink time is the time it takes for the
particle to shrink from its maximum size to a size of 0.

@param particleShrinkTime	The time it takes for a particle to shrink from its
							maximum size to a size of 0.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleShrinkTime(XF32 particleShrinkTime)
{
	m_particleShrinkTime = particleShrinkTime;
}



//*****************************************************************************
/**
Gets the shrink time for particles.  The shrink time is the time it takes for the
particle to shrink from its maximum size to a size of 0.

@return The time it takes for a particle to shrink from its
		maximum size to a size of 0.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleShrinkTime()
{
	return m_particleShrinkTime;
}



//*****************************************************************************
/**
Sets the speed for particles. 

@param particleSpeed	The speed of a particle.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleSpeed(XF32 particleSpeed)
{
	m_particleSpeed = particleSpeed;
}



//*****************************************************************************
/**
Gets the particle speed. 

@return The speed of the particle.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleSpeed()
{
	return m_particleSpeed;
}



//*****************************************************************************
/**
Sets the percentage variation of speed for particles.  This variation
describes how much particles will be allowed to deviate from the speed setting.

@param particleSpeedVariation	The variation in particle speed.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleSpeedVariation(XF32 particleSpeedVariation)
{
	m_particleSpeedVariation = particleSpeedVariation;
}



//*****************************************************************************
/**
Gets the percentage variation of speed for particles.  This variation
describes how much particles will be allowed to deviate from the speed setting.

@return	The variation in particle speed.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleSpeedVariation()
{
	return m_particleSpeedVariation;
}



//*****************************************************************************
/**
Sets the lifetime for particles.  The lifetime determines how long particles
will stay alive after they have been spawned.

@param particleLife	The lifetime of particles.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleLife(XF32 particleLife)
{
	m_particleLife = particleLife;
}



//*****************************************************************************
/**
Gets the lifetime for particles.  The lifetime determines how long particles
will stay alive after they have been spawned.

@return	The lifetime of particles.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleLife()
{
	return m_particleLife;
}



//*****************************************************************************
/**
Sets the percentage variation of the lifetime for particles.  This variation
describes how much particles will be allowed to deviate from the lifetime setting.

@param particleLifeVariation	The variation in particle lifetime.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleLifeVariation(XF32 particleLifeVariation)
{
	m_particleLifeVariation = particleLifeVariation;
}



//*****************************************************************************
/**
Gets the percentage variation of lifetime for particles.  This variation
describes how much particles will be allowed to deviate from the lifetime setting.

@return	The variation in particle lifetime.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleLifeVariation()
{
	return m_particleLifeVariation;
}



//*****************************************************************************
/**
Sets the size for particles.  The size determines how big the particles will
appear.  Since particles are rendered on billboards, the actual size of the
particle is exactly XF64 of the specified size parameter.

@param particleSize	The size of particles.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleSize(XF32 particleSize)
{
	m_particleSize = particleSize;
}



//*****************************************************************************
/**
Gets the size for particles.  The size determines how big the particles will
appear.  Since particles are rendered on billboards, the actual size of the
particle is exactly XF64 of the specified size parameter.

@return	The size of particles.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleSize()
{
	return m_particleSize;
}

void XParticleEmitter::setParticleRotSpeed(XF32 particleRotSpeed)
{
	m_particleRotSpeed = particleRotSpeed;
}



//*****************************************************************************
/**
Gets the size for particles.  The size determines how big the particles will
appear.  Since particles are rendered on billboards, the actual size of the
particle is exactly XF64 of the specified size parameter.

@return	The size of particles.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleRotSpeed()
{
	return m_particleRotSpeed;
}


void XParticleEmitter::setParticleRotSpeedVariation(XF32 particleRotSpeedVariation)
{
	m_particleRotSpeedVariation = particleRotSpeedVariation;
}



//*****************************************************************************
/**
Gets the size for particles.  The size determines how big the particles will
appear.  Since particles are rendered on billboards, the actual size of the
particle is exactly XF64 of the specified size parameter.

@return	The size of particles.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleRotSpeedVariation()
{
	return m_particleRotSpeedVariation;
}

//*****************************************************************************
/**
Sets the percentage variation of the size for particles.  This variation
describes how much particles will be allowed to deviate from the size setting.

@param particleSizeVariation	The variation in particle size.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleSizeVariation(XF32 particleSizeVariation)
{
	m_particleSizeVariation = particleSizeVariation;
}



//*****************************************************************************
/**
Gets the percentage variation of the size for particles.  This variation
describes how much particles will be allowed to deviate from the size setting.

@return	The variation in particle size.
*/
//*****************************************************************************
XF32 XParticleEmitter::getParticleSizeVariation()
{
	return m_particleSizeVariation;
}




//*****************************************************************************
/**
Sets the start color for particles.  The start color determines the color of the
particles when they are spawned.  During a particle's lifetime, its color will
be interpolated to its end color setting.

@param startColor	The start color of particles.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleStartColor(XColor& startColor)
{
	m_startColor = startColor;
}



//*****************************************************************************
/**
Gets the start color for particles.  The start color determines the color of the
particles when they are spawned.  During a particle's lifetime, its color will
be interpolated to its end color setting.

@return The start color of particles.
*/
//*****************************************************************************
XColor XParticleEmitter::getParticleStartColor()
{
	return m_startColor;
}



//*****************************************************************************
/**
Sets the end color for particles.  The end color determines the color the
particles will be interpolated to over their lifetime.

@param endColor	The end color of particles.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setParticleEndColor(XColor& endColor)
{
	m_endColor = endColor;
}



//*****************************************************************************
/**
Gets the end color for particles.  The end color determines the color the
particles will be interpolated to over their lifetime.

@return The end color of particles.
*/
//*****************************************************************************
XColor XParticleEmitter::getParticleEndColor()
{
	return m_endColor;
}



//*****************************************************************************
/**
Sets the direction vector particles will follow when they are spawned.

@param directionVector	A normalized vector representing the particles direction.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setDirectionVector(XVector3D directionVector)
{
	m_directionVec = directionVector;
}



//*****************************************************************************
/**
Gets the direction vector particles will follow when they are spawned.

@return	A normalized vector representing the particles initial direction.
*/
//*****************************************************************************
XVector3D XParticleEmitter::getDirectionVec()
{
	return m_directionVec;
}



//*****************************************************************************
/**
Sets the percentage variation of the direction vector for particles.  This
variation describes how much particles will be allowed to deviate from the 
direction vector setting.

@param directionVecVariation	The variation in direction.
@return None.
*/
//*****************************************************************************
void XParticleEmitter::setDirectionVecVariation(XF32 directionVecVariation)
{
	m_directionVecVariation = directionVecVariation;
}



//*****************************************************************************
/**
Gets the percentage variation of the direction vector for particles.  This
variation describes how much particles will be allowed to deviate from the 
direction vector setting.

@return The variation in direction.
*/
//*****************************************************************************
XF32 XParticleEmitter::getDirectionVecVariation()
{
	return m_directionVecVariation;
}


void XParticleEmitter::enableOneShot(XBOOL enable)
{
	m_oneShot = enable;
}

XBOOL XParticleEmitter::isOneShotEnabled()
{
	return m_oneShot;
}
	
//*****************************************************************************
/**
Kills all live particles in the system.
*/
//*****************************************************************************
void XParticleEmitter::flushParticles()
{
	//Destroy current particles
	for (XU32 i = 0;i < m_numActiveParticles;++i)
		destroyParticle(m_pParticlesInUse[i]);
	m_numActiveParticles = 0;
}



//*****************************************************************************
//*****************************************************************************
void XParticleEmitter::setUpRotation(XVector3D& q,XVector3D& w,XF32 theta,
									  XMatrix4x4& rq)
{ 
	XVector3D tempVec;
	XMatrix3x3 r;
	XF32 ww1,ww2,ww3,
		  w12,w13,w23,
		  cosTheta,
		  sinTheta,
		  minCosTheta;

	ww1 = w.array[0] * w.array[0];
	ww2 = w.array[1] * w.array[1];
	ww3 = w.array[2] * w.array[2];
	w12 = w.array[0] * w.array[1];
	w13 = w.array[0] * w.array[2];
	w23 = w.array[1] * w.array[2];

	cosTheta = (XF32)cos(theta);
	minCosTheta = 1.0f - cosTheta;
	sinTheta = (XF32)sin(theta);
	
	r.array[0][0] = ww1 + (1.0f - ww1) * cosTheta;
	r.array[0][1] = w12 * minCosTheta + w.array[2] * sinTheta;
	r.array[0][2] = w13 * minCosTheta - w.array[1] * sinTheta;
	r.array[1][0] = w12 * minCosTheta - w.array[2] * sinTheta;
	r.array[1][1] = ww2 + (1.0f - ww2) * cosTheta;
	r.array[1][2] = w23 * minCosTheta + w.array[0] * sinTheta;
	r.array[2][0] = w13 * minCosTheta + w.array[1] * sinTheta;
	r.array[2][1] = w23 * minCosTheta - w.array[0] * sinTheta;
	r.array[2][2] = ww3 + (1.0f - ww3) * cosTheta;
	 
	tempVec = q * r;

	rq._11 = r._11;rq._12 = r._12;rq._13 = r._13;
	rq._21 = r._21;rq._22 = r._22;rq._23 = r._23;
	rq._31 = r._31;rq._32 = r._32;rq._33 = r._33;
	rq._41 = q.array[0] - tempVec.x;
	rq._42 = q.array[1] - tempVec.y;
	rq._43 = q.array[2] - tempVec.z;
	rq._14 = rq._24 = rq._34 = rq._44 =  1; 
}



//*****************************************************************************
//*****************************************************************************
void XParticleEmitter::rotatePoint(XVector3D& point,XVector3D& q,
									XVector3D& w,XF32 theta)
{ 
	XMatrix4x4 returnMatrix;
	XVector4D returnPoint,p;

	setUpRotation(q,w,theta,returnMatrix);

	p.x = point.x;
	p.y = point.y;
	p.z = point.z;
	p.w = 1;
	 
	returnPoint = p * returnMatrix;
	 
	point.x = returnPoint.x;
	point.y = returnPoint.y;
	point.z = returnPoint.z;
}



//*****************************************************************************
//*****************************************************************************
XVector3D XParticleEmitter::varyVector(XVector3D& vector,XF32 percent,
										 XF32 maxAngle)
{   
	XVector3D result;
	XVector3D zero;

	//Create random vector.
	XVector3D randomVec(X_MATH_RAND_F_N1_P1,X_MATH_RAND_F_N1_P1,X_MATH_RAND_F_N1_P1);
	
	//Get a 3rd vector that is perpendicular to the passed in vector
	//and the random vector.
	XVector3D c = (randomVec.cross(vector)).normalize();

	//Get a random angle within the limits of maxAngle.
	XF32 theta = maxAngle * X_MATH_RAND_PERCENT(percent);

	//Rotate the vector.  "RotatePoint" will modify the first
	//parameter passed in.  So, use a temporary vector and
	//pass back the result.
	result = vector;
	rotatePoint(result,zero,c,theta);
	return result;
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleEmitter::attachShaderController(XS8* pMaterialName,
												   XShaderController* pMatController)
{
	//If we are looking for a specific material name
	if (pMaterialName)
	{
		XS8* matName = XVP::GraphicsMgr.getMaterialName(m_matHandle);
		if (matName)
			if (strcmp(XVP::GraphicsMgr.getMaterialName(m_matHandle),pMaterialName) == 0)
				XVP::GraphicsMgr.getMaterialInterface(m_matHandle)->attachShaderController(pMatController);					
	}
	//else we are applying the controller to all materials
	else
		XVP::GraphicsMgr.getMaterialInterface(m_matHandle)->attachShaderController(pMatController);					
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleEmitter::setMaterialHandle(XHandle matHandle)
{
	m_matHandle = matHandle;
	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XHandle XParticleEmitter::getMaterialHandle()
{
	return m_matHandle;
}




XStatus XParticleEmitter::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("BirthRate",setBirthRate(value))
	PB_MAKE_PROPERTY("EmitterLength",setEmitterLength(value))
	PB_MAKE_PROPERTY("EmitterWidth",setEmitterWidth(value))
	PB_MAKE_PROPERTY("EmitterHeight",setEmitterHeight(value))
	PB_MAKE_PROPERTY("ParticleShrinkTime",setParticleShrinkTime(value))
	PB_MAKE_PROPERTY("ParticleGrowTime",setParticleGrowTime(value))
	PB_MAKE_PROPERTY("ParticleSpeed",setParticleSpeed(value))
	PB_MAKE_PROPERTY("ParticleSpeedVariation",setParticleSpeedVariation(value))
	PB_MAKE_PROPERTY("ParticleLife",setParticleLife(value))
	PB_MAKE_PROPERTY("ParticleLifeVariation",setParticleLifeVariation(value))
	PB_MAKE_PROPERTY("ParticleRotSpeed",setParticleRotSpeed(value))
	PB_MAKE_PROPERTY("ParticleRotSpeedVariation",setParticleRotSpeedVariation(value))
	PB_MAKE_PROPERTY("ParticleSize",setParticleSize(value))
	PB_MAKE_PROPERTY("ParticleSizeVariation",setParticleSizeVariation(value))
	PB_MAKE_PROPERTY("DirectionVecVariation",setDirectionVecVariation(value))

	return X_ERR_NO_SUCH_OBJECT;
}


XStatus XParticleEmitter::setUIntProperty(XS8* pPropertyName,XU32 value)
{
	PB_MAKE_PROPERTY("MaxParticles",setMaxParticles(value))

	return X_ERR_NO_SUCH_OBJECT;
}

XStatus XParticleEmitter::setColorProperty(XS8* pPropertyName,XColor& value)
{
	PB_MAKE_PROPERTY("StartColor",setParticleStartColor(value))
	PB_MAKE_PROPERTY("EndColor",setParticleEndColor(value))

	return X_ERR_NO_SUCH_OBJECT;
}

XStatus XParticleEmitter::setEnumProperty(XS8* pPropertyName,XS32 value)
{
	PB_MAKE_PROPERTY("EmitterType",setEmitterType((X_EMITTER_TYPE)value))

	return X_ERR_NO_SUCH_OBJECT;
}

XStatus XParticleEmitter::setBoolProperty(XS8* pPropertyName,XBOOL value)
{
	PB_MAKE_PROPERTY("OneShot",enableOneShot(value))

	return X_ERR_NO_SUCH_OBJECT;
}

XStatus XParticleEmitter::setVector3DProperty(XS8* pPropertyName,XVector3D& value)
{
	PB_MAKE_PROPERTY("DirectionVec",setDirectionVector(value))

	return X_ERR_NO_SUCH_OBJECT;
}