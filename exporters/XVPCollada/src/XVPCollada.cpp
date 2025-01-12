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
//	Module Name:	XVPCollada.cpp
//	Author:			Lynn Duke
//	Creation Date:	2-13-2006
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#define _CRT_SECURE_NO_DEPRECATE
#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN	
#define NOMINMAX
#define VERSION 1		

//System includes

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <list>
#include "dae.h" 
#include "dom/domCOLLADA.h"
#include <dom/domProfile_COMMON.h> 
#include "CommandLineOptions.h"
#include "structs.h"
#include "XBSStream.h"
#include "XVPCollada.h"
#include "XVertexFormats.h"
#include "packedVertexIndex.h"
#include "NvTriStrip.h"
#include "NvMeshMender.h"
#include "XIntersectLib.h"

using namespace std;

#define DEFAULT_ROOT_NODE "__root__"


//*****************************************************************************
//Globals
//*****************************************************************************
DAE* g_pInputDAE = NULL;
DAEMaterialLib g_materialLib;
DAEGeometryLib g_geometryLib;
DAEAnimationLib g_animationLib;
DAENodeLib g_nodeLib;
DAESkinLib g_skinLib;

XBOOL g_defaultRootNodeGenerated = false;


enum UP_DIR
{
	X_UP,
	Y_UP,
	Z_UP
};
UP_DIR g_upDirection;

XBOOL genTanBi = false;
XBOOL tristrip = false;

//*****************************************************************************
//Helper/Utility functions.
//*****************************************************************************
void FatalError(string errorMsg,XU32 line)
{
	cout << "###### ERROR (" << line << ") -> " << errorMsg << " ... exiting." << endl;
	exit(-1);
}
#define FATAL_ERROR(errorMsg) { FatalError(errorMsg,__LINE__);}

#define FATAL_ERRORF(string,...)	{XS8 buf[4096]; sprintf(buf,string,__VA_ARGS__); FatalError(buf,__LINE__);}


void WARNING(string warningMsg)
{
	cout << "##### WARNING -> " << warningMsg << endl;
}

#define WARNINGF(string,...)	{XS8 buf[4096]; sprintf(buf,string,__VA_ARGS__); cout << buf;}

#if 0
template <typename T> 
inline T *daeSafeCast(/* const */ daeElement *element) 
{ 
	if(element && element->getMeta() == T::_Meta) 
		return (T *)element; 
	return NULL; 
} 
#endif
 
template <typename T> 
T *getEffectForProfile(const domFx_profile_abstract_Array &array) 
{ 
	for(size_t i = 0 ; i < array.getCount() ; i++) 
	{ 
		domFx_profile_abstractRef fxProfile = array.get(i); 
		if(!fxProfile) 
			continue; 
		T *test = daeSafeCast<T>(fxProfile); 
		if(test) 
			return test; 
	} 
	return NULL; 
} 


void DAEMatrixConvert(domFloat* pFloats,XMatrix4x4& mat)
{
	switch (g_upDirection)
	{
	case Z_UP:
		mat._11 = (XF32)pFloats[0]; mat._12 = (XF32)pFloats[8]; mat._13 = (XF32)pFloats[4]; mat._14 = (XF32)pFloats[12];
		mat._21 = (XF32)pFloats[2]; mat._22 = (XF32)pFloats[10]; mat._23 = (XF32)pFloats[6]; mat._24 = (XF32)pFloats[14];
		mat._31 = (XF32)pFloats[1]; mat._32 = (XF32)pFloats[9]; mat._33 = (XF32)pFloats[5]; mat._34 = (XF32)pFloats[13];
		mat._41 = (XF32)pFloats[3]; mat._42 = (XF32)pFloats[11]; mat._43 = (XF32)pFloats[7]; mat._44 = (XF32)pFloats[15];
		break;
	}
}


void CreateVertexDescription(DAETriangles* pTri,DAESkin* pSkin,XVertexDesc& desc)
{
	desc.numNormals = 1;

	XU32 numColorSets = 0;
	for (XU32 i = 0;i < pTri->colorIndices.size();++i)
		if (pTri->colorIndices[i].size() != 0)
			++numColorSets;
	desc.numColors = numColorSets;

	XU32 numUVSets = 0;
	for (XU32 i = 0;i < pTri->texCoordIndices.size();++i)
		if (pTri->texCoordIndices[i].size() != 0)
			++numUVSets;
	desc.numUVs = numUVSets;
	if (genTanBi)
	{
		desc.numTangents = 1;
		desc.numBinormals = 1;
	}
	else
	{
		desc.numTangents = 0;
		desc.numBinormals = 0;
	}
		
	if (pSkin)
		desc.numBoneWeights4Tuplets = desc.numBoneIndices4Tuplets = 1;
}



//*****************************************************************************
//*****************************************************************************
int main(int argc, char* argv[])
{

	int error;
	string pInputFile,pOutputFile,pTemplateString,pPrimType,pGenerate;

	

	CommandLineOptions* pClo = new CommandLineOptions();
	pClo->SetTitle("XVPCollada Exporter");
	pClo->AddExample("XVPCollada -i infile -o outfile -t template", "Take input and output from files");
	pClo->AddSetting("i", "inputfile", "Input COLLADA file name");
	pClo->AddSetting("o", "outputfile", "Output XBS file name");
	pClo->AddSetting("t", "template", "Template type (model, animtrack, etc...");
	pClo->AddSetting("p", "primitive", "Primitive type (trilist,tristrip, etc...");
	pClo->AddSetting("g", "generate", "Generate tangent/binormal (tanbi, etc...");
	pClo->AddOption("V", "Print version");

	//Parse the command line options
	pClo->Load(argc, argv);

	//If requested, print version and exit
    if(pClo->CheckForOption("V"))
	{
		cout << "XVPCollada version " << VERSION << "\n";
		return 0;
	}

	//Input and output file names are required
	if(pClo->CheckForSetting("i"))
	{
		pInputFile = pClo->GetString("i");
		cout << "Input File: " << pInputFile << endl;
	}
	else
	{
		cout << "No input file specified.\n";
		return 0;
	}

	if(pClo->CheckForSetting("o"))
	{
		pOutputFile = pClo->GetString("o");
		cout << "Output File: " << pOutputFile << endl;
	}
	else
	{
		cout << "No output file specified.\n";
		return 0;
	}

	if(pClo->CheckForSetting("t"))
	{
		pTemplateString = pClo->GetString("t");
		cout << "Template: " << pTemplateString << endl;
	}
	else
	{
		cout << "No template specified.\n";
		return 0;
	}

	if(pClo->CheckForSetting("p"))
	{
		pPrimType = pClo->GetString("p");
		cout << "Primitive: " << pPrimType << endl;
		if (pPrimType == "tristrip")
			tristrip = true;
	}
	else
	{
		cout << "No primitive type specified, defaulting to triangle list.";
		pPrimType = "trilist";
	}

	if(pClo->CheckForSetting("g"))
	{
		pGenerate = pClo->GetString("g");
		if (pGenerate == "tanbi")
		{
			cout << "Tangent/Binormal generation enabled." << endl;
			genTanBi = true;
		}
		else
			cout << "Tangent/Binormal generation disabled." << endl;
	}

	//Load the COLLADA file
	g_pInputDAE = new DAE;	

	error = g_pInputDAE->load(pInputFile.c_str() );
	if (error != DAE_OK)
	{
		
		cout << "Could not load input file: " << daeErrorString(error) <<"\n";
		cout << pInputFile.c_str() << endl;
		int lll;
		cin >> lll;
		return 0;
	}

	
	g_upDirection = Z_UP;
	
		
	FILE* outFile = fopen(pOutputFile.c_str(),"wb");

	//If we couldn't create the file, we're done.
	if (!outFile)
	{
		FATAL_ERROR("Could not create new file.  Make sure the target file is not read only.");
		return 0;
	}

	//Keep track of how long the export takes
	XU32 startTime = timeGetTime();

	//Output according to specified template
	if (pTemplateString == "model")
	{
		ExportModel(outFile);
	}
	else if (pTemplateString == "skinmesh")
	{
		ExportSkinMesh(outFile);
	}
	else if (pTemplateString == "animtrack")
	{
		ExportAnimTrack(outFile);
	}
	else
	{
		cout << "Unrecognized template type encountered." << endl;
		return 0;
	}

	XU32 endTime = timeGetTime();

	cout << endl << "Asset exported in " << (endTime - startTime)/1000.0f << " seconds." << endl;

	fclose(outFile);

	//Exit with success
	delete g_pInputDAE;

	DAE::cleanup();
 
	return 0;  
} 
 
 
//*****************************************************************************
//*****************************************************************************
void DAEParseModelVisualSceneLib()
{
	int error;
	g_defaultRootNodeGenerated = false;

	//How many visual scenes are there
	int visualSceneElementCount = g_pInputDAE->getDatabase()->getElementCount(NULL,"visual_scene",NULL);
	cout  << "\n\nFound " << visualSceneElementCount << " visual scenes.\n"; 

	//Only one visual scene is supported
	if (visualSceneElementCount == 0)
		FATAL_ERROR("No visual scenes found");
	if (visualSceneElementCount > 1)
		FATAL_ERROR("Only 1 visual scene is supported");

	//Iterate over all the visual scene elements
	for(int currentVisualScene = 0; currentVisualScene < visualSceneElementCount; currentVisualScene++)
	{
		//Get the next visual scene element
		domVisual_scene* thisVisualScene;
		error = g_pInputDAE->getDatabase()->getElement((daeElement**)&thisVisualScene,currentVisualScene, NULL, "visual_scene");
		
		//Get name of visual scene
		if (thisVisualScene->getName())
			cout << "\n--------------------------------------------------\nProcessing " << thisVisualScene->getName() << endl;			
		
		domNode_Array& nodeArray = thisVisualScene->getNode_array();
		XU32 nextID = 0;
		
		//If there are no nodes, then we can't continue
		if (nodeArray.getCount() == 0)
			FATAL_ERROR("Visual scene doesn't have any nodes")

		//If there is more than one node at the top level, then we need to create a dummy node
		//to contain all nodes in this array.  This is only for the first level.
		else if (nodeArray.getCount() > 1)
		{
			cout << "More that one node at root level.  Generating default root..." << endl;
			DAENode newNode;
			g_nodeLib.nodeList.push_back(newNode);

			//Get a pointer to the new node object
			DAENode* pNode = &g_nodeLib.nodeList[g_nodeLib.nodeList.size()-1];
			pNode->ID = nextID++;
			pNode->name = DEFAULT_ROOT_NODE;	

			g_defaultRootNodeGenerated = true;
		}
		
		//Parse the rest of the scene
		DAEParseModelObjects(nodeArray,nextID);
	}

}



//*****************************************************************************
//*****************************************************************************
void DAEParseSkinMeshVisualSceneLib()
{
	int error;

	//How many visual scenes are there
	int visualSceneElementCount = g_pInputDAE->getDatabase()->getElementCount(NULL,"visual_scene",NULL);
	cout  << "\n\nFound " << visualSceneElementCount << " visual scenes.\n"; 

	//Only one visual scene is supported
	if (visualSceneElementCount == 0)
		FATAL_ERROR("No visual scenes found");
	if (visualSceneElementCount > 1)
		FATAL_ERROR("Only 1 visual scene is supported");

	//Iterate over all the visual scene elements
	for(int currentVisualScene = 0; currentVisualScene < visualSceneElementCount; currentVisualScene++)
	{
		//Get the next visual scene element
		domVisual_scene* thisVisualScene;
		error = g_pInputDAE->getDatabase()->getElement((daeElement**)&thisVisualScene,currentVisualScene, NULL, "visual_scene");
		
		//Get name of visual scene
		if (thisVisualScene->getName())
			cout << "\n--------------------------------------------------\nProcessing " << thisVisualScene->getName() << endl;			
		
		domNode_Array& nodeArray = thisVisualScene->getNode_array();
		XU32 nextID = 0;
		
		//If there are no nodes, then we can't continue
		if (nodeArray.getCount() == 0)
			FATAL_ERROR("Visual scene doesn't have any nodes");
		
		//Parse the rest of the scene
		DAEParseSkinMeshObjects(nodeArray,nextID);
	}

}



