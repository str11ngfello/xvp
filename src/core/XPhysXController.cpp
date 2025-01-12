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
//	Module Name:	XPhysXController.cpp
//	Author:			Lynn Duke
//	Creation Date:	10-7-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


NxPhysicsSDK*	XPhysXController::gPhysicsSDK = NULL;
NxScene*		XPhysXController::gScene = NULL;
NxActor*		XPhysXController::groundPlane = NULL;



//*****************************************************************************
/**
*/
//*****************************************************************************
XPhysXController::XPhysXController()
{
	box = NULL;
}



//*****************************************************************************
/**
Destructor for the physics controller.
*/
//*****************************************************************************
XPhysXController::~XPhysXController()
{
}



//*****************************************************************************
//*****************************************************************************
XU8 XPhysXController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_PRE_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
*/
//*****************************************************************************
XS8* XPhysXController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XPhysXController";
	return description; 
}



//*****************************************************************************
/**
*/
//*****************************************************************************
XStatus XPhysXController::update(XF32 dt)
{
	
	return X_OK;
}



//*****************************************************************************
/**
*/
//*****************************************************************************
XStatus XPhysXController::handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,
														 XSceneNode* pParentNode,
														 XFreeArray<XMatrix4x4>* pMatrixStack)
{
	
	NxShape*const* shapes = box->getShapes();
	NxU32 nShapes = box->getNbShapes();
	while (nShapes--)
	{	
		NxMat34 pose = shapes[nShapes]->getGlobalPose();
		//XVector3D v(pose.t.x,pose.t.y,pose.t.z);
		XMatrix4x4 matrix;
		pose.getColumnMajor44((NxF32*)&matrix);
		
		pNode->setModelTransform(matrix);
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPhysXController::initPhysics()
{
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
	X_ASSERTM(gPhysicsSDK,"Physics SDK creation failed");
	
	//Set the physics parameters
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01f);

	//Set the debug visualization parameters
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

    //Create the scene
    NxSceneDesc sceneDesc;
    sceneDesc.gravity = NxVec3(0,-9.8f,0); 
	sceneDesc.simType = NX_SIMULATION_SW; 
	gScene = gPhysicsSDK->createScene(sceneDesc);  
	X_ASSERTM(gPhysicsSDK,"Could not create physics scene");

	//Create the default material
	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);

	//Start the first frame of the simulation
	StartPhysics();

	//Install the callback
	XVP::setPhysicsUpdateCallback(XPhysXController::updatePhysics);

	//Create a default ground plane for now
	groundPlane = CreateGroundPlane();

	return X_OK;
}


XStatus XPhysXController::destroyPhysics()
{
    if (gScene)
	{
		//Make sure to fetchResults() before shutting down
		GetPhysicsResults();  
		gPhysicsSDK->releaseScene(*gScene);
	}
	if (gPhysicsSDK)  
		gPhysicsSDK->release();

	return X_OK;
}

void XPhysXController::updatePhysics(XF32 dt)
{
	//Update Physics Simulation
	if (gScene)
	{
		GetPhysicsResults();
		StartPhysics();
	}
}

//*****************************************************************************
//*****************************************************************************
void XPhysXController::StartPhysics()
{
	//Update the time step
	//gDeltaTime = UpdateTime();

	//Start collision and dynamics for delta time since the last frame
	gScene->simulate(XVP::getFrameDt());
	gScene->flushStream();
}



void XPhysXController::GetPhysicsResults()
{
	//Get results from gScene->simulate(gDeltaTime)
	while (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
}


NxActor* XPhysXController::CreateGroundPlane()
{
    // Create a plane with default descriptor
    NxPlaneShapeDesc planeDesc;
    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&planeDesc);
    return gScene->createActor(actorDesc);
}

NxActor* XPhysXController::CreateBox(const NxVec3& pos, const NxVec3& boxDim, const NxReal density)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(boxDim.x,boxDim.y,boxDim.z);
	boxDesc.localPose.t = NxVec3(0,boxDim.y,0);
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density = density;
	actorDesc.globalPose.t = pos;
	return gScene->createActor(actorDesc);	
}


NxActor* XPhysXController::CreateSphere(const NxVec3& pos, const NxReal radius, const NxReal density)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a sphere
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = radius;
	sphereDesc.localPose.t = NxVec3(0,radius,0);
	actorDesc.shapes.pushBack(&sphereDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density = density;
	actorDesc.globalPose.t = pos;
	return gScene->createActor(actorDesc);	
}

NxActor* XPhysXController::CreateCapsule(const NxVec3& pos, const NxReal height, const NxReal radius, const NxReal density)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a capsule
	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.height = height;
	capsuleDesc.radius = radius;
	capsuleDesc.localPose.t = NxVec3(0,radius+0.5f*height,0);
	actorDesc.shapes.pushBack(&capsuleDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density = density;
	actorDesc.globalPose.t = pos;
	return gScene->createActor(actorDesc);	
}


XStatus XPhysXController::init()
{
	box = CreateBox(NxVec3(5,0,5), NxVec3(0.5,1,0.5), 20);
	return X_OK;
}


XStatus XPhysXController::destroy()
{
	return X_OK;
}


void XPhysXController::addForce(XVector3D& forceVec)
{
	box->addForce((NxVec3&)forceVec);
}