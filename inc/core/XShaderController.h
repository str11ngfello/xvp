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

#ifndef __X_SHADER_CONTROLLER_H__
#define __X_SHADER_CONTROLLER_H__




//---------------------------------------------------------------------------------
//!Controls rendering properties for a material.
/**
The XShaderController object controls rendering properties for materials.  It is
responsible for setting shader constant buffers for the materials before rendering.  
Controllers can be attached to multiple materials.  In this case, a single shader 
constant change will affect all materials currently attached to the controller.  If each
material should have it's own copy of shader constants, you should create multiple
instances of the shader controller and attach as necessary.

You can inherit from the XShaderController object to create new types of controllers.
For example, a blinn-phong lighting controller or a toon shader controller.  See the
samples for examples on creating and using shader controllers for more information.

@warning	It's important that shader controllers do not get deallocated while
			they are attached to materials.  This will cause the application
			to become unstable.  
*/
//---------------------------------------------------------------------------------
class XShaderController : public XPropertyBag
{
protected:
	XU32		m_refCount;

public:
	//---------------------------------------------------------------------------------
	//Constructors/Destructors
	//---------------------------------------------------------------------------------
	XShaderController();
	virtual ~XShaderController();

	//---------------------------------------------------------------------------------
	//Public Interface
	//---------------------------------------------------------------------------------
	virtual XStatus init(XS8* pShaderName = NULL);
	virtual XStatus destroy();
	virtual XS8* getTypeDesc();
	virtual void setShaderParamBlock(XS8* pMemoryChunk);
	virtual XStatus update(XF32 dt);
	virtual XStatus prepForAttachment();
	virtual XStatus prepForDetachment();
	virtual XStatus attachment();
	virtual XStatus detachment();
	virtual XStatus prepForRender();
	virtual XStatus render(XRenderEvent* pRenderEvent) = 0;
};

#endif 