//*****************************************************************************
//*****************************************************************************
void DAEParseMaterialLib()
{
	int error;

	//How many materials are there
	int materialElementCount = g_pInputDAE->getDatabase()->getElementCount(NULL,"material",NULL);
	cout  << "\n\nFound " << materialElementCount << " material elements.\n"; 

	//Iterate over all the material elements
	for(int currentMaterial = 0; currentMaterial < materialElementCount; currentMaterial++)
	{
		DAEMaterial newMaterial;
		g_materialLib.materialList.push_back(newMaterial);

		//Get a pointer to the new material object
		DAEMaterial* pMaterial = &g_materialLib.materialList[g_materialLib.materialList.size()-1];

		//Get the next material element
		domMaterial* thisMaterial;
		error = g_pInputDAE->getDatabase()->getElement((daeElement**)&thisMaterial,currentMaterial, NULL, "material");

		//Get name of material element
		if (!thisMaterial->getID())
			FATAL_ERROR("Material doesn't have an ID")
		else
			cout << "\n--------------------------------------------------\nProcessing " << thisMaterial->getID() << endl;			
			
		pMaterial->name = thisMaterial->getID();
		
		thisMaterial->getInstance_effect()->getUrl().resolveElement();
		daeElement* pElement = thisMaterial->getInstance_effect()->getUrl().getElement();
		domEffect* pEffect = (domEffect*)pElement;
		if (!pEffect)
			FATAL_ERROR("Could not get effect referenced by material");
		
		//Get name of effect element
		if (pEffect->getID())
			cout << "Effect ID: " << pEffect->getID() << endl;

		domFx_profile_abstract_Array& profileArray = pEffect->getFx_profile_abstract_array();
		if (profileArray.getCount() <= 0)
			FATAL_ERROR("No profile could be found for effect");

		for (unsigned int currentProfile = 0;currentProfile < profileArray.getCount();++currentProfile)
		{		
			if (strcmp(profileArray[currentProfile]->getTypeName(),"profile_COMMON") == 0)
			{
				cout << "Found profile of type " << profileArray[currentProfile]->getTypeName() << endl;
				domProfile_COMMON* commonProfile = getEffectForProfile<domProfile_COMMON>(profileArray); 	
				domProfile_COMMON::domTechniqueRef technique = commonProfile->getTechnique();
				domCommon_newparam_type_Array& paramArray = commonProfile->getNewparam_array();

				//Is there a phong element?
				domProfile_COMMON::domTechnique::domPhong* pPhong = technique->getPhong();
				if (pPhong)
				{				
					cout << "Found phong element." << endl;
					domFx_color_common color;

					if (pPhong->getDiffuse())
					{
						//DIFFUSE
						cout << "DIFFUSE ";
						if (!pPhong->getDiffuse()->getColor())
						{
							//Determine image information....first find the sampler name from the texture element.
							xsNCName samplerName = pPhong->getDiffuse()->getTexture()->getTexture();
							for (unsigned int i = 0; i < paramArray.getCount();++i)
							{
								if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
								{
									//Now find the surface name from the 2d sampler
									xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
									for (unsigned int j = 0; j < paramArray.getCount();++j)
									{
										if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
										{
											//Now find the surface name from the 2d sampler
											domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
											if (initArray.getCount() != 1)
												FATAL_ERROR("Expected exactly 1 element in initFrom array")
											else
											{
												//Finally we can access the real image filename.
												domImage* pImage;
												error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
												cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

												pMaterial->diffuseColor.m_R = 1.0f;
												pMaterial->diffuseColor.m_G = 1.0f;
												pMaterial->diffuseColor.m_B = 1.0f;
												pMaterial->diffuseColor.m_A = 1.0f;
												pMaterial->diffuseMapName = pImage->getInit_from()->getValue().getFile();
											}
										}
									}
								}
							}
						}
						else
						{
							color = pPhong->getDiffuse()->getColor()->getValue();
				 			cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
							
							pMaterial->diffuseColor.m_R = (XF32)color[0];
							pMaterial->diffuseColor.m_G = (XF32)color[1];
							pMaterial->diffuseColor.m_B = (XF32)color[2];
							pMaterial->diffuseColor.m_A = (XF32)color[3];
						}
					}

					if (pPhong->getAmbient())
					{
						//AMBIENT
						cout << "AMBIENT ";
						if (!pPhong->getAmbient()->getColor())
						{
							//Determine image information....first find the sampler name from the texture element.
							xsNCName samplerName = pPhong->getAmbient()->getTexture()->getTexture();
							for (unsigned int i = 0; i < paramArray.getCount();++i)
							{
								if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
								{
									//Now find the surface name from the 2d sampler
									xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
									for (unsigned int j = 0; j < paramArray.getCount();++j)
									{
										if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
										{
											//Now find the surface name from the 2d sampler
											domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
											if (initArray.getCount() != 1)
												FATAL_ERROR("Expected exactly 1 element in initFrom array")
											else
											{
												//Finally we can access the real image filename.
												domImage* pImage;
												error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
												cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

												pMaterial->ambientColor.m_R = 1.0f;
												pMaterial->ambientColor.m_G = 1.0f;
												pMaterial->ambientColor.m_B = 1.0f;
												pMaterial->ambientColor.m_A = 1.0f;
												pMaterial->ambientMapName = pImage->getInit_from()->getValue().getFile();
											}
										}
									}
								}
							}
						}
						else
						{
							color = pPhong->getAmbient()->getColor()->getValue();
							cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
							
							pMaterial->ambientColor.m_R = (XF32)color[0];
							pMaterial->ambientColor.m_G = (XF32)color[1];
							pMaterial->ambientColor.m_B = (XF32)color[2];
							pMaterial->ambientColor.m_A = (XF32)color[3];
						}
					}

					if (pPhong->getSpecular())
					{
						//SPECULAR
						cout << "SPECULAR ";
						if (!pPhong->getSpecular()->getColor())
						{
							//Determine image information....first find the sampler name from the texture element.
							xsNCName samplerName = pPhong->getSpecular()->getTexture()->getTexture();
							for (unsigned int i = 0; i < paramArray.getCount();++i)
							{
								if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
								{
									//Now find the surface name from the 2d sampler
									xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
									for (unsigned int j = 0; j < paramArray.getCount();++j)
									{
										if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
										{
											//Now find the surface name from the 2d sampler
											domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
											if (initArray.getCount() != 1)
												FATAL_ERROR("Expected exactly 1 element in initFrom array")
											else
											{
												//Finally we can access the real image filename.
												domImage* pImage;
												error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
												cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

												pMaterial->specularColor.m_R = 1.0f;
												pMaterial->specularColor.m_G = 1.0f;
												pMaterial->specularColor.m_B = 1.0f;
												pMaterial->specularColor.m_A = 1.0f;
												pMaterial->specularMapName = pImage->getInit_from()->getValue().getFile();
											}
										}
									}
								}
							}
						}
						else
						{
							color = pPhong->getSpecular()->getColor()->getValue();
							cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
							
							pMaterial->specularColor.m_R = (XF32)color[0];
							pMaterial->specularColor.m_G = (XF32)color[1];
							pMaterial->specularColor.m_B = (XF32)color[2];
							pMaterial->specularColor.m_A = (XF32)color[3];
						}
					}
					
					if(pPhong->getShininess())
						cout << "SHININESS (" << pPhong->getShininess()->getFloat()->getValue() << ")" << endl;

					if (pPhong->getReflective())
					{
						//REFLECTIVE
						cout << "REFLECTIVE ";
						if (!pPhong->getReflective()->getColor())
						{
							//Determine image information....first find the sampler name from the texture element.
							xsNCName samplerName = pPhong->getReflective()->getTexture()->getTexture();
							for (unsigned int i = 0; i < paramArray.getCount();++i)
							{
								if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
								{
									//Now find the surface name from the 2d sampler
									xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
									for (unsigned int j = 0; j < paramArray.getCount();++j)
									{
										if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
										{
											//Now find the surface name from the 2d sampler
											domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
											if (initArray.getCount() != 1)
												FATAL_ERROR("Expected exactly 1 element in initFrom array")
											else
											{
												//Finally we can access the real image filename.
												domImage* pImage;
												error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
												cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

												pMaterial->reflectiveColor.m_R = 1.0f;
												pMaterial->reflectiveColor.m_G = 1.0f;
												pMaterial->reflectiveColor.m_B = 1.0f;
												pMaterial->reflectiveColor.m_A = 1.0f;
												pMaterial->reflectiveMapName = pImage->getInit_from()->getValue().getFile();
											}
										}
									}
								}
							}
							/*domImage* pImage;
							error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,pPhong->getReflective()->getTexture()->getTexture() ,NULL);
							cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";
							cout << "(" << pPhong->getReflective()->getTexture()->getTexcoord() << ") " << endl;

							pMaterial->reflectiveColor.m_R = 1.0f;
							pMaterial->reflectiveColor.m_G = 1.0f;
							pMaterial->reflectiveColor.m_B = 1.0f;
							pMaterial->reflectiveColor.m_A = 1.0f;
							pMaterial->reflectiveMapName = pImage->getInit_from()->getValue().getFile();*/
						}
						else
						{
							color = pPhong->getReflective()->getColor()->getValue();
							cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
							
							pMaterial->reflectiveColor.m_R = (XF32)color[0];
							pMaterial->reflectiveColor.m_G = (XF32)color[1];
							pMaterial->reflectiveColor.m_B = (XF32)color[2];
							pMaterial->reflectiveColor.m_A = (XF32)color[3];
						}
					}

					//TRANSPARENT
					if (pPhong->getTransparent())
					{
						cout << "TRANSPARENT ";
						if (!pPhong->getTransparent()->getColor())
						{
							//Determine image information....first find the sampler name from the texture element.
							xsNCName samplerName = pPhong->getTransparent()->getTexture()->getTexture();
							for (unsigned int i = 0; i < paramArray.getCount();++i)
							{
								if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
								{
									//Now find the surface name from the 2d sampler
									xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
									for (unsigned int j = 0; j < paramArray.getCount();++j)
									{
										if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
										{
											//Now find the surface name from the 2d sampler
											domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
											if (initArray.getCount() != 1)
												FATAL_ERROR("Expected exactly 1 element in initFrom array")
											else
											{
												//Finally we can access the real image filename.
												domImage* pImage;
												error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
												cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

												pMaterial->transparentColor.m_R = 1.0f;
												pMaterial->transparentColor.m_G = 1.0f;
												pMaterial->transparentColor.m_B = 1.0f;
												pMaterial->transparentColor.m_A = 1.0f;
												pMaterial->transparentMapName = pImage->getInit_from()->getValue().getFile();
											}
										}
									}
								}
							}
						}
						else
						{
							color = pPhong->getTransparent()->getColor()->getValue();
				 			cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
							
							pMaterial->transparentColor.m_R = (XF32)color[0];
							pMaterial->transparentColor.m_G = (XF32)color[1];
							pMaterial->transparentColor.m_B = (XF32)color[2];
							pMaterial->transparentColor.m_A = (XF32)color[3];
						}
					}

					if (pPhong->getTransparency())
					{
						//TRANSPARENCY
						cout << "TRANSPARENCY ";
						if (!pPhong->getTransparency()->getParam())
						{
							cout << "(" << pPhong->getTransparency()->getFloat()->getValue() << ")" << endl;
						}
						else
						{
							FATAL_ERROR("Don't know how to process \"param\" type for TRANPARENCY");
						}
					}
					continue;
				}

				//Is there a blinn element?
				domProfile_COMMON::domTechnique::domBlinn* pBlinn = technique->getBlinn();
				if (pBlinn)
				{					
					cout << "Found blinn element." << endl;
					domFx_color_common color;

					//DIFFUSE
					cout << "DIFFUSE ";
					if (!pBlinn->getDiffuse()->getColor())
					{
						//Determine image information....first find the sampler name from the texture element.
						xsNCName samplerName = pBlinn->getDiffuse()->getTexture()->getTexture();
						for (unsigned int i = 0; i < paramArray.getCount();++i)
						{
							if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
							{
								//Now find the surface name from the 2d sampler
								xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
								for (unsigned int j = 0; j < paramArray.getCount();++j)
								{
									if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
									{
										//Now find the surface name from the 2d sampler
										domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
										if (initArray.getCount() != 1)
											FATAL_ERROR("Expected exactly 1 element in initFrom array")
										else
										{
											//Finally we can access the real image filename.
											domImage* pImage;
											error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
											cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

											pMaterial->diffuseColor.m_R = 1.0f;
											pMaterial->diffuseColor.m_G = 1.0f;
											pMaterial->diffuseColor.m_B = 1.0f;
											pMaterial->diffuseColor.m_A = 1.0f;
											pMaterial->diffuseMapName = pImage->getInit_from()->getValue().getFile();
										}
									}
								}
							}
						}
					}
					else
					{
						color = pBlinn->getDiffuse()->getColor()->getValue();
				 		cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
						
						pMaterial->diffuseColor.m_R = (XF32)color[0];
						pMaterial->diffuseColor.m_G = (XF32)color[1];
						pMaterial->diffuseColor.m_B = (XF32)color[2];
						pMaterial->diffuseColor.m_A = (XF32)color[3];
					}

					//AMBIENT
					cout << "AMBIENT ";
					if (!pBlinn->getAmbient()->getColor())
					{
						//Determine image information....first find the sampler name from the texture element.
						xsNCName samplerName = pBlinn->getAmbient()->getTexture()->getTexture();
						for (unsigned int i = 0; i < paramArray.getCount();++i)
						{
							if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
							{
								//Now find the surface name from the 2d sampler
								xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
								for (unsigned int j = 0; j < paramArray.getCount();++j)
								{
									if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
									{
										//Now find the surface name from the 2d sampler
										domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
										if (initArray.getCount() != 1)
											FATAL_ERROR("Expected exactly 1 element in initFrom array")
										else
										{
											//Finally we can access the real image filename.
											domImage* pImage;
											error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
											cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

											pMaterial->ambientColor.m_R = 1.0f;
											pMaterial->ambientColor.m_G = 1.0f;
											pMaterial->ambientColor.m_B = 1.0f;
											pMaterial->ambientColor.m_A = 1.0f;
											pMaterial->ambientMapName = pImage->getInit_from()->getValue().getFile();
										}
									}
								}
							}
						}
					}
					else
					{
						color = pBlinn->getAmbient()->getColor()->getValue();
						cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
						
						pMaterial->ambientColor.m_R = (XF32)color[0];
						pMaterial->ambientColor.m_G = (XF32)color[1];
						pMaterial->ambientColor.m_B = (XF32)color[2];
						pMaterial->ambientColor.m_A = (XF32)color[3];
					}

					//SPECULAR
					cout << "SPECULAR ";
					if (!pBlinn->getSpecular()->getColor())
					{
						//Determine image information....first find the sampler name from the texture element.
						xsNCName samplerName = pBlinn->getSpecular()->getTexture()->getTexture();
						for (unsigned int i = 0; i < paramArray.getCount();++i)
						{
							if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
							{
								//Now find the surface name from the 2d sampler
								xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
								for (unsigned int j = 0; j < paramArray.getCount();++j)
								{
									if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
									{
										//Now find the surface name from the 2d sampler
										domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
										if (initArray.getCount() != 1)
											FATAL_ERROR("Expected exactly 1 element in initFrom array")
										else
										{
											//Finally we can access the real image filename.
											domImage* pImage;
											error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
											cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

											pMaterial->specularColor.m_R = 1.0f;
											pMaterial->specularColor.m_G = 1.0f;
											pMaterial->specularColor.m_B = 1.0f;
											pMaterial->specularColor.m_A = 1.0f;
											pMaterial->specularMapName = pImage->getInit_from()->getValue().getFile();
										}
									}
								}
							}
						}
					}
					else
					{
						color = pBlinn->getSpecular()->getColor()->getValue();
						cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
						
						pMaterial->specularColor.m_R = (XF32)color[0];
						pMaterial->specularColor.m_G = (XF32)color[1];
						pMaterial->specularColor.m_B = (XF32)color[2];
						pMaterial->specularColor.m_A = (XF32)color[3];
					}
					
					cout << "SHININESS (" << pBlinn->getShininess()->getFloat()->getValue() << ")" << endl;

					//REFLECTIVE
					cout << "REFLECTIVE ";
					if (!pBlinn->getReflective()->getColor())
					{
						//Determine image information....first find the sampler name from the texture element.
						xsNCName samplerName = pBlinn->getReflective()->getTexture()->getTexture();
						for (unsigned int i = 0; i < paramArray.getCount();++i)
						{
							if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
							{
								//Now find the surface name from the 2d sampler
								xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
								for (unsigned int j = 0; j < paramArray.getCount();++j)
								{
									if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
									{
										//Now find the surface name from the 2d sampler
										domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
										if (initArray.getCount() != 1)
											FATAL_ERROR("Expected exactly 1 element in initFrom array")
										else
										{
											//Finally we can access the real image filename.
											domImage* pImage;
											error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
											cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

											pMaterial->reflectiveColor.m_R = 1.0f;
											pMaterial->reflectiveColor.m_G = 1.0f;
											pMaterial->reflectiveColor.m_B = 1.0f;
											pMaterial->reflectiveColor.m_A = 1.0f;
											pMaterial->reflectiveMapName = pImage->getInit_from()->getValue().getFile();
										}
									}
								}
							}
						}
						/*domImage* pImage;
						error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,pBlinn->getReflective()->getTexture()->getTexture() ,NULL);
						cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";
						cout << "(" << pBlinn->getReflective()->getTexture()->getTexcoord() << ") " << endl;

						pMaterial->reflectiveColor.m_R = 1.0f;
						pMaterial->reflectiveColor.m_G = 1.0f;
						pMaterial->reflectiveColor.m_B = 1.0f;
						pMaterial->reflectiveColor.m_A = 1.0f;
						pMaterial->reflectiveMapName = pImage->getInit_from()->getValue().getFile();*/
					}
					else
					{
						color = pBlinn->getReflective()->getColor()->getValue();
						cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
						
						pMaterial->reflectiveColor.m_R = (XF32)color[0];
						pMaterial->reflectiveColor.m_G = (XF32)color[1];
						pMaterial->reflectiveColor.m_B = (XF32)color[2];
						pMaterial->reflectiveColor.m_A = (XF32)color[3];
					}

					//TRANSPARENT
					cout << "TRANSPARENT ";
					if (!pBlinn->getTransparent()->getColor())
					{
						//Determine image information....first find the sampler name from the texture element.
						xsNCName samplerName = pBlinn->getTransparent()->getTexture()->getTexture();
						for (unsigned int i = 0; i < paramArray.getCount();++i)
						{
							if (strcmp(paramArray.get(i)->getSid(),samplerName)==0)
							{
								//Now find the surface name from the 2d sampler
								xsNCName surfaceName = paramArray.get(i)->getSampler2D()->getSource()->getValue();
								for (unsigned int j = 0; j < paramArray.getCount();++j)
								{
									if (strcmp(paramArray.get(j)->getSid(),surfaceName)==0)
									{
										//Now find the surface name from the 2d sampler
										domFx_surface_common_complexType::domInit_from_Array& initArray = paramArray.get(j)->getSurface()->getInit_from_array();
										if (initArray.getCount() != 1)
											FATAL_ERROR("Expected exactly 1 element in initFrom array")
										else
										{
											//Finally we can access the real image filename.
											domImage* pImage;
											error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,initArray.get(0)->getValue().getID(),NULL);
											cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";

											pMaterial->transparentColor.m_R = 1.0f;
											pMaterial->transparentColor.m_G = 1.0f;
											pMaterial->transparentColor.m_B = 1.0f;
											pMaterial->transparentColor.m_A = 1.0f;
											pMaterial->transparentMapName = pImage->getInit_from()->getValue().getFile();
										}
									}
								}
							}
						}
					}
					else
					{
						color = pBlinn->getTransparent()->getColor()->getValue();
				 		cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
						
						pMaterial->transparentColor.m_R = (XF32)color[0];
						pMaterial->transparentColor.m_G = (XF32)color[1];
						pMaterial->transparentColor.m_B = (XF32)color[2];
						pMaterial->transparentColor.m_A = (XF32)color[3];
					}

					//TRANSPARENCY
					cout << "TRANSPARENCY ";
					if (!pBlinn->getTransparency()->getParam())
					{
						cout << "(" << pBlinn->getTransparency()->getFloat()->getValue() << ")" << endl;
					}
					else
					{
						FATAL_ERROR("Don't know how to process \"param\" type for TRANPARENCY");
					}

					continue;
				}

				//Is there a lambert element?
				domProfile_COMMON::domTechnique::domLambert* pLambert = technique->getLambert();
				if (pLambert)
				{	
					cout << "Found lambert element." << endl;
					domFx_color_common color;

					//DIFFUSE
					cout << "DIFFUSE" << endl;
					if (!pLambert->getDiffuse()->getColor())
					{
						domImage* pImage;
						error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,pLambert->getDiffuse()->getTexture()->getTexture() ,NULL);
						cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";
						cout << "(" << pLambert->getDiffuse()->getTexture()->getTexcoord() << ") " << endl;

						pMaterial->diffuseColor.m_R = 1.0f;
						pMaterial->diffuseColor.m_G = 1.0f;
						pMaterial->diffuseColor.m_B = 1.0f;
						pMaterial->diffuseColor.m_A = 1.0f;
						pMaterial->diffuseMapName = pImage->getInit_from()->getValue().getFile();
					}
					else
					{
						color = pLambert->getDiffuse()->getColor()->getValue();
						cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
						
						pMaterial->diffuseColor.m_R = (XF32)color[0];
						pMaterial->diffuseColor.m_G = (XF32)color[1];
						pMaterial->diffuseColor.m_B = (XF32)color[2];
						pMaterial->diffuseColor.m_A = (XF32)color[3];
					}

					//AMBIENT
					cout << "AMBIENT" << endl;
					if (!pLambert->getAmbient()->getColor())
					{
						domImage* pImage;
						error = g_pInputDAE->getDatabase()->getElement((daeElement**)&pImage,0,pLambert->getAmbient()->getTexture()->getTexture() ,NULL);
						cout << "(" << pImage->getInit_from()->getValue().getFile() << ") ";
						cout << "(" << pLambert->getAmbient()->getTexture()->getTexcoord() << ") " << endl;

						pMaterial->diffuseColor.m_R = 1.0f;
						pMaterial->diffuseColor.m_G = 1.0f;
						pMaterial->diffuseColor.m_B = 1.0f;
						pMaterial->diffuseColor.m_A = 1.0f;
						pMaterial->diffuseMapName = pImage->getInit_from()->getValue().getFile();
					}
					else
					{
						color = pLambert->getAmbient()->getColor()->getValue();
						cout << "(" << color[0] << " "  << color[1] << " "  << color[2] << " "  << color[3] << ")" << endl;
						
						pMaterial->ambientColor.m_R = (XF32)color[0];
						pMaterial->ambientColor.m_G = (XF32)color[1];
						pMaterial->ambientColor.m_B = (XF32)color[2];
						pMaterial->ambientColor.m_A = (XF32)color[3];
					}

					continue;
				}

				//If we make it here then we didn't find a supported element
				FATAL_ERROR("NO ELEMENT FOUND: Looking for <phong> or <lambert> elements");
			}
			else
				FATAL_ERROR("Unrecognized profile in effect");
		}
	}
}



