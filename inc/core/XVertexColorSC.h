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
//	Module Name:	XVertexColorSC.h
//	Author:			Lynn Duke
//	Creation Date:	11-19-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_VERTEX_COLOR_SC_H__
#define __X_VERTEX_COLOR_SC_H__





//*****************************************************************************
//*****************************************************************************
class XVertexColorSC : public XShaderController
{
private:
	XHandle				m_vsHandle;
	XHandle				m_psHandle;
	XHandle				m_vsCBHandle;
	XBOOL				m_inited;

	struct vsCB
	{
		XMatrix4x4  wvpMatrix;
	};
	vsCB m_vsCB;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XVertexColorSC();
	virtual ~XVertexColorSC();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pShaderName = NULL);
	XStatus destroy();
	XS8* getTypeDesc();
	XStatus render(XRenderEvent* pRenderEvent);
};


#endif