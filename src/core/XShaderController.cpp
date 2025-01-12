//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//     _  ___    ______ 
//    | |/ / |  / / __ \
//    |   /| | / / /_/ /
//   /   | | |/ / ____/ 
//  /_/|_| |___/_/       
//
//	eXtensible Visualization Platform 
//	Copyright 2000 by Seventh Night Studios. All Rights Reserved.
//		
//	This source is protected by copyright laws. Unauthorized reproduction or
//	distribution of this source code in any form will be prosecuted to the
//	maximum extent possible under law.  Please see the XVPLicense.txt file
//	located in the root sdk directory for important licensing information.
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

#include <xvpsdk.h>


//---------------------------------------------------------------------------------
/**
Constructor for the controller.
*/
//---------------------------------------------------------------------------------
XShaderController::XShaderController()
{
	m_refCount = 0;
}


//---------------------------------------------------------------------------------
/**
Destructor for the controller.  
*/
//---------------------------------------------------------------------------------
XShaderController::~XShaderController()
{
}


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
XStatus XShaderController::init(XS8* pShaderName)
{
	return X_OK;
}


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
XStatus XShaderController::destroy()
{
	XVP::removeShaderController(this);
	return X_OK;
}
	
	
//---------------------------------------------------------------------------------
/**
Returns a string description of the controller.  This is the description that
appears in log files and other debug output.  You should override this method
to pass back a specific description for new controllers.

@return A string description of the controller.
*/
//---------------------------------------------------------------------------------
XS8* XShaderController::getTypeDesc()
{
	static char description[X_TYPE_DESC_LEN] = "XShaderController";
	return description;
}


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
void XShaderController::setShaderParamBlock(XS8* pMemoryChunk)
{
}


//---------------------------------------------------------------------------------
/**
Notifies the controller that it is being attached to a scene node.  
It gives the controller a chance to add the scene node as a "client" that will
be requiring updates in the future.  Also, if necessary, X-VP is
notified that it now must update this controller once per frame.

@param pNode	A pointer to the scene node that this controller is being attached
				to.
@param passType	An enum indicating that this controller has been attached as either
				an update or render controller.
@return A status code of type XStatus.
*/
//---------------------------------------------------------------------------------
XStatus XShaderController::prepForAttachment()
{
	//Increment the reference count
	++m_refCount;
	
	//Only add the controller to the engine once.  If the refCount = 1 then
	//this is the first time the controller is being attached to a node,
	//therefore we add it to the engine.
	if (m_refCount == 1)
		XVP::addShaderController(this);

	return attachment();
}


//---------------------------------------------------------------------------------
/**
Notifies the controller that it is being detached from a scene node.  
It gives the controller a chance to remove the scene node as a "client" that will
be requiring updates in the future.  Also, if necessary, X-VP is notified
that it does not have to update the controller anymore.

@param pNode	A pointer to the scene node that this controller is being detached
				from.
@param passType	An enum indicating that this controller has been attached as either
				an update or render controller.
@return A status code of type XStatus.
*/
//---------------------------------------------------------------------------------
XStatus XShaderController::prepForDetachment()
{
	//Decrement the reference count.
	if (m_refCount > 0)
		--m_refCount;

	//If this controller is no longer attached to any nodes then remove from
	//xvp.
	if (m_refCount == 0)
		XVP::removeShaderController(this);

	return detachment();
}


//---------------------------------------------------------------------------------
/**
Responds to a render attachment event.  This type of event occurs when the
controller is attached to a scene node as a render controller.  You can override
this method for your derived controller to add custom functionality in response
to this type of event.

@param pNode	The node that this controller is being attached to.
@return A return code of type XStatus.
*/
//---------------------------------------------------------------------------------
XStatus XShaderController::attachment()
{
	return X_OK;
}


//---------------------------------------------------------------------------------
/**
Responds to a render detachment event.  This type of event occurs when the
controller is detached from a scene node as a render controller.  You can override
this method for your derived controller to add custom functionality in response to
this type of event.

@param pNode	The node that this controller is being detached from.
@return A return code of type XStatus.
*/
//---------------------------------------------------------------------------------
XStatus XShaderController::detachment()
{
	return X_OK;
}


//---------------------------------------------------------------------------------
/**
Updates time dependent data associated with the controller.  X-VP calls this
method if the controller is attached and the node its attached to is in
the scene graph.  You should override this method to implement custom
functionality for new controllers.

@param dt The amount of time, in seconds, that has elapsed since the
				 last update.
@return None.
*/
//---------------------------------------------------------------------------------
XStatus XShaderController::update(XF32 dt)
{
	return X_OK;
}


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
XStatus XShaderController::prepForRender()
{
	return X_OK;
}