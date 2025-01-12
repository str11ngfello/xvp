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
//	Module Name:	xvpsdk.h
//	Author:			Lynn Duke
//	Creation Date:	12-11-2001
//
//	Description: 
//
//*****************************************************************************
//*****************************************************************************


#ifndef XSP_WINDOWS
#ifndef XSP_XBOX360
#ifndef XSP_PS3
#error "No XSP has been defined.  Ex. (XSP_WINDOWS)"
#endif
#endif
#endif

#ifdef XSP_WINDOWS
#define NOMINMAX
#endif
#include <../resource.h>
#include <tinyxml.h>
#include <zlib.h>
#include <NxPhysics.h>
extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}
#include <XOSMgrTypes.h>
#include <XGraphicsMgrTypes.h>
#include <XInputMgrTypes.h>
#include <XColor.h>
#include <XHeap.h>
#include <XMemBlockArray.h>
#include <XMemBlockList.h>
#include <XMemoryPool.h>
#include <XFreeList.h>
#include <XFreeArray.h>
#include <XMathUtils.h>
#include <XVector2D.h>
#include <XVector3D.h>
#include <XVector4D.h>
#include <XMatrix3x3.h>
#include <XMatrix4x4.h>
#include <XQuaternion.h>
#include <XRandomNumberLib.h>
#include <XRay3D.h>
#include <XVPDesc.h>
#include <XResource.h>
#include <XResourceList.h>
#include <XRsa.h>
#include <XSmoothFilter.h>
#include <XPropertyBag.h>
#include <XOSMgr.h>
#include <XNew.h>
#include <XString.h>
#include <XStringMap.h>
#include <XObjectFactory.h>
#include <XHeapFactory.h>
#include <XOSMgrLuaGlue.h>
#include <XIntersectLib.h>
#include <XPlane.h>
#include <XPolygon.h>
#include <XRect.h>
#include <XAudioMgr.h>
#include <XAudioMgrLuaGlue.h>
#include <XBaseSound.h>
#include <XOggSound.h>
#include <XOggStream.h>
#include <XWavSound.h>
#include <XWavStream.h>
#include <XMaterial.h>
#include <XBlendState.h>
#include <XDepthStencilState.h>
#include <XSamplerState.h>
#include <XRasterizerState.h>
#include <XTexture.h>
#include <XBaseIndexBuffer.h>
#include <XBaseMovie.h>
#include <XBasePixelShader.h>
#include <XBaseTexture.h>
#include <XBaseVertexShader.h>
#include <XBaseGeometryShader.h>
#include <XDSMovie.h>
#include <XRenderTargetArray.h>
#include <XGraphicsMgr.h>
#include <XGraphicsMgrLuaGlue.h>
#include <XHLSLPixelShader.h>
#include <XHLSLVertexShader.h>
#include <XHLSLGeometryShader.h>
#include <XIndexBuffer.h>
#include <XVertexBuffer.h>
#include <XConstantBuffer.h>
#include <XVertexDeclaration.h>
#include <XInputMgr.h>
#include <XInputMgrLuaGlue.h>
#include <XBaseTCPConnection.h>
#include <XBaseTCPListener.h>
#include <XBaseUDPConnection.h>
#include <XNetworkMgr.h>
#include <XNetworkMgrLuaGlue.h>
#include <XTCPConnection.h>
#include <XTCPListener.h>
#include <XUDPConnection.h>
#include <XMaterialList.h>
#include <XShaderController.h>
#include <XWhiteSC.h>
#include <XVertexColorSC.h>
#include <XMaterialColorSC.h>
#include <XParticleSystemBlinnPhongSC.h>
#include <XParticleSystemNoLightSC.h>
#include <XModelBlinnPhongSC.h>
#include <XModelNoLightSC.h>
#include <XDistortionSC.h>
#include <XShadowCasterSSMSC.h>
#include <XState.h>
#include <XController.h>
#include <XSceneGraph.h>
#include <XSceneNode.h>
#include <XActionMap.h>
#include <XBitmap.h>
#include <XBoundingBox.h>
#include <XBsp.h>
#include <XBSStream.h>
#include <XCharSet.h>
#include <XChaseController.h>
#include <XUFOController.h>
#include <XPRZController.h>
#include <XPhysXController.h>
#include <XInputActionMap.h>
#include <XLookAtController.h>
#include <XPhysicsController.h>
#include <XScreenFader.h>
#include <XSkyBox.h>
#include <XTimer.h>
#include <XViewBindController.h>
#include <XVP.h>
#include <XRenderContext.h>
#include <XPostEffect.h>
#include <XVPLuaGlue.h>
#include <XParentChildCombo.h>
#include <XWad.h>
#include <XPose.h>
#include <XGeometry.h>
#include <XModelObject.h>
#include <XSkinGeometry.h>
#include <XSkinObject.h>
#include <XModel.h>
#include <XModelLoader.h>
#include <XModelLODController.h>
#include <XModelObjectTemplate.h>
#include <XSkinObjectTemplate.h>
#include <XModelTemplate.h>
#include <XViewingFrustum.h>
#include <XCameraObject.h>
#include <XCamera.h>
#include <XLight.h>
#include <XlightDesc.h>
#include <XLightMgr.h>
#include <XOctree.h>
#include <XDebugUtilLib.h>
#include <XGrid.h>
#include <XGizmo.h>
#include <XGizmoController.h>
#include <XGizmoMove.h>
#include <XGizmoRotate.h>
#include <XGizmoScale.h>
#include <XGizmoMgr.h>
#include <XAnimTrack.h>
#include <XAnimTimeEvent.h>
#include <XAnimTrackInfo.h>
#include <XAnimController.h>
#include <XMatAnimTrack.h>
#include <XMatAnimController.h>
#include <XBaseSprite.h>
#include <X2DMovie.h>
#include <XNetAddress.h>
#include <XNetGlobals.h>
#include <XPacketPool.h>
#include <XTCPPacketizer.h>
#include <XNetHost.h>
#include <XGamePlayerDesc.h>
#include <XGamePlayer.h>
#include <XGameServerDesc.h>
#include <XGameServer.h>
#include <XParticle.h>
#include <XParticleEmitter.h>
#include <XParticleSystem.h>
#include <XParticleSystemTemplate.h>
#include <XBone.h>
#include <XSkinMesh.h>
#include <XSkinMeshTemplate.h>
#include <XSprite.h>
#include <XAnimSpriteDesc.h>
#include <XAnimSprite.h>
#include <XTextureSequence.h>
#include <XTexSeqSprite.h>
#include <XScene.h>
#include <XHDRPostEffect.h>
#include <XDOFPostEffect.h>
#include <XEdgePostEffect.h>
#include <XDistortionPostEffect.h>
#include <XMotionBlurPostEffect.h>
#include <XTextBoxDesc.h>
#include <XCursor.h>
#include <XUIElement.h>
#include <XUI2DMovie.h>
#include <XUIAnimSprite.h>
#include <XUIButton.h>
#include <XUIFrame.h>
#include <XUIHorizontalSlider.h>
#include <XUIHorizontalScrollbar.h>
#include <XUISprite.h>
#include <XUIProgressBar.h>
#include <XUITextbox.h>
#include <XUISpinner.h>
#include <XUIToggle.h>
#include <XUIVerticalSlider.h>
#include <XUIVerticalScrollbar.h>
#include <XUIListBox.h>
#include <XUIComboBox.h>
#include <XUserInterfaceLuaGlue.h>
#include <XUserInterface.h>
#include <XUIListView.h>
#include <XConsole.h>
#include <XProfiler.h>
#include <XProfilerIterator.h>
#include <XProfilerNode.h>
#include <XLuaScript.h>
//#include <XScriptBindings.h>
#include <XLua.h>
#include <XActorLuaGlue.h>
#include <XActor.h>
#include <XStage.h>
#include <XActorContainer.h>
#include <XDecalMgr.h>
#include <XOctree.h>

