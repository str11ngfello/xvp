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
//*****************************************************************************
//*****************************************************************************

#ifndef __X_COLLADA_H__
#define __X_COLLADA_H__


#include "structs.h"
#include <fstream>


//*****************************************************************************
//Conversion/Utility functions 
//*****************************************************************************
void DAEMatrixConvert(domFloat* pFloats,XMatrix4x4& mat);
void CreateVertexDescription(DAETriangles* pTri,XVertexDesc& desc);

//*****************************************************************************
//General parsing/exporting functions
//*****************************************************************************
void DAEParseMaterialLib();
void DAEParseGeometryLib(DAEGeometryLib& geometryLib);
void DAEParseAnimationLib(DAEAnimationLib& animationLib);
void ExportMaterials(FILE* outFile);


//*****************************************************************************
//Functions for parsing COLLADA documents into models
//*****************************************************************************
void DAEParseModelVisualSceneLib();
void DAEParseModelObjects(domNode_Array& nodeArray,XU32& nextID);
void ExportModel(FILE* outFile);
void ExportModelObjects(FILE* outFile);
void ExportModelVertexBuffersFromTriangleLists(DAENode* pNode,DAESkin* pSkin,DAEGeometry* pGeom,FILE* outFile);


//*****************************************************************************
//Functions for parsing COLLADA documents into skin meshes
//*****************************************************************************
void DAEParseSkinMeshVisualSceneLib();
void DAEParseSkinMeshObjects(domNode_Array& nodeArray,XU32& nextID);
void ExportSkinMesh(FILE* outFile);
void ExportSkinMeshObjects(FILE* outFile);
void ExportSkinMeshVertexBuffersFromTriangleLists(DAESkin* pSkin,DAEGeometry* pGeom,FILE* outFile);


//*****************************************************************************
//Functions for parsing COLLADA documents into animation tracks
//*****************************************************************************
//void DAEParseAnimationVisualSceneLib();
void ExportAnimTrack(FILE* outFile);



#endif