//*****************************************************************************
//*****************************************************************************
void DAEParseGeometryLib(DAEGeometryLib& g_geometryLib)
{
	int error;

	//How many geometry elements are there?
	int geometryElementCount = g_pInputDAE->getDatabase()->getElementCount(NULL,"geometry",NULL);
	cout  << "\n\nFound " << geometryElementCount << " geometry elements." << endl; 

	//Iterate over all the geometry elements
	for(int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
	{
		//Push a new geometry object into the geometry lib
		DAEGeometry newGeom;
		g_geometryLib.geometryList.push_back(newGeom);

		//Get a pointer to the new geom object
		DAEGeometry* pGeom = &g_geometryLib.geometryList[g_geometryLib.geometryList.size()-1];

		//Get the next geometry element
		domGeometry* thisGeometry = NULL;
		error = g_pInputDAE->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry");
		if (!thisGeometry)
			FATAL_ERROR("NULL geometry pointer returned from g_pInputDAE->getDatabase()->getElement");

		//Get name of geometry element
		if (!thisGeometry->getID())
			FATAL_ERROR("Geometry doesn't have a name")
		else
		{
			pGeom->geometryName = thisGeometry->getID();
			cout << "\n--------------------------------------------------\nProcessing " << pGeom->geometryName << endl;			
		}
 
		// Get the geometry element's mesh
		domMesh *thisMesh = thisGeometry->getMesh();
		if (!thisMesh)
			FATAL_ERROR("Couldn't get mesh from geometry");

		// Loop over all the triangle elements in this mesh
		int trianglesElementCount = (int)(thisMesh->getTriangles_array().getCount());
		//if (trianglesElementCount != 1)
		//	FATAL_ERROR("Mesh must contain exactly 1 triangle set");

		for(int currentTriangles = 0; currentTriangles < trianglesElementCount; currentTriangles++)
		{
			// Get the next triangles out of the mesh
			domTriangles *thisTriangles = thisMesh->getTriangles_array().get(currentTriangles);  
			
			DAETriangles newTriangles;
			pGeom->triangleLists.push_back(newTriangles);
			DAETriangles* pTriangles = &pGeom->triangleLists[pGeom->triangleLists.size()-1];

			//Material
			if (thisTriangles->getMaterial())
			{
				cout << "Material: " << thisTriangles->getMaterial() << endl;
				pTriangles->materialName = thisTriangles->getMaterial();
			}

			//Cache each input source
			int inputCount = (int)(thisTriangles->getInput_array().getCount());
			int numPrimitives = (int)(thisTriangles->getCount());
			
			cout << "Num inputs: " << inputCount << endl;
			cout << "Num primitives: " << numPrimitives << endl << endl;

			for(int inputNumber=0; inputNumber < inputCount; inputNumber++)
			{
				domSource *thisSource;
				int stride = 0;
				std::vector<XVector4D>* pReceivingList = NULL;
				domInputLocalOffset *thisInput = thisMesh->getTriangles_array().get(0)->getInput_array()[inputNumber];
				
				//Find the source for thisInput and verify that we support this input
				thisInput->getSource().resolveElement();
				if(strcmp(thisInput->getSemantic(),"VERTEX")==0)
				{
					//Set the receiving list
					pReceivingList = &pGeom->vertexList;
					
					//Cache the input list pointer for late and check to make sure the order
					//it appeared in is the same as the offset
					pTriangles->inputLists.push_back(pReceivingList);
					//if (pTriangles->inputLists.size()-1 != thisInput->getOffset())
					//	FATAL_ERROR("Inputs are out of order for triangles element");

					//Set the list that will recieve the indices later 
					pTriangles->outputLists.push_back(&pTriangles->vertexIndices);
					pTriangles->listTypes.push_back(IDT_VERTEX);

					//If we've already cached a vertex input then we assume this is the 
					//same one.  We only support one vertex input so continue.
					if (pGeom->vertexList.size() != 0)
						continue;

					cout << "Caching VERTEX Input ";
					daeElement * thisElement = thisInput->getSource().getElement();
					domVertices * thisVertices = (domVertices*)thisElement; 
					thisVertices->getInput_array().get(0)->getSource().resolveElement();
					thisElement = thisVertices->getInput_array().get(0)->getSource().getElement();
					thisSource = (domSource *)thisElement;

					//Make sure the stride is what we expect
					stride = thisSource->getTechnique_common()->getAccessor()->getStride();
					if (stride != 3)
						FATAL_ERROR("Unexpected stride found in VERTEX input");

					//We can only process float arrays
					domFloat_array* thisArray = thisSource->getFloat_array();
					if (!thisArray)
						FATAL_ERROR("Expected float type for vertex")
					else
					{
						cout << "(" << thisArray->getCount()/stride << ")" << endl;
					}

					domListOfFloats& list = thisArray->getValue();
					pReceivingList->reserve(thisArray->getCount()/stride);
					for (domUint i = 0;i < thisArray->getCount()/stride;++i)
					{
						pReceivingList->push_back(XVector3D((XF32)list[i*stride],(XF32)list[i*stride+2],(XF32)list[i*stride+1]));
						//cout << "(" << i << ") " << list[i*stride] << " " << list[i*stride+1] << " " << list[i*stride+2] << endl;				
					}
					cout << endl; 
				}
				else
				{
					if(strcmp(thisInput->getSemantic(),"NORMAL")==0)
					{
						//Set the receiving list
						pReceivingList = &pGeom->normalList;
						
						//Cache the input list pointer for late and check to make sure the order
						//it appeared in is the same as the offset
						pTriangles->inputLists.push_back(pReceivingList);
						//if (pTriangles->inputLists.size()-1 != thisInput->getOffset())
						//	FATAL_ERROR("Inputs are out of order for triangles element");
						
						//Set the list that will recieve the indices later 
						pTriangles->outputLists.push_back(&pTriangles->normalIndices);
						pTriangles->listTypes.push_back(IDT_NORMAL);

						//If we've already cached a vertex input then we assume this is the 
						//same one.  We only support one vertex input so continue.
						if (pGeom->normalList.size() != 0)
							continue;

						cout << "Caching NORMAL Input ";
						
						// We are referencing the source directly
						daeElement * thisElement = thisInput->getSource().getElement();
						thisSource = (domSource *)thisElement;
						
						//Make sure the stride is what we expect
						stride = thisSource->getTechnique_common()->getAccessor()->getStride();
						if (stride != 3)
							FATAL_ERROR("Unexpected stride found in NORMAL input");		

						//We can only process float arrays
						domFloat_array* thisArray = thisSource->getFloat_array();
						if (!thisArray)
							FATAL_ERROR("Expected float type for normals")
						else
						{
							cout << "(" << thisArray->getCount()/stride << ")" << endl;
						}

						domListOfFloats& list = thisArray->getValue();
						pReceivingList->reserve(thisArray->getCount()/stride);
						for (domUint i = 0;i < thisArray->getCount()/stride;++i)
						{
							pReceivingList->push_back(XVector3D((XF32)list[i*stride],(XF32)list[i*stride+2],(XF32)list[i*stride+1]));
							//cout << "(" << i << ") " << list[i*stride] << " " << list[i*stride+1] << " " << list[i*stride+2] << endl;				
						}
						cout << endl;
					}
					else if(strcmp(thisInput->getSemantic(),"TEXCOORD")==0)
					{
						domUint setNum = thisInput->getSet();
						if (setNum >= MAX_TEX_COORD_SETS)
							FATAL_ERRORF("Unexpected texcoord set number %d.  Only 0-7 are supported",setNum);

						//Set the receiving list
						pReceivingList = &pGeom->texCoordLists[setNum];

						//Cache the input list pointer for late and check to make sure the order
						//it appeared in is the same as the offset
						pTriangles->inputLists.push_back(pReceivingList);
						//if (pTriangles->inputLists.size()-1 != thisInput->getOffset())
						//	FATAL_ERROR("Inputs are out of order for triangles element");

						//Set the list that will recieve the indices later 
						pTriangles->outputLists.push_back(&pTriangles->texCoordIndices[setNum]);
						switch (setNum)
						{
						case 0:
							pTriangles->listTypes.push_back(IDT_TEXCOORD0);
							break;
						case 1:
							pTriangles->listTypes.push_back(IDT_TEXCOORD1);
							break;
						case 2:
							pTriangles->listTypes.push_back(IDT_TEXCOORD2);
							break;
						case 3:
							pTriangles->listTypes.push_back(IDT_TEXCOORD3);
							break;
						case 4:
							pTriangles->listTypes.push_back(IDT_TEXCOORD4);
							break;
						case 5:
							pTriangles->listTypes.push_back(IDT_TEXCOORD5);
							break;
						case 6:
							pTriangles->listTypes.push_back(IDT_TEXCOORD6);
							break;
						case 7:
							pTriangles->listTypes.push_back(IDT_TEXCOORD7);
							break;
						default:
							FATAL_ERROR("Didn't recognize tex coord set number");
							break;
						}
						
						//Have we seen this set before?  If so, continue since it's already
						//cached, otherwise we need to cache it.
						if (pGeom->texCoordLists[setNum].size() != 0)
							continue;
						
						cout << "Caching TEXCOORD Input Set " << setNum << " ";
						
						// We are referencing the source directly
						daeElement * thisElement = thisInput->getSource().getElement();
						thisSource = (domSource *)thisElement;
						
						//Make sure the stride is what we expect
						stride = thisSource->getTechnique_common()->getAccessor()->getStride();
						if (stride != 2)
							FATAL_ERRORF("Unexpected stride %d found in TEXCOORD input",stride);

						//We can only process float arrays
						domFloat_array* thisArray = thisSource->getFloat_array();
						if (!thisArray)
							FATAL_ERROR("Expected float type for tex coordinates")
						else
						{
							cout << "(" << thisArray->getCount()/stride << ")" << endl;
						}

						domListOfFloats& list = thisArray->getValue();
						pReceivingList->reserve(thisArray->getCount()/stride);
						for (domUint i = 0;i < thisArray->getCount()/stride;++i)
						{
							pReceivingList->push_back(XVector3D((XF32)list[i*stride],1.0f - (XF32)list[i*stride+1],0));
							//cout << "(" << i << ") " << list[i*stride] << " " << list[i*stride+1] << " " << list[i*stride+2] << endl;				
						}
						cout << endl;
					}
					else if(strcmp(thisInput->getSemantic(),"COLOR")==0)
					{
						domUint setNum = thisInput->getSet();
						if (setNum >= MAX_COLOR_SETS)
							FATAL_ERRORF("Unexpected color set number %d.  Only 0-7 are supported",setNum);

						//Set the receiving list
						pReceivingList = &pGeom->colorLists[setNum];

						//Cache the input list pointer for late and check to make sure the order
						//it appeared in is the same as the offset
						pTriangles->inputLists.push_back(pReceivingList);
						//if (pTriangles->inputLists.size()-1 != thisInput->getOffset())
						//	FATAL_ERROR("Inputs are out of order for triangles element");

						//Set the list that will recieve the indices later 
						pTriangles->outputLists.push_back(&pTriangles->colorIndices[setNum]);
						switch (setNum)
						{
						case 0:
							pTriangles->listTypes.push_back(IDT_COLOR0);
							break;
						case 1:
							pTriangles->listTypes.push_back(IDT_COLOR1);
							break;
						case 2:
							pTriangles->listTypes.push_back(IDT_COLOR2);
							break;
						case 3:
							pTriangles->listTypes.push_back(IDT_COLOR3);
							break;
						case 4:
							pTriangles->listTypes.push_back(IDT_COLOR4);
							break;
						case 5:
							pTriangles->listTypes.push_back(IDT_COLOR5);
							break;
						case 6:
							pTriangles->listTypes.push_back(IDT_COLOR6);
							break;
						case 7:
							pTriangles->listTypes.push_back(IDT_COLOR7);
							break;
						default:
							FATAL_ERROR("Didn't recognize color set number");
							break;
						}
						
						//Have we seen this set before?  If so, continue since it's already
						//cached, otherwise we need to cache it.
						if (pGeom->colorLists[setNum].size() != 0)
							continue;
						
						cout << "Caching COLOR Input Set " << setNum << " ";
						
						// We are referencing the source directly
						daeElement * thisElement = thisInput->getSource().getElement();
						thisSource = (domSource *)thisElement;
						
						//Make sure the stride is what we expect
						stride = thisSource->getTechnique_common()->getAccessor()->getStride();
						if (stride != 4)
							FATAL_ERRORF("Unexpected stride %d found in COLOR input",stride);

						//We can only process float arrays
						domFloat_array* thisArray = thisSource->getFloat_array();
						if (!thisArray)
							FATAL_ERROR("Expected float type for color")
						else
						{
							cout << "(" << thisArray->getCount()/stride << ")" << endl;
						}

						domListOfFloats& list = thisArray->getValue();
						pReceivingList->reserve(thisArray->getCount()/stride);
						for (domUint i = 0;i < thisArray->getCount()/stride;++i)
						{
							pReceivingList->push_back(XVector4D((XF32)list[i*stride],(XF32)list[i*stride+1],(XF32)list[i*stride+2],(XF32)list[i*stride+3]));
							//cout << "(" << i << ") " << list[i*stride] << " " << list[i*stride+1] << " " << list[i*stride+2] << endl;				
						}
						cout << endl;
					}
					else 
					{
						//Didn't recognize the input semantic.  This is a problem
						//since we won't know some of the data in the primitive array when
						//we get to it.
						string errorString = "Unknown input semantic -> ";
						errorString += thisInput->getSemantic();
						FATAL_ERROR(errorString);
					}
				}		
			}
			
			//Iterate over all the primitives and split the indices into their respective
			//lists.
			cout << "\nSplitting primitive array (" << numPrimitives << ")" << endl;
			domListOfUInts primList = thisTriangles->getP()->getValue();
			domUint value;
			domUint offset = 0;
			int vertIndex;
			for(int currentPrimitive = 0; currentPrimitive < numPrimitives; currentPrimitive++)
			{
				vertIndex = currentPrimitive * 3;
				for(int i = 0;i < inputCount;++i)
				{
					pTriangles->outputLists[i]->push_back(primList[vertIndex]);
					pTriangles->outputLists[i]->push_back(primList[vertIndex+1]);
					pTriangles->outputLists[i]->push_back(primList[vertIndex+2]);
				}
			
				/*offset = 0;
				currentTriangleIndex = currentPrimitive * 3 * inputCount;
				
				for(int i = 1;i <= 3;++i)
				{
					for(int currentIndex = offset; currentIndex < inputCount*i; currentIndex++)
					{
						value = primList[currentTriangleIndex + currentIndex];

						//Put this index into the appropriate list
						//pTriangles->inputLists[(currentTriangleIndex + currentIndex)%3][value];
						pTriangles->outputLists[(currentTriangleIndex + currentIndex)%inputCount]->push_back(value);
						//cout << value << " ";
					}
					offset += inputCount;
				}
				*/
				//cout << endl;
			}
		}
	}
}



//*****************************************************************************
//*****************************************************************************
void DAEParseAnimationLib(DAEAnimationLib& animationLib)
{
	int error;

	//How many animations are there?
	int animationElementCount = g_pInputDAE->getDatabase()->getElementCount(NULL,"animation",NULL);
	cout  << "\n\nFound " << animationElementCount << " animations.\n"; 

	//Iterate over all the animation elements
	for(int currentAnim = 0; currentAnim < animationElementCount; currentAnim++)
	{
		//Get the next animation element
		domAnimation* thisAnimation;
		error = g_pInputDAE->getDatabase()->getElement((daeElement**)&thisAnimation,currentAnim, NULL, "animation");
		
		size_t numSources = thisAnimation->getSource_array().getCount();
		size_t numSamplers = thisAnimation->getSampler_array().getCount();
		size_t numChannels = thisAnimation->getChannel_array().getCount();

		//Currently we only support 1 channel
		if (numChannels != 1)
		{
			cout << "%" << numChannels << endl;
			continue;
			//FATAL_ERRORF("Num channels must be 1 but found %d",numChannels);
		}

		//Push a new animation object into the animation lib
		DAEAnimation newAnimation;
		g_animationLib.animationList.push_back(newAnimation);

		//Get a pointer to the new animation object
		DAEAnimation* pAnim = &g_animationLib.animationList[g_animationLib.animationList.size()-1];


		for(unsigned int currentChannel = 0;currentChannel < numChannels;++currentChannel)
		{
			domChannel* pChannel = thisAnimation->getChannel_array()[currentChannel];

			if (pChannel->getTarget())
			cout << "\n--------------------------------------------------\nProcessing " << pChannel->getTarget() << endl;			
		
			pAnim->name = pChannel->getTarget();
			size_t nameLength = pAnim->name.find("/");
			if (nameLength != 0)
				pAnim->name.erase(nameLength);
			
			DAENode* pNode = g_nodeLib.findNodeByName(pAnim->name);
			if (!pNode)
				FATAL_ERROR("Cannot find animation target in visual scene");
			pAnim->ID = pNode->ID;

			cout << pAnim->ID << endl;
			cout << pAnim->name << endl;

			cout << "Num Sources: " << numSources << endl;
			cout << "Num Samplers: " << numSamplers << endl;
			cout << "Num Channels: " << numChannels << endl << endl;

			daeElement* pElement = NULL;
			domSampler*	pSampler = NULL;

			//Find sampler
			pChannel->getSource().resolveElement();
			pElement = pChannel->getSource().getElement();		// Direct casting (domSampler*)(pChannel->source.element) won't work, so we need a temporary variable...
			pSampler = (domSampler*)pElement;

			if(pSampler)
			{
				//CvSplineCV*		LKeyFrames = NULL;
				domSource* pSource = NULL;
				size_t numInputs = pSampler->getInput_array().getCount();
				int	numKeyFrames = 0;

				//We only support linear interpolation so we are only interested in the INPUT and OUTPUT
				//sources.  We also assume that INPUT is always a float type representing time and
				//OUTPUT is always the target's transform at that time (of type float4x4)
				for(unsigned int currentInput = 0;currentInput < numInputs;++currentInput)
				{
					domInputLocal*	pInput = pSampler->getInput_array()[currentInput];
					pInput->getSource().resolveElement();
					
					//First, if we find a semantic we don't support we can safely skip it, we only
					//support linear interpolation
					if ((strcmp("INTERPOLATION",pInput->getSemantic()) == 0) || 
						(strcmp("IN_TANGENT",pInput->getSemantic()) == 0) ||
						(strcmp("OUT_TANGENT",pInput->getSemantic()) == 0))
						continue;

					//Check only for "INPUT" and "OUTPUT" semantics
					if (strcmp("INPUT",pInput->getSemantic()) != 0 && strcmp("OUTPUT",pInput->getSemantic()) != 0)
						FATAL_ERROR("Found a semantic other than INPUT, OUTPUT or INTERPOLATION")
					else
						cout << pInput->getSemantic() << endl;

					pElement = pInput->getSource().getElement();		// (domSampler*)(pChannel->source.element) won't work, so we need a temporary variable...
					pSource = (domSource*)pElement;

					if(pSource)
					{
						domSource::domTechnique_common*	pTechniqueCommon = pSource->getTechnique_common();
						if(pTechniqueCommon)
						{
							domAccessor* pAccesor = pTechniqueCommon->getAccessor();
							int stride = pAccesor->getStride();
							int offset = pAccesor->getOffset();

							daeString paramType = pAccesor->getParam_array()[0]->getType();

							//The paramType must be a float array 
							if (strcmp(paramType,"float") != 0 && strcmp(paramType,"float4x4") != 0)
								FATAL_ERROR("Only float and float4x4 param types are supported for animation input and output");
							
							if(pAccesor)
							{
								pAccesor->getSource().resolveElement();
								pElement = pAccesor->getSource().getElement();		// (domArray*)(pAccesor->source.element) won't work, so we need a temporary variable...
								domFloat_array*	floatArray = (domFloat_array*)pElement;
									
								if (numKeyFrames == 0)
								{
									numKeyFrames = pAccesor->getCount();
									if (numKeyFrames)
										cout << "Num Key Frames: " << numKeyFrames << endl;
								}
												
								if (floatArray)
								{
									domFloat* pFloats = (domFloat*)(floatArray->getValue().getRawData());

									if (strcmp(pInput->getSemantic(), "INPUT") == 0)
									{
										for (int currentFrame = 0;currentFrame < numKeyFrames;++currentFrame)
										{
											pAnim->timeList.push_back((const XF32)pFloats[currentFrame*stride+offset]);
											//cout << "(" << currentFrame*stride+offset << ")" << " " << pFloats[currentFrame*stride+offset] << endl;
										}
										cout << endl;
									}
									else if(strcmp(pInput->getSemantic(), "OUTPUT") == 0)
									{
										for (int currentFrame = 0;currentFrame < numKeyFrames;++currentFrame)
										{
											XMatrix4x4 matrix;
											DAEMatrixConvert(&pFloats[currentFrame*stride+offset],matrix);
											
											//Pull the sampled animation matrix back into object space
											DAENode* pNode = g_nodeLib.findNodeByName(pAnim->name);
											if (!pNode)
												FATAL_ERROR("Could not find node referenced by animation")
											else
											{
												matrix = matrix * pNode->modelTransform.getInverse();											
											}

											//Decompose transform into individual rotation, translation
											//and scale components.
											pAnim->rotKeyList.push_back(matrix.toQuat());
											pAnim->posKeyList.push_back(XVector3D(matrix._41,matrix._42,matrix._43));

											XVector3D scaleVec;
											scaleVec.x = XVector3D(matrix.row1).mag();
											scaleVec.y = XVector3D(matrix.row2).mag();
											scaleVec.z = XVector3D(matrix.row3).mag();
											pAnim->scaleKeyList.push_back(scaleVec);

											/*cout << "(" << (currentFrame*stride+offset) / 16 << ")" << " "
											<< pFloats[currentFrame*stride+offset] << " "
											<< pFloats[currentFrame*stride+offset+1] << " " 
											<< pFloats[currentFrame*stride+offset+2] << " " 
											<< pFloats[currentFrame*stride+offset+3] << " " 
											<< pFloats[currentFrame*stride+offset+4] << " " 
											<< pFloats[currentFrame*stride+offset+5] << " " 
											<< pFloats[currentFrame*stride+offset+6] << " " 
											<< pFloats[currentFrame*stride+offset+7] << " " 
											<< pFloats[currentFrame*stride+offset+8] << " " 
											<< pFloats[currentFrame*stride+offset+9] << " " 
											<< pFloats[currentFrame*stride+offset+10] << " " 
											<< pFloats[currentFrame*stride+offset+11] << " " 
											<< pFloats[currentFrame*stride+offset+12] << " " 
											<< pFloats[currentFrame*stride+offset+13] << " " 
											<< pFloats[currentFrame*stride+offset+14] << " " 
											<< pFloats[currentFrame*stride+offset+15]
											<< endl;*/
										}
										cout << endl;
									}
									else
										FATAL_ERROR("Didn't recognize animation input semantic");
								}
								else
									FATAL_ERROR("Couldn't get float or float4x4 array for animation");
							}
							else
								FATAL_ERROR("Couldn't get animation accessor");
						}
					}
					else
						FATAL_ERROR("Couldn't get animation source");
				}
			}
			else
				FATAL_ERROR("Couldn't get animation sampler");
		}
	}
}


//*****************************************************************************
//*****************************************************************************
void DAEParseModelObjects(domNode_Array& nodeArray,XU32& nextID)
{
	for (XU32 i = 0;i < nodeArray.getCount();++i)
	{
		//Skip certain nodes such as pivot nodes, etc..  If this node doesn't have
		//a name, don't export it and warn
		if (nodeArray[i]->getName())
		{
			if (strstr(nodeArray[i]->getName(),"_PIVOT") ||
				strstr(nodeArray[i]->getName(),"global-ambient-light_scene-node"))
				continue;
		}
		else
		{
			WARNING("Found a node without a name, it will not be exported.");
			continue;
		}

		DAENode newNode;
		g_nodeLib.nodeList.push_back(newNode);

		//Get a pointer to the new node object
		DAENode* pNode = &g_nodeLib.nodeList[g_nodeLib.nodeList.size()-1];

		//Get the ID of the node first
		if (!nodeArray[i]->getID())
			FATAL_ERROR("Node does not have ID");

		//If there is a skin controller don't worry about assigning or bumping the ID
		if (nodeArray[i]->getInstance_controller_array().getCount() == 0)
			pNode->ID = nextID++;
		pNode->name = nodeArray[i]->getID();
		cout << nodeArray[i]->getID();

		//Is this a bone? If so, store the bone name
		if (nodeArray[i]->getType() == NODETYPE_JOINT)
		{
			pNode->boneName = nodeArray[i]->getID();
			cout << nodeArray[i]->getID();
		}

		//Find parent if any
		if (nodeArray[i]->getXMLParentElement()->getMeta() == domNode::_Meta)
		{
			cout << " P-> " << nodeArray[i]->getXMLParentElement()->getID() << " (";
			pNode->parentName = nodeArray[i]->getXMLParentElement()->getID();
		}
		else 
		{
			//If a default root node was generated earlier, we need to hook these nodes
			//as children.
			if (g_defaultRootNodeGenerated)
			{
				pNode->parentName = DEFAULT_ROOT_NODE;
				cout << " P-> " << DEFAULT_ROOT_NODE << " (";
			}
			else 
				cout << " P-> NULL (";
		}

		//Do we have a matrix for this node?
		if (nodeArray[i]->getMatrix_array().getCount() != 0)
		{
			domListOfFloats& floatList = nodeArray[i]->getMatrix_array()[0]->getValue();	
			for (XU32 j = 0;j < 16;++j)
				cout << floatList[j] << " ";
			cout << ")" << endl;

			//Convert the matrix
			XMatrix4x4 mat;
			DAEMatrixConvert(&floatList[0],mat);

			pNode->modelTransform = mat;
			
			//Get instance geometry and find the geometry in the cached geometry lib.  If we
			//don't find any geometry attached to this node look to see if the 
			//geometry is attached to a child pivot node.  If we still don't see
			//any geometry, then this must be a dummy node.
			if (nodeArray[i]->getInstance_geometry_array().getCount() == 0)
			{
				for (XU32 j = 0;j < nodeArray[i]->getNode_array().getCount();++j)
				{
					//Look for a pivot node
					if (strstr(nodeArray[i]->getNode_array()[j]->getName(),"_PIVOT") && 
						strstr(nodeArray[i]->getNode_array()[j]->getName(),nodeArray[i]->getName()))
					{
						
						if (nodeArray[i]->getNode_array()[j]->getInstance_geometry_array().getCount() != 0)
						{
							nodeArray[i]->getNode_array()[j]->getInstance_geometry_array()[0]->getUrl().resolveElement();
							daeElement* pElement = nodeArray[i]->getNode_array()[j]->getInstance_geometry_array()[0]->getUrl().getElement();
							domGeometry* pDomGeometry = (domGeometry*)pElement;
							DAEGeometry* pGeom = g_geometryLib.findGeometry(pDomGeometry->getID());
							if (!pGeom)
								FATAL_ERROR("Couldn't find cached geometry for PIVOT node");

							pNode->geometryName = pDomGeometry->getID();

							//Save the pivot transform so we can pull verts back into pivot space later
							//when we parse nodes
							if (nodeArray[i]->getNode_array()[j]->getMatrix_array().getCount() != 0)
							{
								domListOfFloats& floatList = nodeArray[i]->getNode_array()[j]->getMatrix_array()[0]->getValue();	
								for (XU32 j = 0;j < 16;++j)
									cout << floatList[j] << " ";
								cout << ")" << endl;

								//Convert the matrix
								XMatrix4x4 mat;
								DAEMatrixConvert(&floatList[0],mat);

								pNode->pivotTransform = mat;
							}
						}
					}
				}
			}
			else
			{
				nodeArray[i]->getInstance_geometry_array()[0]->getUrl().resolveElement();
				daeElement* pElement = nodeArray[i]->getInstance_geometry_array()[0]->getUrl().getElement();
				domGeometry* pDomGeometry = (domGeometry*)pElement;
				DAEGeometry* pGeom = g_geometryLib.findGeometry(pDomGeometry->getID());
				if (!pGeom)
					FATAL_ERROR("Couldn't find cached geometry for node");

				pNode->geometryName = pDomGeometry->getID();
			}
		}
		else if (nodeArray[i]->getInstance_controller_array().getCount() != 0)
		{
			cout << "no matrix)" << endl;
			
			//We have an object that has no matrix, so we don't want to keep it for
			//the final hierarchy.  Remove the node we were preparing to fill and 
			//instead create a skin object in the skin library.
			DAESkin newSkin;
			g_skinLib.skinList.push_back(newSkin);
			DAESkin* pSkin = &g_skinLib.skinList[g_skinLib.skinList.size()-1];
			pSkin->name = pNode->name;
			pSkin->parentName = pNode->parentName;

			//Note: pNode is no longer valid.
			g_nodeLib.nodeList.pop_back();
			pNode = NULL;
			
			//Only one skin object is supported currently
			//if (g_skinLib.skinList.size() != 1)
			//	FATAL_ERROR("Only one skin object is currently supported");
				
			//Get the controller element
			daeElement* pElement = nodeArray[i]->getInstance_controller_array()[0]->getUrl().getElement();
			domController* thisController =  (domController*)pElement;
			
			//Is there a skin element?
			domSkin *pDomSkin = thisController->getSkin();
			if (pDomSkin)
			{
				cout << "\nFound skin..." << endl;
				
				domGeometry* pSourceGeom = NULL;
				daeString skinSource = pDomSkin->getID();

				//If the skin source is referenced in the node use it other wise we need to resolve
				//the source
				if (!skinSource)
				{
					pDomSkin->getSource().resolveElement();
					daeElementRef element = pDomSkin->getSource().getElement();
					pSkin->geometryName = element->getID();
				}
				else
				{
					g_pInputDAE->getDatabase()->getElement((daeElement**)&pSourceGeom,0, skinSource, "geometry");
					pSkin->geometryName = pSourceGeom->getID();
				}
				
				if (pSkin->geometryName != "")
				{
					cout << "Source geometry: " << pSkin->geometryName << endl;
				}
				else
					FATAL_ERROR("Could not find cached geometry for skin controller");

				//Get joints
				domSkin::domJoints* pJoints = pDomSkin->getJoints();
				if (pJoints)
				{
					cout << "Found joints" << endl;
					int inputCount = (int)(pJoints->getInput_array().getCount());
					cout << "Num Inputs: " << inputCount << endl;

					for(int inputNumber=0; inputNumber < inputCount; inputNumber++)
					{
						domSource *thisSource;
						domInputLocalRef thisInput = pJoints->getInput_array()[inputNumber];
						thisInput->getSource().resolveElement();

						// We are referencing the source directly
						daeElement * thisElement = thisInput->getSource().getElement();
						thisSource = (domSource *)thisElement;
						
						if(strcmp(thisInput->getSemantic(),"JOINT")==0)
						{
							cout << "\n\nJOINT" << endl;
							domUint stride = thisSource->getTechnique_common()->getAccessor()->getStride();
							daeString type = thisSource->getTechnique_common()->getAccessor()->getParam_array()[0]->getType();
							cout << "Stride: " << stride << endl;
							cout << "Type: " <<  type << endl;

							if (strcmp(type,"name") == 0)
							{
								domName_array * thisArray = thisSource->getName_array();
								cout << endl << thisArray->getID() << " NumEntrys: " << thisArray->getCount() << endl;
								domListOfNames& list = thisArray->getValue();

								//If we don't have any bones created yet, that means the joint element
								//has been found for the first time.  So, push on as many bones as we need.
								//We don't do this next time we encounter a joint element, which we surely
								//will.
								if (pSkin->bones.size() == 0)
								{
									for (XU32 i = 0;i < thisArray->getCount();++i) 
									{
										DAEBone b;
										pSkin->bones.push_back(b);
									}
								}

								//If we don't have any bone names, then we need to fill them in during
								//this encounter of the joint element, otherwise, just like above,
								//never do it again.
								if (pSkin->bones[0].name.size() == 0)
								{
									for (XU32 i = 0;i < thisArray->getCount();++i)
									{
										pSkin->bones[i].name = list[i];
										cout << list[i] << " ";		
									}
								}
							}
							else
								FATAL_ERROR("Unrecognized type for joint source");

						}
						else if (strcmp(thisInput->getSemantic(),"INV_BIND_MATRIX")==0)
						{
							cout << "\n\nINV_BIND_MATRIX" << endl;
							domUint stride = thisSource->getTechnique_common()->getAccessor()->getStride();
							daeString type = thisSource->getTechnique_common()->getAccessor()->getParam_array()[0]->getType();
							cout << "Stride: " << stride << endl;
							cout << "Type: " <<  type << endl;

							if (strcmp(type,"float4x4") == 0)
							{
								domFloat_array * thisArray = thisSource->getFloat_array();
								cout << endl << thisArray->getID() << " NumEntrys: " << thisArray->getCount() << endl;
								domListOfFloats& list = thisArray->getValue();
								
								//Here, we assume we have just as many inv bind transforms as we do joints. 
								//Therefore, the array of floats should be a multiple of 16.  Do a quick
								//sanity check
								if (thisArray->getCount()/16 != pSkin->bones.size())
									FATAL_ERROR("Number of inverse bind transforms and joints don't match");
								
								for (domUint i = 0;i < thisArray->getCount()/16;++i)
								{
									DAEMatrixConvert(&list[i*16],pSkin->bones[i].invBoneTransform);
									for (XU32 j = 0;j < 16;++j)
										cout << list[j+(i*16)] << " ";	
									cout << endl;
								}
							}
							else
								FATAL_ERROR("Unrecognized type for inverse bind matrix source");

						}
					}
				}
				else
					FATAL_ERROR("Could not find joints");

				//Get weights
				domSkin::domVertex_weights* pWeights = pDomSkin->getVertex_weights();
				domFloat_array* weightArray = NULL;
				if (pWeights)
				{
					cout << "\n\nFound weights..." << endl;
					int inputCount = (int)(pWeights->getInput_array().getCount());
					cout << "Num Inputs: " << inputCount << endl;

					for(int inputNumber=0; inputNumber < inputCount; inputNumber++)
					{
						domSource *thisSource;
						domInputLocalOffsetRef thisInput = pWeights->getInput_array()[inputNumber];
						thisInput->getSource().resolveElement();

						// We are referencing the source directly
						daeElement * thisElement = thisInput->getSource().getElement();
						thisSource = (domSource *)thisElement;
						
						if(strcmp(thisInput->getSemantic(),"JOINT")==0)
						{
							//At this point, we assume we have already found a joints element
							//but check to make sure.
							if (pSkin->bones.size() == 0)
								FATAL_ERROR("Found joint reference in weight element before joints were created.");
							
							/*
							cout << "\n\nJOINT" << endl;
							domUint stride = thisSource->getTechnique_common()->getAccessor()->getStride();
							daeString type = thisSource->getTechnique_common()->getAccessor()->getParam_array()[0]->getType();
							cout << "Stride: " << stride << endl;
							cout << "Type: " <<  type << endl;

							if (strcmp(type,"Name") == 0)
							{
								domName_array * thisArray = thisSource->getName_array();
								cout << endl << thisArray->getID() << " NumEntrys: " << thisArray->getCount() << endl;
								domListOfNames& list = thisArray->getValue();
								for (domUint i = 0;i < thisArray->getCount();++i)
								{
									cout << list[i] << " ";				
								}
							}
							else
								FATAL_ERROR("Unrecognized type for joint source");
								*/

						}
						else if (strcmp(thisInput->getSemantic(),"WEIGHT")==0)
						{
							cout << "\n\nWEIGHT" << endl;
							domUint stride = thisSource->getTechnique_common()->getAccessor()->getStride();
							daeString type = thisSource->getTechnique_common()->getAccessor()->getParam_array()[0]->getType();
							cout << "Stride: " << stride << endl;
							cout << "Type: " <<  type << endl;

							if (strcmp(type,"float") == 0)
							{
								weightArray = thisSource->getFloat_array();
								cout << endl << weightArray->getID() << " NumEntrys: " << weightArray->getCount() << endl;
								/*domListOfFloats& list = thisArray->getValue();
								for (domUint i = 0;i < thisArray->getCount();++i)
								{
									cout << list[i] << " ";				
								}*/
							}
							else
								FATAL_ERROR("Unrecognized type for inverse bind matrix source");
						}
					}

					//Sanity check to make sure we found a weight array.
					if (!weightArray)
						FATAL_ERROR("Could not find weight array");

					//Get vcount
					domSkin::domVertex_weights::domVcountRef vCount = pWeights->getVcount();
					domListOfUInts vCountList = vCount->getValue();
					cout << "\n\nVcount" << endl;
					XU32 numVCountEntries = (XU32)vCountList.getCount();			
					cout << "NumEntries: " << numVCountEntries << endl;
					
					/*for (XU32 i = 0;i < numVCountEntries;++i)
					{
						cout << vCountList[i] << " ";
					}
					cout << endl;*/

					//Get v
					domSkin::domVertex_weights::domVRef v = pWeights->getV();
					domListOfInts vList = v->getValue();

					cout << "\n\nV" << endl;
					XU32 numV = (XU32)vList.getCount();
					cout << "NumEntries: " << numV << endl;
					/*for (XU32 i = 0;i < numV;++i)
					{
						cout << vList[i] << " ";
					}
					cout << endl;
*/
					XU32 ctr = 0;
					for (XU32 i = 0;i < numVCountEntries;++i)
					{
						DAESkinVertex sv;
						pSkin->verts.push_back(sv);
						DAESkinVertex* pSkinVert = &pSkin->verts[pSkin->verts.size()-1];
						//cout << "V " << i << " (" << vCountList[i] << " bones)" << endl;
						for (XU32 j = 0;j < vCountList[i];++j)
						{
							DAEBoneWeight bw;

							//We don't support negative bone indices, which to COLLADA means
							//bind shape
							if (vList[ctr] < 0)
								FATAL_ERROR("-1 bone index is supported (COLLADA bind shape)");
							bw.boneID = vList[ctr++];
							bw.weight = (XF32)weightArray->getValue()[vList[ctr++]];

							//TODO: we might need to normalize the bone influences according
							//to COLLADA spec.

							//cout << bw.boneID << " " << bw.weight << endl;
							pSkinVert->boneWeights.push_back(bw);
						}
					}
				}
				else 
					FATAL_ERROR("Could not find vertex weights");
			}
			else
				FATAL_ERROR("Found a node with no matrix and no skin controller");
		}
		else
			FATAL_ERROR("Unexpected node.  Could not find matrix or skin controller");
		
		//Now recurse to next level if it exists
		if (nodeArray[i]->getNode_array().getCount() != 0)
			DAEParseModelObjects(nodeArray[i]->getNode_array(),nextID);	
	}
}



//*****************************************************************************
//*****************************************************************************
void DAEParseSkinMeshObjects(domNode_Array& nodeArray,XU32& nextID)
{
	for (XU32 i = 0;i < nodeArray.getCount();++i)
	{
		//Skip certain nodes such as pivot nodes, etc..
		if (strstr(nodeArray[i]->getName(),"_PIVOT") ||
			strstr(nodeArray[i]->getName(),"global-ambient-light_scene-node"))
			continue;

		DAENode newNode;
		g_nodeLib.nodeList.push_back(newNode);

		//Get a pointer to the new node object
		DAENode* pNode = &g_nodeLib.nodeList[g_nodeLib.nodeList.size()-1];

		//Get the ID of the node first
		if (!nodeArray[i]->getID())
			FATAL_ERROR("Node does not have ID");

		pNode->name = nodeArray[i]->getID();
		cout << nodeArray[i]->getID();

		if (nodeArray[i]->getSid())
		{
			pNode->boneName = nodeArray[i]->getSid();
			cout << nodeArray[i]->getSid();
		}

		//Find parent if any
		if (nodeArray[i]->getXMLParentElement()->getMeta() == domNode::_Meta)
		{
			cout << " P-> " << nodeArray[i]->getXMLParentElement()->getID() << " (";
			pNode->parentName = nodeArray[i]->getXMLParentElement()->getID();
		}
		else 
			cout << " P-> NULL (";


		//Do we have a matrix for this node?  If not, we should investigate this node
		//to see if it contains a skin controller
		if (nodeArray[i]->getMatrix_array().getCount() != 0)
		{
			domListOfFloats& floatList = nodeArray[i]->getMatrix_array()[0]->getValue();	
			for (XU32 j = 0;j < 16;++j)
				cout << floatList[j] << " ";
			cout << ")" << endl;

			//Convert the matrix
			XMatrix4x4 mat;
			DAEMatrixConvert(&floatList[0],mat);

			pNode->modelTransform = mat;
			pNode->ID = nextID++;
		}
		else if (nodeArray[i]->getInstance_controller_array().getCount() != 0)
		{
			cout << "no matrix)" << endl;
			
			//We have an object that has no matrix, so we don't want to keep it for
			//the final hierarchy.  Remove the node we were preparing to fill and 
			//instead create a skin object in the skin library.
			DAESkin newSkin;
			g_skinLib.skinList.push_back(newSkin);
			DAESkin* pSkin = &g_skinLib.skinList[g_skinLib.skinList.size()-1];
			pSkin->name = pNode->name;
			pSkin->parentName = pNode->parentName;

			//Note: pNode is no longer valid.
			g_nodeLib.nodeList.pop_back();
			pNode = NULL;
			
			//Only one skin object is supported currently
			if (g_skinLib.skinList.size() != 1)
				FATAL_ERROR("Only one skin object is currently supported");
				
			//Get the controller element
			daeElement* pElement = nodeArray[i]->getInstance_controller_array()[0]->getUrl().getElement();
			domController* thisController =  (domController*)pElement;
			
			//Is there a skin element?
			domSkin *pDomSkin = thisController->getSkin();
			if (pDomSkin)
			{
				cout << "\nFound skin..." << endl;
				
				domGeometry* pSourceGeom = NULL;
				g_pInputDAE->getDatabase()->getElement((daeElement**)&pSourceGeom,0, pDomSkin->getID(), "geometry");
				
				if (pSourceGeom)
				{
					pSkin->geometryName = pSourceGeom->getID();
					cout << "Source geometry: " << pSourceGeom->getID() << endl;
				}
				else
					FATAL_ERROR("Could not find cached geometry for skin controller");

				//Get joints
				domSkin::domJoints* pJoints = pDomSkin->getJoints();
				if (pJoints)
				{
					cout << "Found joints" << endl;
					int inputCount = (int)(pJoints->getInput_array().getCount());
					cout << "Num Inputs: " << inputCount << endl;

					for(int inputNumber=0; inputNumber < inputCount; inputNumber++)
					{
						domSource *thisSource;
						domInputLocalRef thisInput = pJoints->getInput_array()[inputNumber];
						thisInput->getSource().resolveElement();

						// We are referencing the source directly
						daeElement * thisElement = thisInput->getSource().getElement();
						thisSource = (domSource *)thisElement;
						
						if(strcmp(thisInput->getSemantic(),"JOINT")==0)
						{
							cout << "\n\nJOINT" << endl;
							domUint stride = thisSource->getTechnique_common()->getAccessor()->getStride();
							daeString type = thisSource->getTechnique_common()->getAccessor()->getParam_array()[0]->getType();
							cout << "Stride: " << stride << endl;
							cout << "Type: " <<  type << endl;

							if (strcmp(type,"Name") == 0)
							{
								domName_array * thisArray = thisSource->getName_array();
								cout << endl << thisArray->getID() << " NumEntrys: " << thisArray->getCount() << endl;
								domListOfNames& list = thisArray->getValue();

								//If we don't have any bones created yet, that means the joint element
								//has been found for the first time.  So, push on as many bones as we need.
								//We don't do this next time we encounter a joint element, which we surely
								//will.
								if (pSkin->bones.size() == 0)
								{
									for (XU32 i = 0;i < thisArray->getCount();++i) 
									{
										DAEBone b;
										pSkin->bones.push_back(b);
									}
								}

								//If we don't have any bone names, then we need to fill them in during
								//this encounter of the joint element, otherwise, just like above,
								//never do it again.
								if (pSkin->bones[0].name.size() == 0)
								{
									for (XU32 i = 0;i < thisArray->getCount();++i)
									{
										pSkin->bones[i].name = list[i];
										cout << list[i] << " ";		
									}
								}
							}
							else
								FATAL_ERROR("Unrecognized type for joint source");

						}
						else if (strcmp(thisInput->getSemantic(),"INV_BIND_MATRIX")==0)
						{
							cout << "\n\nINV_BIND_MATRIX" << endl;
							domUint stride = thisSource->getTechnique_common()->getAccessor()->getStride();
							daeString type = thisSource->getTechnique_common()->getAccessor()->getParam_array()[0]->getType();
							cout << "Stride: " << stride << endl;
							cout << "Type: " <<  type << endl;

							if (strcmp(type,"float4x4") == 0)
							{
								domFloat_array * thisArray = thisSource->getFloat_array();
								cout << endl << thisArray->getID() << " NumEntrys: " << thisArray->getCount() << endl;
								domListOfFloats& list = thisArray->getValue();
								
								//Here, we assume we have just as many inv bind transforms as we do joints. 
								//Therefore, the array of floats should be a multiple of 16.  Do a quick
								//sanity check
								if (thisArray->getCount()/16 != pSkin->bones.size())
									FATAL_ERROR("Number of inverse bind transforms and joints don't match");
								
								for (domUint i = 0;i < thisArray->getCount()/16;++i)
								{
									DAEMatrixConvert(&list[i*16],pSkin->bones[i].invBoneTransform);
									for (XU32 j = 0;j < 16;++j)
										cout << list[j+(i*16)] << " ";	
									cout << endl;
								}
							}
							else
								FATAL_ERROR("Unrecognized type for inverse bind matrix source");

						}
					}
				}
				else
					FATAL_ERROR("Could not find joints");

				//Get weights
				domSkin::domVertex_weights* pWeights = pDomSkin->getVertex_weights();
				domFloat_array* weightArray = NULL;
				if (pWeights)
				{
					cout << "\n\nFound weights..." << endl;
					int inputCount = (int)(pWeights->getInput_array().getCount());
					cout << "Num Inputs: " << inputCount << endl;

					for(int inputNumber=0; inputNumber < inputCount; inputNumber++)
					{
						domSource *thisSource;
						domInputLocalOffsetRef thisInput = pWeights->getInput_array()[inputNumber];
						thisInput->getSource().resolveElement();

						// We are referencing the source directly
						daeElement * thisElement = thisInput->getSource().getElement();
						thisSource = (domSource *)thisElement;
						
						if(strcmp(thisInput->getSemantic(),"JOINT")==0)
						{
							//At this point, we assume we have already found a joints element
							//but check to make sure.
							if (pSkin->bones.size() == 0)
								FATAL_ERROR("Found joint reference in weight element before joints were created.");
							
							/*
							cout << "\n\nJOINT" << endl;
							domUint stride = thisSource->getTechnique_common()->getAccessor()->getStride();
							daeString type = thisSource->getTechnique_common()->getAccessor()->getParam_array()[0]->getType();
							cout << "Stride: " << stride << endl;
							cout << "Type: " <<  type << endl;

							if (strcmp(type,"Name") == 0)
							{
								domName_array * thisArray = thisSource->getName_array();
								cout << endl << thisArray->getID() << " NumEntrys: " << thisArray->getCount() << endl;
								domListOfNames& list = thisArray->getValue();
								for (domUint i = 0;i < thisArray->getCount();++i)
								{
									cout << list[i] << " ";				
								}
							}
							else
								FATAL_ERROR("Unrecognized type for joint source");
								*/

						}
						else if (strcmp(thisInput->getSemantic(),"WEIGHT")==0)
						{
							cout << "\n\nWEIGHT" << endl;
							domUint stride = thisSource->getTechnique_common()->getAccessor()->getStride();
							daeString type = thisSource->getTechnique_common()->getAccessor()->getParam_array()[0]->getType();
							cout << "Stride: " << stride << endl;
							cout << "Type: " <<  type << endl;

							if (strcmp(type,"float") == 0)
							{
								weightArray = thisSource->getFloat_array();
								cout << endl << weightArray->getID() << " NumEntrys: " << weightArray->getCount() << endl;
								/*domListOfFloats& list = thisArray->getValue();
								for (domUint i = 0;i < thisArray->getCount();++i)
								{
									cout << list[i] << " ";				
								}*/
							}
							else
								FATAL_ERROR("Unrecognized type for inverse bind matrix source");
						}
					}

					//Sanity check to make sure we found a weight array.
					if (!weightArray)
						FATAL_ERROR("Could not find weight array");

					//Get vcount
					domSkin::domVertex_weights::domVcountRef vCount = pWeights->getVcount();
					domListOfUInts vCountList = vCount->getValue();
					cout << "\n\nVcount" << endl;
					XU32 numVCountEntries = (XU32)vCountList.getCount();			
					cout << "NumEntries: " << numVCountEntries << endl;
					
					/*for (XU32 i = 0;i < numVCountEntries;++i)
					{
						cout << vCountList[i] << " ";
					}
					cout << endl;*/

					//Get v
					domSkin::domVertex_weights::domVRef v = pWeights->getV();
					domListOfInts vList = v->getValue();

					cout << "\n\nV" << endl;
					XU32 numV = (XU32)vList.getCount();
					cout << "NumEntries: " << numV << endl;
					/*for (XU32 i = 0;i < numV;++i)
					{
						cout << vList[i] << " ";
					}
					cout << endl;
*/
					XU32 ctr = 0;
					for (XU32 i = 0;i < numVCountEntries;++i)
					{
						DAESkinVertex sv;
						pSkin->verts.push_back(sv);
						DAESkinVertex* pSkinVert = &pSkin->verts[pSkin->verts.size()-1];
						//cout << "V " << i << " (" << vCountList[i] << " bones)" << endl;
						for (XU32 j = 0;j < vCountList[i];++j)
						{
							DAEBoneWeight bw;

							//We don't support negative bone indices, which to COLLADA means
							//bind shape
							if (vList[ctr] < 0)
								FATAL_ERROR("-1 bone index is supported (COLLADA bind shape)");
							bw.boneID = vList[ctr++];
							bw.weight = (XF32)weightArray->getValue()[vList[ctr++]];

							//TODO: we might need to normalize the bone influences according
							//to COLLADA spec.

							cout << bw.boneID << " " << bw.weight << endl;
							pSkinVert->boneWeights.push_back(bw);
						}
					}
				}
				else 
					FATAL_ERROR("Could not find vertex weights");
			}
			else
				FATAL_ERROR("Found a node with no matrix and no skin controller");
		}
		else
			FATAL_ERROR("Unexpected node found while looking for skin mesh objects");
		
		
		//Now recurse to next level if it exists
		if (nodeArray[i]->getNode_array().getCount() != 0)
			DAEParseSkinMeshObjects(nodeArray[i]->getNode_array(),nextID);	
	}
}


//*****************************************************************************
//*****************************************************************************
void ExportModel(FILE* outFile)
{
	//ofstream f("test.xmf");

	//Order of parsing is important
	DAEParseMaterialLib();
	DAEParseGeometryLib(g_geometryLib);
	DAEParseModelVisualSceneLib();
	//DAEParseAnimationLib(g_animationLib);
	//f.close();

	//First, write out a blank header, later we'll come back and fill this header
	//in once we know the size of the model chunk.
	long beginOffset = ftell(outFile);
	XBSChunkHeader h;
	fwrite(&h,sizeof(h),1,outFile);

	//Now, write out the number of model objects
	XU32 numModelObjects = (XU32)g_nodeLib.nodeList.size();
	fwrite(&numModelObjects,sizeof(numModelObjects),1,outFile);
		
	//Now, write out materials
	ExportMaterials(outFile);

	//Followed by model objects/bones
	ExportModelObjects(outFile);

	//Followed by skin meshes
	ExportSkinMeshObjects(outFile);

	//Now, the model chunk has been streamed out, so we can rewind
	//back to the header and place the correct information into it.
	//Once we fill in the header, seek back to end of file.
	long endOffset = ftell(outFile);
	h.chunkID = XBS_MODEL;
	h.sizeInBytes = ((beginOffset-endOffset) *- 1) - sizeof(XBSChunkHeader);
	h.version = XBS_VERSION;
	fseek(outFile,beginOffset-endOffset,SEEK_CUR);
	fwrite(&h,sizeof(h),1,outFile);
	fseek(outFile,h.sizeInBytes,SEEK_CUR);
}



//*****************************************************************************
//*****************************************************************************
void ExportSkinMesh(FILE* outFile)
{
	//Order of parsing is important
	DAEParseMaterialLib();
	DAEParseGeometryLib(g_geometryLib);
	DAEParseSkinMeshVisualSceneLib();

	//First, write out a blank header, later we'll come back and fill this header
	//in once we know the size of the skin mesh chunk.
	long beginOffset = ftell(outFile);
	XBSChunkHeader h;
	fwrite(&h,sizeof(h),1,outFile);

	//Now, write out the number of bones
	XU32 numBones = (XU32)g_skinLib.skinList[0].bones.size();
	fwrite(&numBones,sizeof(numBones),1,outFile);

	//Now, write out materials
	ExportMaterials(outFile);

	//Followed by nodes
	ExportSkinMeshObjects(outFile);

	//Now, the skin mesh chunk has been streamed out, so we can rewind
	//back to the header and place the correct information into it.
	//Once we fill in the header, seek back to end of file.
	long endOffset = ftell(outFile);
	h.chunkID = XBS_SKIN_MESH;
	h.sizeInBytes = ((beginOffset-endOffset) *- 1) - sizeof(XBSChunkHeader);
	h.version = XBS_VERSION;
	fseek(outFile,beginOffset-endOffset,SEEK_CUR);
	fwrite(&h,sizeof(h),1,outFile);
	fseek(outFile,h.sizeInBytes,SEEK_CUR);
}



//*****************************************************************************
//*****************************************************************************
void ExportMaterials(FILE* outFile)
{
	//Export the entire material library to the passed in stream
	for (XU32 i = 0;i < g_materialLib.materialList.size();++i)
	{
		//Output material chunk ID and material name
		XBSMaterialChunk chunk;
		DAEMaterial* pMaterial = &g_materialLib.materialList[i];

		if (pMaterial->name.size() > XBS_MAX_NAME_LEN-1)
			FATAL_ERROR("Material name too long")
		else
			strcpy((char*)chunk.name,pMaterial->name.c_str());

		/*if (pMaterial->diffuseMapName.size() > 0)
		{
			//Check size
			if (pMaterial->diffuseMapName.size() > XBS_MAX_NAME_LEN-1)
				FATAL_ERROR("Diffuse map name too long");
			else
				strcpy((char*)chunk.diffuseMapName,pMaterial->diffuseMapName.c_str());
		}
		chunk.diffuseColor = pMaterial->diffuseColor;

		if (pMaterial->ambientMapName.size() > 0)
		{
			//Check size
			if (pMaterial->ambientMapName.size() > XBS_MAX_NAME_LEN-1)
				FATAL_ERROR("Ambient map name too long");
			else
				strcpy((char*)chunk.ambientMapName,pMaterial->ambientMapName.c_str());
		}
		chunk.ambientColor = pMaterial->ambientColor;

		if (pMaterial->specularMapName.size() > 0)
		{
			//Check size
			if (pMaterial->specularMapName.size() > XBS_MAX_NAME_LEN-1)
				FATAL_ERROR("Specular map name too long");
			else
				strcpy((char*)chunk.specularMapName,pMaterial->specularMapName.c_str());
		}
		chunk.specularColor = pMaterial->specularColor;

		if (pMaterial->reflectiveMapName.size() > 0)
		{
			//Check size
			if (pMaterial->reflectiveMapName.size() > XBS_MAX_NAME_LEN-1)
				FATAL_ERROR("Reflective map name too long");
			else
				strcpy((char*)chunk.reflectiveMapName,pMaterial->reflectiveMapName.c_str());
		}
		chunk.reflectiveColor = pMaterial->reflectiveColor;

		if (pMaterial->transparentMapName.size() > 0)
		{
			//Check size
			if (pMaterial->transparentMapName.size() > XBS_MAX_NAME_LEN-1)
				FATAL_ERROR("Transparent map name too long");
			else
				strcpy((char*)chunk.transparentMapName,pMaterial->transparentMapName.c_str());
		}
		chunk.transparentColor = pMaterial->transparentColor;

		chunk.transparency = pMaterial->transparency;
		chunk.shininess = pMaterial->shininess;
		*/

		//Stream out the header and chunk
		long beginOffset = ftell(outFile);
		XBSChunkHeader h;
		fwrite(&h,sizeof(h),1,outFile);
		fwrite(&chunk,sizeof(chunk),1,outFile);
		long endOffset = ftell(outFile);
		h.chunkID = XBS_MATERIAL;
		h.sizeInBytes = ((beginOffset-endOffset) *- 1) - sizeof(XBSChunkHeader);
		h.version = XBS_VERSION;
		fseek(outFile,beginOffset-endOffset,SEEK_CUR);
		fwrite(&h,sizeof(h),1,outFile);
		fseek(outFile,h.sizeInBytes,SEEK_CUR);
	}
}


//*****************************************************************************
//*****************************************************************************
void ExportModelObjects(FILE* outFile)
{
	//Export the entire node library to the passed in stream
	for (XU32 i = 0;i < g_nodeLib.nodeList.size();++i) 
	{
		DAENode* pNode = &g_nodeLib.nodeList[i];

		//Stream out the header and chunk
		long beginOffset = ftell(outFile);
		XBSChunkHeader h;
		fwrite(&h,sizeof(h),1,outFile);

		XBSModelObjectChunk modelObjectChunk;

		//Determine if we have model object or a bone.  If it's a bone, grab
		//the inverse bone transform matrix
		if (g_skinLib.skinList.size() != 0)
		{
			DAEBone* pBone = g_skinLib.skinList[0].findBone(pNode->boneName);
			if (pBone)
			{
				modelObjectChunk.isBone = true;
				modelObjectChunk.invBoneTransform = pBone->invBoneTransform;
			}
		}
		
		//Name
		if (pNode->name.size() > XBS_MAX_NAME_LEN-1)
			FATAL_ERROR("Node name too long")
		else
			strcpy((char*)modelObjectChunk.name,pNode->name.c_str());
		
		//Parent name
		if (pNode->parentName.size() > XBS_MAX_NAME_LEN-1)
			FATAL_ERROR("Parent node name too long")
		else
			strcpy((char*)modelObjectChunk.parentName,pNode->parentName.c_str());

		//Model transform
		modelObjectChunk.modelTransform = pNode->modelTransform;

		//Is there geometry associated with this model object? (it could be a dummy node)
		DAEGeometry* pGeom = NULL;
		if (pNode->geometryName.size() != 0)
		{
			pGeom = g_geometryLib.findGeometry(pNode->geometryName);
			if (pGeom)
			{
				if (pGeom->triangleLists.size() == 0)
					FATAL_ERRORF("Geometry object %s has empty triangle list during export",pNode->geometryName)
				else
					modelObjectChunk.numVertexBuffers = (XU32)pGeom->triangleLists.size();
			}
		}
		else
		{
			//This node has no geometry
			modelObjectChunk.numVertexBuffers = 0;
		}

		fwrite(&modelObjectChunk,sizeof(XBSModelObjectChunk),1,outFile);

		//Now we need to output the the vertex buffers associated with this model object,
		//if there was geometry associated with this node.  If not, then output a
		//0 sized bounding sphere
		if (pGeom)
			ExportModelVertexBuffersFromTriangleLists(pNode,NULL,pGeom,outFile);
		else
		{
			XSphere sphere(XVector3D(0,0,0),0);
			fwrite(&sphere.center,sizeof(sphere.center),1,outFile);
			fwrite(&sphere.radius,sizeof(sphere.radius),1,outFile);
		}

		//Finish the model object chunk		
		long endOffset = ftell(outFile);
		h.chunkID = XBS_MODEL_OBJECT;
		h.sizeInBytes = ((beginOffset-endOffset) *- 1) - sizeof(XBSChunkHeader);
		h.version = XBS_VERSION;
		fseek(outFile,beginOffset-endOffset,SEEK_CUR);
		fwrite(&h,sizeof(h),1,outFile);
		fseek(outFile,h.sizeInBytes,SEEK_CUR);
	}
}



//*****************************************************************************
//*****************************************************************************
void ExportModelVertexBuffersFromTriangleLists(DAENode* pNode,DAESkin* pSkin,DAEGeometry* pGeom,FILE* outFile)
{
	XSphere lastSphere;
	for (XU32 i = 0;i < pGeom->triangleLists.size();++i)
	{
		if (pGeom->triangleLists[i].vertexIndices.size() == 0)
			FATAL_ERRORF("Found empty triangle set for geometry %s",pGeom->geometryName.c_str())

		cout << endl << "Triangle Set " << i << endl;

		//First, write out the primitive type
		X_PRIMITIVE_TYPE pt = X_PT_TRIANGLE_LIST;
		if (tristrip)
			pt = X_PT_TRIANGLE_STRIP;
		
		fwrite(&pt,sizeof(pt),1,outFile);

		//Calculate and output vertex description structure
		XVertexDesc desc;
		CreateVertexDescription(&pGeom->triangleLists[i],pSkin,desc);
		cout << "Vertex Description:" << endl;
		cout << "Normal Sets = " << (XU32)desc.numNormals << endl;
		cout << "Color Sets = " << (XU32)desc.numColors << endl;
		cout << "UV Sets = " << (XU32)desc.numUVs << endl;
		cout << "Tangent Sets = " << (XU32)desc.numTangents << endl;
		cout << "Binormal Sets = " << (XU32)desc.numBinormals << endl;
		cout << "Blend Weight Sets = " << (XU32)desc.numBoneWeights4Tuplets << endl;
		cout << "Blend Index Sets = " << (XU32)desc.numBoneIndices4Tuplets << endl;
		fwrite(&desc,sizeof(desc),1,outFile);
		//Then vertex buffer type
		//X_VERTEX_TYPE vt = X_VT_MODEL;
		//fwrite(&vt,sizeof(vt),1,outFile);

		//Then material name
		XU8 s[XBS_MAX_NAME_LEN];
		if (pGeom->triangleLists[i].materialName.size() > XBS_MAX_NAME_LEN-1)
			FATAL_ERROR("Material name too long")
		else
		{
			cout << "Material Name: " << pGeom->triangleLists[i].materialName.c_str() << endl;
			strcpy((char*)s,pGeom->triangleLists[i].materialName.c_str());
		}
		fwrite(s,sizeof(XU8),XBS_MAX_NAME_LEN,outFile);
		
		//Look at the triangle object and de-index the indices
		//into vertex and index buffers.  This will get rid of redundant vertices
		//and compact the vertex and index buffer
		cout << "De-indexing into vertex and index buffers..." << endl;
		packedVertexIndex myVertexIndex((XU32)pGeom->triangleLists[i].inputLists.size());

		XU32 newIndex = 0;							//This is next free index in the output array of deindexed vertices
		XU32 numNonUniqueVerts = 0;					//Number of vertices found that were not unique
        set <packedVertexIndex> uniqueVertices;		//This tracks unique vertex combinations
		vector<XUberVertex> rawVertexBuffer;			//The de-indexed vertex buffer
		vector<XU16> rawIndexBuffer;					//The de-indexed index buffer
		
		//Spin through the primitives 
		//cout << (XU32)pGeom->triangleLists[i].vertexIndices.size() << " " << (XU32)pGeom->triangleLists[i].inputLists.size() << endl;
		XU32 numPrims = (XU32)pGeom->triangleLists[i].vertexIndices.size() / 3;//(XU32)pGeom->triangleLists[i].inputLists.size();
		cout << "Triangle Count: " << numPrims << endl;
		XU32 vertsPerPrim = 3;
		for (XU32 j = 0;j < numPrims;++j)
		{
			for (XU32 numVertex = 0;numVertex < vertsPerPrim;++numVertex)
			{
				//Make a packedVertexIndex object that we will use to determine if this
				//combination of indices is unique;
				for(XU32 k = 0;k < pGeom->triangleLists[i].inputLists.size(); k++)
				{
					switch (pGeom->triangleLists[i].listTypes[k])
					{ 
					case IDT_VERTEX:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].vertexIndices[j*vertsPerPrim+numVertex];
						break;
					case IDT_NORMAL:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].normalIndices[j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR0:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[0][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR1:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[1][j*vertsPerPrim+numVertex];						
						break;
					case IDT_COLOR2:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[2][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR3:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[3][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR4:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[4][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR5:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[5][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR6:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[6][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR7:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[7][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD0:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[0][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD1:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[1][j*vertsPerPrim+numVertex];						
						break;
					case IDT_TEXCOORD2:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[2][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD3:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[3][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD4:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[4][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD5:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[5][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD6:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[6][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD7:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[7][j*vertsPerPrim+numVertex];
						break;
					default:
						FATAL_ERROR("Unrecognized input type while making a packed vertex");
						break;
					}
					
				}
		
				//Try to insert the vertex.  If the vertex already exists, it won't be inserted again.
				//cout << "trying to insert " << myVertexIndex.indices[0] << " " << myVertexIndex.indices[1] << " " << myVertexIndex.indices[2] << " " << myVertexIndex.indices[3] << endl;

				pair<set<packedVertexIndex>::iterator,bool> result = uniqueVertices.insert(myVertexIndex);
				if(result.second)
				{
					//Create a blank vertex with default values
					XUberVertex v;

					//The vertex was unique, so add it to the vertex buffer
					for(XU32 k = 0;k < pGeom->triangleLists[i].inputLists.size(); k++)
					{
						XF32 totalWeight = 0;
						switch (pGeom->triangleLists[i].listTypes[k])
						{
						case IDT_VERTEX:
							v.pos = pGeom->vertexList[myVertexIndex.indices[k]];

							//If we have a skin object.  Take care of bone influences. Currently only 4 bone
							//influences per vertex are supported.  Also, bone influences must be normalized
							//so that they all add up to 1.
							if (pSkin)
							{
								if (pSkin->verts[myVertexIndex.indices[k]].boneWeights.size() > 8)
									FATAL_ERROR("No more than 8 bone influences per vertex are supported");
								if (pSkin->verts[myVertexIndex.indices[k]].boneWeights.size() == 0)
									FATAL_ERROR("Vertex has no bone influences");
								totalWeight = 0;
								for (XU32 l = 0;l < pSkin->verts[myVertexIndex.indices[k]].boneWeights.size();++l)
									totalWeight += pSkin->verts[myVertexIndex.indices[k]].boneWeights[l].weight;
								if (fabs(1.0f - totalWeight) > X_MATH_EPSILON)
								{
									/*for (XU32 l = 0;l < pSkin->verts[myVertexIndex.indices[k]].boneWeights.size();++l)
										cout << endl << pSkin->verts[myVertexIndex.indices[k]].boneWeights[l].weight << " " ;
									cout << endl << "now normalized" << endl;*/
									

									//FATAL_ERROR("Bone influences do not add up to 1");
								}
								//if (fabs(1.0f - totalWeight) > X_MATH_EPSILON)
								//	FATAL_ERROR("Bone influences are greater than 1");

								v.numBones = (XU32)pSkin->verts[myVertexIndex.indices[k]].boneWeights.size();
					
									for (XU32 l = 0;l < pSkin->verts[myVertexIndex.indices[k]].boneWeights.size();++l)
									{
										v.boneIndices[l] = (XF32)pSkin->verts[myVertexIndex.indices[k]].boneWeights[l].boneID;

										//Translate bone index from bone array index to final hierarchy ID
										DAENode* pNode = g_nodeLib.findNodeByBoneName(pSkin->bones[v.boneIndices[l]].name);
										if (pNode)
										{
											//cout << "translating bone index from " << v.boneIndices[l] << " to " << pNode->ID << endl;
											v.boneIndices[l] = pNode->ID;
										}
										else
										{
											cout << "didn't match bone mnames with " << pSkin->bones[v.boneIndices[l]].name << endl;
										}

										v.boneWeights[l] = pSkin->verts[myVertexIndex.indices[k]].boneWeights[l].weight;
									}
							
							}
							break;
						case IDT_NORMAL:
							v.normal = pGeom->normalList[myVertexIndex.indices[k]];
							break;
						case IDT_COLOR0:
							v.diffuse[0] = XColor(pGeom->colorLists[0][myVertexIndex.indices[k]].x,
													pGeom->colorLists[0][myVertexIndex.indices[k]].y,
													pGeom->colorLists[0][myVertexIndex.indices[k]].z,
													pGeom->colorLists[0][myVertexIndex.indices[k]].w).getRGBA32Bit();
							break;
						case IDT_COLOR1:
							v.diffuse[1] = XColor(pGeom->colorLists[1][myVertexIndex.indices[k]].x,
													pGeom->colorLists[1][myVertexIndex.indices[k]].y,
													pGeom->colorLists[1][myVertexIndex.indices[k]].z,
													pGeom->colorLists[1][myVertexIndex.indices[k]].w).getRGBA32Bit();
							break;
						case IDT_COLOR2:
							v.diffuse[2] = XColor(pGeom->colorLists[2][myVertexIndex.indices[k]].x,
													pGeom->colorLists[2][myVertexIndex.indices[k]].y,
													pGeom->colorLists[2][myVertexIndex.indices[k]].z,
													pGeom->colorLists[2][myVertexIndex.indices[k]].w).getRGBA32Bit();
							break;
						case IDT_COLOR3:
							v.diffuse[3] = XColor(pGeom->colorLists[3][myVertexIndex.indices[k]].x,
													pGeom->colorLists[3][myVertexIndex.indices[k]].y,
													pGeom->colorLists[3][myVertexIndex.indices[k]].z,
													pGeom->colorLists[3][myVertexIndex.indices[k]].w).getRGBA32Bit();
							break;
						case IDT_COLOR4:
							v.diffuse[4] = XColor(pGeom->colorLists[4][myVertexIndex.indices[k]].x,
													pGeom->colorLists[4][myVertexIndex.indices[k]].y,
													pGeom->colorLists[4][myVertexIndex.indices[k]].z,
													pGeom->colorLists[4][myVertexIndex.indices[k]].w).getRGBA32Bit();
							break;
						case IDT_COLOR5:
							v.diffuse[5] = XColor(pGeom->colorLists[5][myVertexIndex.indices[k]].x,
													pGeom->colorLists[5][myVertexIndex.indices[k]].y,
													pGeom->colorLists[5][myVertexIndex.indices[k]].z,
													pGeom->colorLists[5][myVertexIndex.indices[k]].w).getRGBA32Bit();
							break;
						case IDT_COLOR6:
							v.diffuse[6] = XColor(pGeom->colorLists[6][myVertexIndex.indices[k]].x,
													pGeom->colorLists[6][myVertexIndex.indices[k]].y,
													pGeom->colorLists[6][myVertexIndex.indices[k]].z,
													pGeom->colorLists[6][myVertexIndex.indices[k]].w).getRGBA32Bit();
							break;
						case IDT_COLOR7:
							v.diffuse[7] = XColor(pGeom->colorLists[7][myVertexIndex.indices[k]].x,
													pGeom->colorLists[7][myVertexIndex.indices[k]].y,
													pGeom->colorLists[7][myVertexIndex.indices[k]].z,
													pGeom->colorLists[7][myVertexIndex.indices[k]].w).getRGBA32Bit();
							break;
						case IDT_TEXCOORD0:
							v.texCoords[0].x = pGeom->texCoordLists[0][myVertexIndex.indices[k]].x;
							v.texCoords[0].y = pGeom->texCoordLists[0][myVertexIndex.indices[k]].y;
							break;
						case IDT_TEXCOORD1:
							v.texCoords[1].x = pGeom->texCoordLists[1][myVertexIndex.indices[k]].x;
							v.texCoords[1].y = pGeom->texCoordLists[1][myVertexIndex.indices[k]].y;
							break;
						case IDT_TEXCOORD2:
							v.texCoords[2].x = pGeom->texCoordLists[2][myVertexIndex.indices[k]].x;
							v.texCoords[2].y = pGeom->texCoordLists[2][myVertexIndex.indices[k]].y;
							break;
						case IDT_TEXCOORD3:
							v.texCoords[3].x = pGeom->texCoordLists[3][myVertexIndex.indices[k]].x;
							v.texCoords[3].y = pGeom->texCoordLists[3][myVertexIndex.indices[k]].y;
							break;
						case IDT_TEXCOORD4:
							v.texCoords[4].x = pGeom->texCoordLists[4][myVertexIndex.indices[k]].x;
							v.texCoords[4].y = pGeom->texCoordLists[4][myVertexIndex.indices[k]].y;
							break;
						case IDT_TEXCOORD5:
							v.texCoords[5].x = pGeom->texCoordLists[5][myVertexIndex.indices[k]].x;
							v.texCoords[5].y = pGeom->texCoordLists[5][myVertexIndex.indices[k]].y;
							break;
						case IDT_TEXCOORD6:
							v.texCoords[6].x = pGeom->texCoordLists[6][myVertexIndex.indices[k]].x;
							v.texCoords[6].y = pGeom->texCoordLists[6][myVertexIndex.indices[k]].y;
							break;
						case IDT_TEXCOORD7:
							v.texCoords[7].x = pGeom->texCoordLists[7][myVertexIndex.indices[k]].x;
							v.texCoords[7].y = pGeom->texCoordLists[7][myVertexIndex.indices[k]].y;
							break;
						default:
							FATAL_ERROR("Unrecognized input type while de-indexing a unique vertex");
							break;
						}
					}

					rawVertexBuffer.push_back(v);
					
					//Now insert a the index into the index buffer
					result.first->references=1;
					result.first->new_index_number = newIndex++;
					rawIndexBuffer.push_back((XU16)result.first->new_index_number);
				}
				else
				{
					//The vertex was not unique, the inputs have already been deindexed so just update
					//the index.
					++numNonUniqueVerts;
					rawIndexBuffer.push_back((XU16)result.first->new_index_number);
					result.first->references++;
					//cout << "not unique ";
					//cout << myVertexIndex.indices[0] << " " << myVertexIndex.indices[1] << " " << myVertexIndex.indices[2] << " " << myVertexIndex.indices[3] << endl;
				}
			}
		}

		cout << "Vertex Reduction from DAE: " << (XF32)((XF32)(numNonUniqueVerts/(XF32)pGeom->triangleLists[i].vertexIndices.size())*100.0f) << "%" << endl;
		cout << "Vertex Count: " << rawVertexBuffer.size() << endl;
		cout << "Index Count: " << rawIndexBuffer.size() << endl;
		
		//Buffers to hold the final vertex and index buffers
		vector<XUberVertex> finalVertexBuffer;
		vector<XU16> nonTriStrippedIndexBuffer;

		if (genTanBi)
		{
			//*********************************************************************************
			//MeshMender to generate tangent and binormals
			cout << "Generating tangents and binormals...";
			
			MeshMender mender;
			vector<MeshMender::Vertex> theVertsForMeshMending;
			vector<XU32>theIndicesForMeshMending;
			vector<XU32>mappingNewToOld;

			//fill up the vectors with our vertex data
			for(XU32 x = 0; x < rawVertexBuffer.size();++x)
			{
				MeshMender::Vertex v;
				v.pos.x = rawVertexBuffer[x].pos.x;
				v.pos.y = rawVertexBuffer[x].pos.y;
				v.pos.z = rawVertexBuffer[x].pos.z;
				v.s = rawVertexBuffer[x].texCoords[0].x;
				v.t = rawVertexBuffer[x].texCoords[0].y;

				//meshmender will computer normals, tangents, and binormals, no need to fill those in.
				//however, if you do not have meshmender compute the normals, you _must_ pass in valid
				//normals to meshmender
				v.normal.x = rawVertexBuffer[x].normal.x;
				v.normal.y = rawVertexBuffer[x].normal.y;
				v.normal.z = rawVertexBuffer[x].normal.z;
				
				theVertsForMeshMending.push_back(v);
			}

			for(XU32 ind= 0;ind < rawIndexBuffer.size();++ind)
			{
				theIndicesForMeshMending.push_back(rawIndexBuffer[ind]);
			}

			//pass it in to the mender to do it's stuff
			mender.Mend( theVertsForMeshMending,theIndicesForMeshMending, mappingNewToOld,
						  0,
						  -1,
						  -1,
						  0,
						  MeshMender::DONT_CALCULATE_NORMALS,
						  MeshMender::DONT_RESPECT_SPLITS,
						  MeshMender::DONT_FIX_CYLINDRICAL);

			//Push empty verts on
			for (XU32 x = 0;x < theVertsForMeshMending.size();++x)
			{
				XUberVertex v;
				finalVertexBuffer.push_back(v);
			}

			bool badGeometry = false;
			for (XU32 x = 0;x < theIndicesForMeshMending.size();++x)
			{
				XUberVertex v;
				v = rawVertexBuffer[mappingNewToOld[theIndicesForMeshMending[x]]];
				
				//v.normal.x = theVertsForMeshMending[theIndicesForMeshMending[x]].normal.x;
				//v.normal.y = theVertsForMeshMending[theIndicesForMeshMending[x]].normal.y;
				//v.normal.z = theVertsForMeshMending[theIndicesForMeshMending[x]].normal.z;
				v.tangent.x = theVertsForMeshMending[theIndicesForMeshMending[x]].tangent.x;
				v.tangent.y = theVertsForMeshMending[theIndicesForMeshMending[x]].tangent.y;
				v.tangent.z = theVertsForMeshMending[theIndicesForMeshMending[x]].tangent.z;
				v.binormal.x = theVertsForMeshMending[theIndicesForMeshMending[x]].binormal.x;
				v.binormal.y = theVertsForMeshMending[theIndicesForMeshMending[x]].binormal.y;
				v.binormal.z = theVertsForMeshMending[theIndicesForMeshMending[x]].binormal.z;

				if (IsInfinityOrNAN(v.tangent) || v.tangent.isZero())
				{
					badGeometry = true;
					v.tangent = XVector3D(1.0f,0.0f,0.0f);
				}
				if (IsInfinityOrNAN(v.binormal) || v.binormal.isZero())
				{
					badGeometry = true;
					v.binormal = XVector3D(0.0f,0.0f,1.0f);
				}

				finalVertexBuffer[theIndicesForMeshMending[x]] = v;
			}
			//if (badGeometry)
			//	cout << "(found some irregular tangents/binormals and attempted to repair them) ";
			for (XU32 x = 0;x < theIndicesForMeshMending.size();++x)
				nonTriStrippedIndexBuffer.push_back(theIndicesForMeshMending[x]);
			//	newVert[index]->myData = oldVert[ mappingNewToOldVert[index]]->myData;
			cout << "success." << endl;
		}
		else
		{
			//straight copy of the raw verts
			for(XU32 x = 0; x < rawVertexBuffer.size();++x)
				finalVertexBuffer.push_back(rawVertexBuffer[x]);

			//straight copy of the raw indices
			for(XU32 x= 0;x < rawIndexBuffer.size();++x)
				nonTriStrippedIndexBuffer.push_back(rawIndexBuffer[x]);
		}
		
		//Reverse winding order (important to do BEFORE tristripping process if any)
		vector<XU16> reversedIndices;
		for (XS32 y = (XS32)nonTriStrippedIndexBuffer.size()-1;y >= 0;--y)
			reversedIndices.push_back(nonTriStrippedIndexBuffer[y]);

		PrimitiveGroup* pPrimGroups = NULL;
		XU16 numGroups = 0;

		if (tristrip)
		{
			//*********************************************************************************
			//Trisripping
			cout << "Generating tristrips...";
			GenerateStrips(&reversedIndices[0],(unsigned int)reversedIndices.size(),&pPrimGroups,&numGroups);
			cout << "success.  " << numGroups << " strip(s) created. " << endl; 
			if (numGroups != 1)
				FATAL_ERROR("Expected exactly 1 tripstrip from tristripping process");
			
			
			//*********************************************************************************
			//Statistics

			//Count degenerates
			XU32 numDegenerates = 0;
			for (XU32 x = 0;x < pPrimGroups->numIndices;++x)
			{
				if (x != pPrimGroups->numIndices-1)
				{
					if (pPrimGroups->indices[x] == pPrimGroups->indices[x+1])
						numDegenerates++;
				}
			}

			cout << "Final Triangle Count: " << pPrimGroups->numIndices-2 << " (with " << numDegenerates << " degenerates)" << endl; 
		}
		else
		{
			cout << "Final Triangle Count: " << reversedIndices.size()/3 << endl; 
		}

		cout << "Final Vertex Count: " << finalVertexBuffer.size() << endl;
		//cout << "Final Index Count: " << pPrimGroups->numIndices << endl;


		//Then num vertices
		XU32 numVerts = (XU32)finalVertexBuffer.size(); 
		if (numVerts >= 65535)
			FATAL_ERROR("A maximum of 65535 verts are supported per vertex buffer")
		else
			fwrite(&numVerts,sizeof(numVerts),1,outFile);
		
		//Then vertex buffer.  Pull verts back into the node's pivot space if we have
		//a pNode pointer, otherwise just copy the verts over
		if (pNode)
		{
			for (XU32 j = 0;j < numVerts;++j)	
			{
				XMatrix4x4 pt = pNode->pivotTransform;
				finalVertexBuffer[j].pos = pt * finalVertexBuffer[j].pos;

				pt._41 = pt._42 = pt._43 = 0.0f;
				finalVertexBuffer[j].normal = pt * finalVertexBuffer[j].normal;
				

				//Occasionally there are some verts with no normal or posiion etc
				//that causes the normalize calls to assert.  Skip the normalize here
				//if (finalVertexBuffer[j].normal.isZero())
				//	continue;

				finalVertexBuffer[j].normal.normalize();
				if (genTanBi)
				{
					finalVertexBuffer[j].tangent = pt * finalVertexBuffer[j].tangent;
					finalVertexBuffer[j].tangent.normalize();
					finalVertexBuffer[j].binormal = pt * finalVertexBuffer[j].binormal;	
					finalVertexBuffer[j].binormal.normalize();
				}
			}
		}
		else
		{
			for (XU32 j = 0;j < numVerts;++j)	
			{
				finalVertexBuffer[j].pos = finalVertexBuffer[j].pos;
				finalVertexBuffer[j].normal = finalVertexBuffer[j].normal;	
				
				//Occasionally there are some verts with no normal or posiion etc
				//that causes the normalize calls to assert.  Skip the normalize here
				if (finalVertexBuffer[j].normal.isZero())
					continue;
				finalVertexBuffer[j].normal.normalize();

				if (genTanBi)
				{
					finalVertexBuffer[j].tangent = finalVertexBuffer[j].tangent;
					finalVertexBuffer[j].tangent.normalize();
					finalVertexBuffer[j].binormal = finalVertexBuffer[j].binormal;	
					finalVertexBuffer[j].binormal.normalize();
				}
			}
		}
		
		//Now we need to output the individual streams that are present in this
		//particular vertex type. 
		
		//Position
		for (XU32 j = 0;j < numVerts;++j)
			fwrite(&finalVertexBuffer[j].pos,sizeof(finalVertexBuffer[j].pos),1,outFile);
		//Normals
		if (desc.numNormals == 1)
			for (XU32 j = 0;j < numVerts;++j)
				fwrite(&finalVertexBuffer[j].normal,sizeof(finalVertexBuffer[j].normal),1,outFile);
		//Colors
		for (XU32 j = 0;j < desc.numColors;++j)
			for (XU32 k = 0;k < numVerts;++k)
				fwrite(&finalVertexBuffer[k].diffuse[j],sizeof(finalVertexBuffer[k].diffuse[j]),1,outFile);
		//UVs
		for (XU32 j = 0;j < desc.numUVs;++j)
			for (XU32 k = 0;k < numVerts;++k)
				fwrite(&finalVertexBuffer[k].texCoords[j],sizeof(finalVertexBuffer[k].texCoords[j]),1,outFile);
		//Tangents
		if (desc.numTangents == 1)
			for (XU32 j = 0;j < numVerts;++j)
				fwrite(&finalVertexBuffer[j].tangent,sizeof(finalVertexBuffer[j].tangent),1,outFile);
		//Binormals
		if (desc.numBinormals == 1)
			for (XU32 j = 0;j < numVerts;++j)
				fwrite(&finalVertexBuffer[j].binormal,sizeof(finalVertexBuffer[j].binormal),1,outFile);
		//Num Bones - if there are blend weights
		if (desc.numBoneWeights4Tuplets == 1)
			for (XU32 j = 0;j < numVerts;++j)
				fwrite(&finalVertexBuffer[j].numBones,sizeof(finalVertexBuffer[j].numBones),1,outFile);
		//Blend Weights
		if (desc.numBoneWeights4Tuplets == 1)
			for (XU32 j = 0;j < numVerts;++j)
				fwrite(&finalVertexBuffer[j].boneWeights,sizeof(finalVertexBuffer[j].boneWeights),1,outFile);
		//Blend Indices
		if (desc.numBoneIndices4Tuplets == 1)
			for (XU32 j = 0;j < numVerts;++j)
				fwrite(&finalVertexBuffer[j].boneIndices,sizeof(finalVertexBuffer[j].boneIndices),1,outFile);	
 
		if (tristrip)
		{
			//Then num indicies
			fwrite(&pPrimGroups->numIndices,sizeof(pPrimGroups->numIndices),1,outFile);

			//Then index buffer
			for (XU32 x = 0;x < pPrimGroups->numIndices;++x)
				fwrite(&pPrimGroups->indices[x],sizeof(pPrimGroups->indices[x]),1,outFile);
		}
		else
		{
			//Then num indicies
			XU32 numIndices = reversedIndices.size();
			fwrite(&numIndices,sizeof(numIndices),1,outFile);

			//Then index buffer
			for (XU32 x = 0;x < numIndices;++x)
				fwrite(&reversedIndices[x],sizeof(reversedIndices[x]),1,outFile);
		}

		//Generate a bounding sphere that for this piece of geometry and merge it with
		//the final sphere that will encompass all geometry for this object
		XSphere sphere;
		XIGenBoundingSphere(sphere,numVerts,(XVector3D*)&finalVertexBuffer[0],sizeof(XUberVertex));
		if (i > 0)
			XISphereEnclosingSpheres(lastSphere,lastSphere,sphere);
		else
			lastSphere = sphere;
		//XIGenBoundingAABB(newMot->m_aabb,numVerts,pVB->getPositionStream(),sizeof(XVector3D));

	
		//Clean up
		rawVertexBuffer.clear();
		rawIndexBuffer.clear();
		reversedIndices.clear();
		finalVertexBuffer.clear();
		nonTriStrippedIndexBuffer.clear();

		if (tristrip)
			delete[] pPrimGroups;
	}

	//Now spit out the final bounding sphere center and radius
	fwrite(&lastSphere.center,sizeof(lastSphere.center),1,outFile);
	fwrite(&lastSphere.radius,sizeof(lastSphere.radius),1,outFile);
	cout << endl << "Bounding Sphere: c = " << lastSphere.center.x << " " << lastSphere.center.y << " " << lastSphere.center.z << " r = " << lastSphere.radius << endl; 

}



//*****************************************************************************
//*****************************************************************************
void ExportSkinMeshVertexBuffersFromTriangleLists(DAESkin* pSkin,DAEGeometry* pGeom,FILE* outFile)
{
	for (XU32 i = 0;i < pGeom->triangleLists.size();++i)
	{
		//First, write out the primitive type
		X_PRIMITIVE_TYPE pt = X_PT_TRIANGLE_LIST;
		if (tristrip)
			pt = X_PT_TRIANGLE_STRIP;		
		fwrite(&pt,sizeof(pt),1,outFile);

		//Then vertex buffer type
		X_VERTEX_TYPE vt = X_VT_SKIN;
		fwrite(&vt,sizeof(vt),1,outFile);

		//Then material name
		XU8 s[XBS_MAX_NAME_LEN];
		if (pGeom->triangleLists[i].materialName.size() > XBS_MAX_NAME_LEN-1)
			FATAL_ERROR("Material name too long")
		else
			strcpy((char*)s,pGeom->triangleLists[i].materialName.c_str());
		fwrite(s,sizeof(XU8),XBS_MAX_NAME_LEN,outFile);
		
		//Look at the DAE geometry object passed in and de-index the indices
		//into vertex and index buffers.  This will get rid of redundant vertices
		//and compact the vertex and index buffer
		cout << "De-indexing into vertex and index buffers..." << endl;
		packedVertexIndex myVertexIndex((XU32)pGeom->triangleLists[i].inputLists.size());
		XU32 newIndex = 0;							//This is next free index in the output array of deindexed vertices
		XU32 numNonUniqueVerts = 0;					//Number of vertices found that were not unique
        set <packedVertexIndex> uniqueVertices;		//This tracks unique vertex combinations
		vector<XSkinVertex> vertexBuffer;			//The de-indexed vertex buffer
		vector<XU16> indexBuffer;					//The de-indexed index buffer
		XU32 numOrgVertex = 0;						//Vertex num from original verts

		//Spin through the primitives
		XU32 numPrims = (XU32)pGeom->triangleLists[i].vertexIndices.size() / (XU32)pGeom->triangleLists[i].inputLists.size();
		XU32 vertsPerPrim = 3;
		for (XU32 j = 0;j < numPrims;++j)
		{
			for (XU32 numVertex = 0;numVertex < vertsPerPrim;++numVertex)
			{
				//Make a packedVertexIndex object that we will use to determine if this
				//combination of indices is unique;
				for(XU32 k = 0;k < pGeom->triangleLists[i].inputLists.size(); k++)
				{
					switch (pGeom->triangleLists[i].listTypes[k])
					{
					case IDT_VERTEX:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].vertexIndices[j*vertsPerPrim+numVertex];
						break;
					case IDT_NORMAL:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].normalIndices[j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR0:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[0][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR1:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[1][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR2:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[2][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR3:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[3][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR4:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[4][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR5:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[5][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR6:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[6][j*vertsPerPrim+numVertex];
						break;
					case IDT_COLOR7:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].colorIndices[7][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD0:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[0][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD1:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[1][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD2:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[2][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD3:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[3][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD4:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[4][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD5:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[5][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD6:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[6][j*vertsPerPrim+numVertex];
						break;
					case IDT_TEXCOORD7:
						myVertexIndex.indices[k] = pGeom->triangleLists[i].texCoordIndices[7][j*vertsPerPrim+numVertex];
						break;
					default:
						FATAL_ERROR("Unrecognized input type while de-indexing during export for skin");
						break;
					}
					
				}
		
				//Try to insert the vertex.  If the vertex already exists, it won't be inserted again.
				pair<set<packedVertexIndex>::iterator,bool> result = uniqueVertices.insert(myVertexIndex);
				if(result.second)
				{
					//Create a blank vertex with default values
					XSkinVertex v;
					v.diffuse = 0xFFFFFFFF;

					//The vertex was unique, so add it to the vertex buffer
					for(XU32 k = 0;k < pGeom->triangleLists[i].inputLists.size(); k++)
					{
						XF32 totalWeight = 0;
						switch (pGeom->triangleLists[i].listTypes[k])
						{
						case IDT_VERTEX:
							v.pos = pGeom->vertexList[myVertexIndex.indices[k]];
							
							//Take care of bone influences. Currently only 4 bone influences per vertex are
							//supported.  Also, bone influences must be normalized so that they all add up
							//to 1.
							if (pSkin->verts[myVertexIndex.indices[k]].boneWeights.size() > 8)
								FATAL_ERROR("No more than 8 bone influences per vertex are supported");
							if (pSkin->verts[myVertexIndex.indices[k]].boneWeights.size() == 0)
								FATAL_ERROR("Vertex has no bone influences");
							totalWeight = 0;
							for (XU32 l = 0;l < pSkin->verts[myVertexIndex.indices[k]].boneWeights.size();++l)
								totalWeight += pSkin->verts[myVertexIndex.indices[k]].boneWeights[l].weight;
							if (fabs(1.0f - totalWeight) > X_MATH_EPSILON)
							{
								/*for (XU32 l = 0;l < pSkin->verts[myVertexIndex.indices[k]].boneWeights.size();++l)
									cout << endl << pSkin->verts[myVertexIndex.indices[k]].boneWeights[l].weight << " " ;
								cout << endl << "now normalized" << endl;*/
								

								FATAL_ERROR("Bone influences do not add up to 1");
							}
							if (fabs(1.0f - totalWeight) > X_MATH_EPSILON)
								FATAL_ERROR("Bone influences are greater than 1");

							v.numBones = (XU32)pSkin->verts[myVertexIndex.indices[k]].boneWeights.size();
							for (XU32 l = 0;l < pSkin->verts[myVertexIndex.indices[k]].boneWeights.size();++l)
							{
								v.boneIndices[l] = (XF32)pSkin->verts[myVertexIndex.indices[k]].boneWeights[l].boneID;
								v.boneWeights[l] = pSkin->verts[myVertexIndex.indices[k]].boneWeights[l].weight;
							}

							break;
						case IDT_NORMAL:
							v.normal = pGeom->normalList[myVertexIndex.indices[k]];
							break;
						case IDT_TEXCOORD0:
							v.texCoords.x = pGeom->texCoordLists[0][myVertexIndex.indices[k]].x;
							v.texCoords.y = pGeom->texCoordLists[0][myVertexIndex.indices[k]].y;
							break;
						default:
							FATAL_ERROR("Unrecognized input type while de-indexing a unique vertex");
							break;
						}
					}			

					vertexBuffer.push_back(v);

					//Now insert a the index into the index buffer
					result.first->references=1;
					result.first->new_index_number = newIndex++;
					indexBuffer.push_back((XU16)result.first->new_index_number);
				}
				else
				{
					//The vertex was not unique, the inputs have already been deindexed so just update
					//the index.
					++numNonUniqueVerts;
					indexBuffer.push_back((XU16)result.first->new_index_number);
					result.first->references++;
				}

				++numOrgVertex;
			}
		}

		cout << "Vertex Reduction: " << (XF32)((XF32)(numNonUniqueVerts/(XF32)pGeom->triangleLists[i].vertexIndices.size())*100.0f) << "%" << endl;
		cout << "Vertex Count: " << vertexBuffer.size() << endl;
		cout << "Index Count: " << indexBuffer.size() << endl; 

		//Then num vertices
		XU32 numVerts = (XU32)vertexBuffer.size();
		if (numVerts >= 65535)
			FATAL_ERROR("A maximum of 65535 verts are supported per vertex buffer")
		else
			fwrite(&numVerts,sizeof(numVerts),1,outFile);
		
		//Then vertex buffer.  
		for (XU32 j = 0;j < numVerts;++j)	
		{
			//XMatrix4x4 pt = pNode->pivotTransform;
			//vertexBuffer[j].pos = pt * vertexBuffer[j].pos;

			//pt._41 = pt._42 = pt._43 = 0.0f;
			//vertexBuffer[j].normal = pt * vertexBuffer[j].normal;
			
			fwrite(&vertexBuffer[j],sizeof(vertexBuffer[j]),1,outFile);
		}
		
		//Then num indicies
		XU32 numIndicies = (XU32)indexBuffer.size();
		fwrite(&numIndicies,sizeof(numIndicies),1,outFile);

		//Then index buffer in reversed order to get CCW winding
		for (XS16 j = numIndicies-1;j >= 0;--j)
			fwrite(&indexBuffer[j],sizeof(indexBuffer[j]),1,outFile);

		//Clean up
		vertexBuffer.clear();
		indexBuffer.clear();
		uniqueVertices.clear();
	}

}



//*****************************************************************************
//*****************************************************************************
void ExportSkinMeshObjects(FILE* outFile)
{
	//First, do we even have any skin meshes to export?
	if (g_skinLib.skinList.size() == 0)
		return;
	
	for (XU32 i = 0;i < g_skinLib.skinList.size();++i) 
	{
		DAESkin* pSkin = &g_skinLib.skinList[0];

		//Stream out the header and chunk
		long beginOffset = ftell(outFile);
		XBSChunkHeader h;
		fwrite(&h,sizeof(h),1,outFile);

		XBSSkinChunk skinChunk;
			
		//Name
		if (pSkin->name.size() > XBS_MAX_NAME_LEN-1)
			FATAL_ERROR("Node name too long")
		else
			strcpy((char*)skinChunk.name,pSkin->name.c_str());
			
		//Parent name
		if (pSkin->parentName.size() > XBS_MAX_NAME_LEN-1)
			FATAL_ERROR("Parent node name too long")
		else
			strcpy((char*)skinChunk.parentName,pSkin->parentName.c_str());

		//Model transform we'll leave as the identity since we only support one skin object
		//modelObjectChunk.modelTransform = pSkin->modelTransform;
		
		//Is there geometry associated with this skin mesh?  There should be.
		DAEGeometry* pGeom = NULL;
		if (pSkin->geometryName.size() != 0)
		{
			pGeom = g_geometryLib.findGeometry(pSkin->geometryName);
			if (pGeom)
			{
				if (pGeom->triangleLists.size() == 0)
					FATAL_ERRORF("Geometry object %s has empty triangle list during export",pSkin->geometryName)
				else
					skinChunk.numVertexBuffers = (XU32)pGeom->triangleLists.size();
			}
		}
		else
		{
			FATAL_ERROR("No geometry object found for skin");
			//This node has no geometry
			//skinChunk.numVertexBuffers = 0;
		}

		fwrite(&skinChunk,sizeof(XBSSkinChunk),1,outFile);

		//Now we need to output the the vertex buffers associated with this skin object
		ExportModelVertexBuffersFromTriangleLists(NULL,pSkin,pGeom,outFile);

		//Finish the skin mesh model object chunk		
		long endOffset = ftell(outFile);
		h.chunkID = XBS_SKIN_MESH;
		h.sizeInBytes = ((beginOffset-endOffset) *- 1) - sizeof(XBSChunkHeader);
		h.version = XBS_VERSION;
		fseek(outFile,beginOffset-endOffset,SEEK_CUR);
		fwrite(&h,sizeof(h),1,outFile);
		fseek(outFile,h.sizeInBytes,SEEK_CUR);
	}

	//Export the entire node library as bones
	/*for (XU32 i = 0;i < g_nodeLib.nodeList.size();++i) 
	{
		DAENode* pNode = &g_nodeLib.nodeList[i];

		//Stream out the header and chunk
		long beginOffset = ftell(outFile);
		XBSChunkHeader h;
		fwrite(&h,sizeof(h),1,outFile);

		XBSBoneChunk boneChunk;
		
		//Name
		if (pNode->name.size() > XBS_MAX_NAME_LEN-1)
			FATAL_ERROR("Node name too long");
		else
			strcpy((char*)boneChunk.name,pNode->name.c_str());
		
		//Parent name
		if (pNode->parentName.size() > XBS_MAX_NAME_LEN-1)
			FATAL_ERROR("Parent node name too long");
		else
			strcpy((char*)boneChunk.parentName,pNode->parentName.c_str());

		//Model transform
		boneChunk.modelTransform = pNode->modelTransform;

		//Inverse Bone Transform
		DAEBone* pBone = g_skinLib.skinList[0].findBone(pNode->boneName);
		if (!pBone)
		{
			char buf[512];
			sprintf(buf,"Could not locate inv bone transform for node %s in skin lib",pNode->name.c_str());
			FATAL_ERROR(buf);
		}
		boneChunk.invBoneTransform = pBone->invBoneTransform;

		fwrite(&boneChunk,sizeof(boneChunk),1,outFile);

		//Finish the bone chunk		
		long endOffset = ftell(outFile);
		h.chunkID = XBS_BONE;
		h.sizeInBytes = ((beginOffset-endOffset) *- 1) - sizeof(XBSChunkHeader);
		h.version = XBS_VERSION;
		fseek(outFile,beginOffset-endOffset,SEEK_CUR);
		fwrite(&h,sizeof(h),1,outFile);
		fseek(outFile,h.sizeInBytes,SEEK_CUR);
	}*/
}


//*****************************************************************************
//*****************************************************************************
void ExportAnimTrack(FILE* outFile)
{

	//Order of parsing is important
	DAEParseMaterialLib();
	DAEParseGeometryLib(g_geometryLib);
	DAEParseModelVisualSceneLib();
	DAEParseAnimationLib(g_animationLib);

	XU32 numKeyFrames = 0;
	XU32 time = 0;

	//Let's export the animation track
	for (XU32 i = 0;i < g_animationLib.animationList.size();++i)
	{
		//Stream out the header and chunk
		long beginOffset = ftell(outFile);
		XBSChunkHeader h;
		fwrite(&h,sizeof(h),1,outFile);
		
		cout << "@KEY_FRAME_SET\n";

		fwrite(&g_animationLib.animationList[i].ID,sizeof(g_animationLib.animationList[i].ID),1,outFile);
		cout << "\tID " << g_animationLib.animationList[i].ID << endl;

		cout << "\tNAME \"" << g_animationLib.animationList[i].name << "\"" << endl;

		numKeyFrames = (XU32)g_animationLib.animationList[i].rotKeyList.size();
		fwrite(&numKeyFrames,sizeof(numKeyFrames),1,outFile);
		cout << "\tNUM_ROT_KF " << g_animationLib.animationList[i].rotKeyList.size() << endl;
		
		for (XU32 j = 0;j < g_animationLib.animationList[i].rotKeyList.size();++j)
		{
			time = (XU32)(g_animationLib.animationList[i].timeList[j] * 1000);
			fwrite(&time,sizeof(time),1,outFile);
			fwrite(&g_animationLib.animationList[i].rotKeyList[j],sizeof(g_animationLib.animationList[i].rotKeyList[j]),1,outFile);

			cout << "\t" << j << " " <<  (int)(g_animationLib.animationList[i].timeList[j]*1000) << " " <<
								   g_animationLib.animationList[i].rotKeyList[j].x << " " <<
									g_animationLib.animationList[i].rotKeyList[j].y << " " <<
									g_animationLib.animationList[i].rotKeyList[j].z << " " <<
									g_animationLib.animationList[i].rotKeyList[j].w << endl;
		}

		numKeyFrames = (XU32)g_animationLib.animationList[i].posKeyList.size();
		fwrite(&numKeyFrames,sizeof(numKeyFrames),1,outFile);
		cout << "\tNUM_POS_KF " << g_animationLib.animationList[i].posKeyList.size() << endl;
		
		for (XU32 j = 0;j < g_animationLib.animationList[i].posKeyList.size();++j)
		{
			time = (XU32)(g_animationLib.animationList[i].timeList[j] * 1000);
			fwrite(&time,sizeof(time),1,outFile);
			fwrite(&g_animationLib.animationList[i].posKeyList[j],sizeof(g_animationLib.animationList[i].posKeyList[j]),1,outFile);

			cout << "\t" << j << " " << (int)(g_animationLib.animationList[i].timeList[j]*1000) << " " <<
			g_animationLib.animationList[i].posKeyList[j].x << " " <<
									g_animationLib.animationList[i].posKeyList[j].y << " " <<
									g_animationLib.animationList[i].posKeyList[j].z << endl;
		}

		numKeyFrames = (XU32)g_animationLib.animationList[i].scaleKeyList.size();
		fwrite(&numKeyFrames,sizeof(numKeyFrames),1,outFile);

		cout << "\tNUM_SCA_KF " << g_animationLib.animationList[i].scaleKeyList.size() << endl;
		for (XU32 j = 0;j < g_animationLib.animationList[i].scaleKeyList.size();++j)
		{

			time = (XU32)(g_animationLib.animationList[i].timeList[j] * 1000);
			fwrite(&time,sizeof(time),1,outFile);
			fwrite(&g_animationLib.animationList[i].scaleKeyList[j],sizeof(g_animationLib.animationList[i].scaleKeyList[j]),1,outFile);

			cout << "\t" << j << " " <<  (int)(g_animationLib.animationList[i].timeList[j]*1000) << " " <<
			g_animationLib.animationList[i].scaleKeyList[j].x << " " <<
									g_animationLib.animationList[i].scaleKeyList[j].y << " " <<
									g_animationLib.animationList[i].scaleKeyList[j].z << endl;
		}

		//f.close();

		//Finish the animation track chunk		
		long endOffset = ftell(outFile);
		h.chunkID = XBS_ANIM_TRACK;
		h.sizeInBytes = ((beginOffset-endOffset) * -1) - sizeof(XBSChunkHeader);
		h.version = XBS_VERSION;
		fseek(outFile,beginOffset-endOffset,SEEK_CUR);
		fwrite(&h,sizeof(h),1,outFile);
		fseek(outFile,h.sizeInBytes,SEEK_CUR);
	}
}

/*
void RecurseNodes(domNode_Array& nodeArray)
{
	for (XU32 i = 0;i < nodeArray.getCount();++i)
	{
		f << "@GEOMETRY\n";
		//Get the ID of the node first
		if (!nodeArray[i]->getID())
			FATAL_ERROR("Node does not have ID");

		cout << nodeArray[i]->getName();
		f << "\tNAME \"" << nodeArray[i]->getName() << "\"\n";

		//Find parent if any
		//XMatrix4x4 parentMat;
		if (nodeArray[i]->getXMLParentElement()->getMeta() == domNode::_Meta)
		{
			//Does the parent have a matrix?  If so, get a copy of it.
			//domNode* pParentNode = (domNode*)nodeArray[i]->getXMLParentElement();
			//if (pParentNode->getMatrix_array().getCount() != 0)
			//{
			//	domListOfFloats& floatList = pParentNode->getMatrix_array()[0]->getValue();	
			//	DAEMatrixConvert(floatList,parentMat);
			//}

			cout << " P-> " << nodeArray[i]->getXMLParentElement()->getID() << " (";
			f << "\tPARENT_NAME \"" << nodeArray[i]->getXMLParentElement()->getID() << "\"\n";
		}
		else 
		{
			cout << " P-> NULL (";
			f << "\tPARENT_NAME \"NULL\"\n";
		}


		//Do we have a matrix for this node?
		if (nodeArray[i]->getMatrix_array().getCount() != 0)
		{
			domListOfFloats& floatList = nodeArray[i]->getMatrix_array()[0]->getValue();	
			for (XU32 j = 0;j < 16;++j)
				cout << floatList[j] << " ";
			cout << ")" << endl;

			//Convert the matrix
			XMatrix4x4 mat;
			DAEMatrixConvert(&floatList[0],mat);
			f << "\tMX_R1 " << mat._11 << " " << mat._12 << " " << mat._13 << endl;
			f << "\tMX_R2 " << mat._21 << " " << mat._22 << " " << mat._23 << endl;
			f << "\tMX_R3 " << mat._31 << " " << mat._32 << " " << mat._33 << endl;
			f << "\tMX_R4 " << mat._41 << " " << mat._42 << " " << mat._43 << endl;

		}
		
		//Get instance geometry and find the geometry in the cached geometry lib
		if (nodeArray[i]->getInstance_geometry_array().getCount() == 0)
			FATAL_ERROR("Couldn't find geometry instance for node");
		nodeArray[i]->getInstance_geometry_array()[0]->getUrl().resolveElement();
		daeElement* pElement = nodeArray[i]->getInstance_geometry_array()[0]->getUrl().getElement();
		domGeometry* pDomGeometry = (domGeometry*)pElement;
		DAEGeometry* pGeom = g_geometryLib.findGeometry(pDomGeometry->getName());
		if (!pGeom)
			FATAL_ERROR("Couldn't find cached geometry for node");

		f << "\tMATERIAL_ID 0" << endl;

		f << "\tNUM_VERTS " << pGeom->vertexList.size() << endl;
		
		for (XU32 k = 0;k < pGeom->vertexList.size();++k)
			f << "\t" << k << " " << pGeom->vertexList[k].x << " " << pGeom->vertexList[k].y << " " << pGeom->vertexList[k].z << endl; 

		f << "\tNUM_TVERTS 0" << endl;

		f << "\tNUM_MFACES " << pGeom->triangleLists[0].vertexIndices.size()/3 << endl;
	 
		for (XU32 k = 0;k < pGeom->triangleLists[0].vertexIndices.size()/3;k++)
			f << "\t" << k << " " << pGeom->triangleLists[0].vertexIndices[k*3]<< " " << pGeom->triangleLists[0].vertexIndices[k*3+1]<<" " << pGeom->triangleLists[0].vertexIndices[k*3+2]<<endl; 

		f << "\tNUM_TFACES 0" << endl;

		f << "@\n";

		//Now recurse to next level if it exists
		if (nodeArray[i]->getNode_array().getCount() != 0)
			recurseNodes(nodeArray[i]->getNode_array(),f);	
	}
}
*/



