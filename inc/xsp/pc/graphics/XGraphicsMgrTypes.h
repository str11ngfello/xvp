
#ifndef __X_GRAPHICS_MGR_TYPES_H__
#define __X_GRAPHICS_MGR_TYPES_H__


#include <XOSMgrTypes.h>
#include <XColor.h>

class XScene;
class XSceneNode;

/** @defgroup XGraphicsMgrTypes XGraphicsMgrTypes
These are the globally defined identifiers for graphics types.
@{
*/

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define X_MAX_VERT_DCL_NAME_LEN		64
#define X_MAX_SAMPLERS				16
#define X_MAX_RENDER_TARGETS		8
#define X_MAX_VIEWPORTS				8

#define X_SCENE_SLOT				0
#define X_USER_SLOT0				1
#define X_USER_SLOT1				2
#define X_USER_SLOT2				3
#define X_USER_SLOT3				4
#define X_USER_SLOT4				5
#define X_USER_SLOT5				6
#define X_USER_SLOT6				7
#define X_USER_SLOT7				8
#define X_USER_SLOT8				9
#define X_USER_SLOT9				10
#define X_USER_SLOT10				11
#define X_USER_SLOT11				12
#define X_USER_SLOT12				13
#define X_USER_SLOT13				14
#define X_USER_SLOT14				15


//*****************************************************************************
//! Buffer lock flags.
/**
These flags specify how to lock a memory buffer or how a memory buffer will be
used once it is locked.
*/
//*****************************************************************************
enum X_LOCK_TYPE
{
	X_LT_NONE,			/*!< No special locking type. */
	X_LT_READ_ONLY,		/*!< Locks buffer in read only mode. */
	X_LT_WRITE_ONLY,	/*!< Locks buffer in write only mode. */
	X_LT_READ_WRITE,	/*!< Locks buffer in read/write mode. */
	X_LT_DISCARD,		/*!< Throws away current buffer contents. */
	X_LT_NO_OVERWRITE	/*!< Locks buffer in no overwrite mode. */
};

enum X_SEMANTIC_TYPE
{
	X_ST_POSITION,
	X_ST_COLOR,
	X_ST_NORMAL,
	X_ST_UV,
	X_ST_TANGENT,
	X_ST_BINORMAL
};

//*****************************************************************************
//*****************************************************************************
enum X_PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};



//*****************************************************************************
//! Texture addressing modes.
/**
These defines specify various types of texture addressing modes.
*/
//*****************************************************************************
enum X_TEXTURE_ADDRESS_TYPE 
{
	X_TAT_UNKNOWN,		/*!< Unknown/Invalid texture address type. */
    X_TAT_WRAP,			/*!< Tile the texture at every integer junction. For example, for u values between 0 and 3, the texture is repeated three times; no mirroring is performed. */
    X_TAT_MIRROR,		/*!< Similar to X_TAT_WRAP, except that the texture is flipped at every integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again, and so on.. */
    X_TAT_CLAMP,		/*!< Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively. */
    X_TAT_BORDER,		/*!< Texture coordinates outside the range [0.0, 1.0] are set to the border color. */
    X_TAT_MIRROR_ONCE	/*!< Similar to X_TAT_MIRROR and X_TAT_CLAMP. Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value. The most common usage is for volume textures, where support for the full X_TAT_MIRROR_ONCE texture-addressing mode is not necessary, but the data is symmetric around the one axis. */
};



//*****************************************************************************
//! Texture filtering types.
/**
These defines specify various types of texture filtering.
*/
//*****************************************************************************
enum X_TEXTURE_FILTER_TYPE
{
	X_TFT_UNKNOWN,			
	X_TFT_MIN_MAG_MIP_POINT,
	X_TFT_MIN_MAG_POINT_MIP_LINEAR,
	X_TFT_MIN_POINT_MAG_LINEAR_MIP_POINT,
	X_TFT_MIN_POINT_MAG_MIP_LINEAR,
	X_TFT_MIN_LINEAR_MAG_MIP_POINT,
	X_TFT_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	X_TFT_MIN_MAG_LINEAR_MIP_POINT,
	X_TFT_MIN_MAG_MIP_LINEAR,
	X_TFT_ANISOTROPIC,
	X_TFT_COMPARISON_MIN_MAG_MIP_POINT,
	X_TFT_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
	X_TFT_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
	X_TFT_COMPARISON_MIN_POINT_MAG_MIP_LINEAR0x85,
	X_TFT_COMPARISON_MIN_LINEAR_MAG_MIP_POINT0x90,
	X_TFT_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	X_TFT_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
	X_TFT_COMPARISON_MIN_MAG_MIP_LINEAR,
	X_TFT_COMPARISON_ANISOTROPIC,
	X_TFT_TEXT_1BIT
};



//*****************************************************************************
//! Primitive types.
/**
These defines specify various geometry primitive types.
*/
//*****************************************************************************
enum X_PRIMITIVE_TYPE 
{
	X_PT_UNKNOWN,			/*!< Unknown/Invalid primitive type. */
	X_PT_POINT_LIST,		/*!< Renders the vertices as a collection of isolated points. This value is unsupported for indexed primitives. */
	X_PT_LINE_LIST,			/*!< Renders the vertices as a list of isolated straight line segments. Calls using this primitive type fail if the count is less than two or is odd. */
	X_PT_LINE_STRIP,		/*!< Renders the vertices as a single polyline. Calls using this primitive type fail if the count is less than two. */
	X_PT_TRIANGLE_LIST,		/*!< Renders the specified vertices as a sequence of isolated triangles. Each group of three vertices defines a separate triangle. */
	X_PT_TRIANGLE_STRIP,	/*!< Renders the vertices as a triangle strip. */
	X_PT_LINE_LIST_ADJ,
	X_PT_LINE_STRIP_ADJ,
	X_PT_TRIANGLE_LIST_ADJ,
	X_PT_TRIANGLE_STRIP_ADJ 
};



//*****************************************************************************
//! Fill mode types.
/**
These defines specify various fill mode types.
*/
//*****************************************************************************
enum X_FILL_MODE_TYPE 
{
	X_FMT_UNKNOWN,	/*!< Unknown/Invalid fill mode type. */
	X_FMT_WIRE,		/*!< Geometry is rendered as wire frames. */
	X_FMT_SOLID		/*!< Geometry is rendered as solid polygons. */
};



//*****************************************************************************
//! Blend types.
/**
These defines specify various blending types.  In the descriptions, the RGBA
values of the source and destination are indicated by the lowercase 's; and 'd'.
*/
//*****************************************************************************
enum X_BLEND_TYPE
{
	X_BT_UNKNOWN,				/*!< Unknown/Invalid blend type. */	
    X_BT_ZERO,					/*!< Blend factor is (0, 0, 0, 0). */
    X_BT_ONE,					/*!< Blend factor is (1, 1, 1, 1). */
    X_BT_SRC_COLOR,				/*!< Blend factor is (Rs,Gs,Bs,As). */
    X_BT_INV_SRC_COLOR,			/*!< Blend factor is (1 - Rs, 1 - Gs, 1 - Bs, 1 - As. */
    X_BT_SRC_ALPHA,				/*!< Blend factor is (As, As, As, As). */
    X_BT_INV_SRC_ALPHA,			/*!< Blend factor is ( 1 - As, 1 - As, 1 - As, 1 - As). */
    X_BT_DEST_ALPHA,			/*!< Blend factor is (Ad, Ad, Ad, Ad). */
    X_BT_INV_DEST_ALPHA,		/*!< Blend factor is (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad). */		
    X_BT_DEST_COLOR,			/*!< Blend factor is (Rd, Gd, Bd, Ad).  */
    X_BT_INV_DEST_COLOR,		/*!< Blend factor is (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad). */
    X_BT_SRC_ALPHA_SAT,			/*!< Blend factor is (f, f, f, 1); f = min(A, 1 - Ad). */
    X_BT_BLEND_FACTOR,
	X_BT_INV_BLEND_FACTOR,
	X_BT_SRC1_COLOR,
	X_BT_INV_SRC1_COLOR,
	X_BT_SRC1_ALPHA,
	X_BT_INV_SRC1_ALPHA		
};



//*****************************************************************************
//! Cull modes.
/**
These defines specify various culling modes.  
*/
//*****************************************************************************
enum X_CULL_TYPE
{
	X_CULLT_UNKNOWN,	/*!< Unknown/Invalid cull mode. */
    X_CULLT_NONE,		/*!< Specifies no culling will take place. */
    X_CULLT_FRONT,		/*!< Specifies culling will occur for front faces. */
    X_CULLT_BACK		/*!< Specifies culling will occur for back faces. */
};



//*****************************************************************************
//! Compare functions.
/**
These defines specify various compare functions.  
*/
//*****************************************************************************
enum X_CMP_FUNC_TYPE
{
	X_CFT_UNKNOWN,			/*!< Unknown/Invalid compare function. */
    X_CFT_NEVER,			/*!< Always fail the test. */
    X_CFT_LESS,				/*!< Accept the new pixel if its value is less than the value of the current pixel. */
    X_CFT_EQUAL,			/*!< Accept the new pixel if its value equals the value of the current pixel. */
    X_CFT_LESS_EQUAL,		/*!< Accept the new pixel if its value is less than or equal to the value of the current pixel. */
    X_CFT_GREATER,			/*!< Accept the new pixel if its value is greater than the value of the current pixel. */
    X_CFT_NOT_EQUAL,		/*!< Accept the new pixel if its value does not equal the value of the current pixel. */
    X_CFT_GREATER_EQUAL,	/*!< Accept the new pixel if its value is greater than or equal to the value of the current pixel. */
    X_CFT_ALWAYS			/*!< Always pass the test. */
};



//*****************************************************************************
//! Stencil operation types.
/**
These defines specify various stencil operation types.  
*/
//*****************************************************************************
enum X_STENCIL_OP_TYPE
{
	X_SOT_UNKNOWN,		/*!< Unknown/Invalid stencil operation. */
    X_SOT_KEEP,			/*!< Do not update the entry in the stencil buffer. */
    X_SOT_ZERO,			/*!< Set the stencil-buffer entry to 0. */
    X_SOT_REPLACE,		/*!< Replace the stencil-buffer entry with reference value. */
    X_SOT_INCRSAT,		/*!< Increment the stencil-buffer entry, clamping to the maximum value. */
    X_SOT_DECRSAT,		/*!< Decrement the stencil-buffer entry, clamping to zero. */
    X_SOT_INVERT,		/*!< Invert the bits in the stencil-buffer entry. */
    X_SOT_INCR,			/*!< Increment the stencil-buffer entry, wrapping to zero if the new value exceeds the maximum value. */
    X_SOT_DECR			/*!< Decrement the stencil-buffer entry, wrapping to the maximum value if the new value is less than zero. */
};



//*****************************************************************************
//! Blend operation types.
/**
Defines various blend operation types.
*/
//*****************************************************************************
enum X_BLEND_OP_TYPE 
{
	X_BOT_UNKNOWN,			/*!< Unknown/Invalid blend operation type. */
    X_BOT_ADD,				/*!< The result is the destination added to the source. */
    X_BOT_SUBTRACT,			/*!< The result is the destination subtracted from to the source. */
    X_BOT_REVSUBTRACT,		/*!< The result is the source subtracted from the destination. */
    X_BOT_MIN,				/*!< The result is the minimum of the source and destination. */
    X_BOT_MAX				/*!< The result is the maximum of the source and destination. */
};



//*****************************************************************************
//! Light types.
/**
Defines various types of lights.
*/
//*****************************************************************************
enum X_LIGHT_TYPE 
{
    X_LT_POINT,			/*!< Point light. Radiates in all directions from a single point. */
    X_LT_SPOT,			/*!< Spot light. */
    X_LT_DIRECTIONAL	/*!< Directional light.  Infinite from all positions but only in a single direction. */
};



//****************************************************************************
//****************************************************************************
struct XRenderEvent
{
	XScene* pScene;
	XSceneNode* pSceneNode;
	void* pEventData;
	XHandle materialHandle;
	XU32 renderLevel;
};



//*****************************************************************************
//! Vertex types.
/**
These defines specify various types of vertices and the information they
contain.  
*/
//*****************************************************************************
enum X_VERTEX_TYPE 
{
	X_VT_UNKNOWN,	/*!< Unknown/Invalid vertex type */
	X_VT_RHW,		/*!< x,y,z, rhw position, 1 diffuse, 1 pair of texure coordinates */
	X_VT_PLAIN,		/*!< x,y,z position */
	X_VT_MODEL,		/*!< x,y,z position, 1 normal vector, 1 diffuse color, 1 pair of texture coordinates */
	X_VT_SKIN,		/*!< x,y,z position, 4 blend weights, 4 blend indices, 1 diffuse color, 1 pair texture of coordinates */
	X_VT_PARTICLE,	/*!< x,y,z position, 1 diffuse color, 1 pair texture of coordinates */
};



//*****************************************************************************
//*****************************************************************************
enum X_VERTEX_SHADER_VERSION
{
	X_VSV_UNKNOWN,
	X_VSV_1_1,
	X_VSV_2_0,
	X_VSV_3_0
};



//*****************************************************************************
//*****************************************************************************
enum X_PIXEL_SHADER_VERSION
{
	X_PSV_UNKNOWN,
	X_PSV_1_1,
	X_PSV_1_2,
	X_PSV_1_3,
	X_PSV_1_4,
	X_PSV_2_0,
	X_PSV_3_0
};



//*****************************************************************************
//*****************************************************************************
enum X_DATA_FORMAT_TYPE
{
    X_DFT_UNKNOWN,
    X_DFT_R32G32B32A32_TYPELESS,
    X_DFT_R32G32B32A32_FLOAT,
    X_DFT_R32G32B32A32_UINT,
    X_DFT_R32G32B32A32_SINT,
    X_DFT_R32G32B32_TYPELESS,
    X_DFT_R32G32B32_FLOAT,
    X_DFT_R32G32B32_UINT,
    X_DFT_R32G32B32_SINT,
    X_DFT_R16G16B16A16_TYPELESS,
    X_DFT_R16G16B16A16_FLOAT,
    X_DFT_R16G16B16A16_UNORM,
    X_DFT_R16G16B16A16_UINT,
    X_DFT_R16G16B16A16_SNORM,
    X_DFT_R16G16B16A16_SINT,
    X_DFT_R32G32_TYPELESS,
    X_DFT_R32G32_FLOAT,
    X_DFT_R32G32_UINT,
    X_DFT_R32G32_SINT,
    X_DFT_R32G8X24_TYPELESS,
    X_DFT_D32_FLOAT_S8X24_UINT,
    X_DFT_R32_FLOAT_X8X24_TYPELESS,
    X_DFT_X32_TYPELESS_G8X24_UINT,
    X_DFT_R10G10B10A2_TYPELESS,
    X_DFT_R10G10B10A2_UNORM,
    X_DFT_R10G10B10A2_UINT,
    X_DFT_R11G11B10_FLOAT,
    X_DFT_R8G8B8A8_TYPELESS,
    X_DFT_R8G8B8A8_UNORM,
    X_DFT_R8G8B8A8_UNORM_SRGB,
    X_DFT_R8G8B8A8_UINT,
    X_DFT_R8G8B8A8_SNORM,
    X_DFT_R8G8B8A8_SINT,
    X_DFT_R16G16_TYPELESS,
    X_DFT_R16G16_FLOAT,
    X_DFT_R16G16_UNORM,
    X_DFT_R16G16_UINT,
    X_DFT_R16G16_SNORM,
    X_DFT_R16G16_SINT,
    X_DFT_R32_TYPELESS,
    X_DFT_D32_FLOAT,
    X_DFT_R32_FLOAT,
    X_DFT_R32_UINT,
    X_DFT_R32_SINT,
    X_DFT_R24G8_TYPELESS,
    X_DFT_D24_UNORM_S8_UINT,
    X_DFT_R24_UNORM_X8_TYPELESS,
    X_DFT_X24_TYPELESS_G8_UINT,
    X_DFT_R8G8_TYPELESS,
    X_DFT_R8G8_UNORM,
    X_DFT_R8G8_UINT,
    X_DFT_R8G8_SNORM,
    X_DFT_R8G8_SINT,
    X_DFT_R16_TYPELESS,
    X_DFT_R16_FLOAT,
    X_DFT_D16_UNORM,
    X_DFT_R16_UNORM,
    X_DFT_R16_UINT,
    X_DFT_R16_SNORM,
    X_DFT_R16_SINT,
    X_DFT_R8_TYPELESS,
    X_DFT_R8_UNORM,
    X_DFT_R8_UINT,
    X_DFT_R8_SNORM,
    X_DFT_R8_SINT,
    X_DFT_A8_UNORM,
    X_DFT_R1_UNORM,
    X_DFT_R9G9B9E5_SHAREDEXP,
    X_DFT_R8G8_B8G8_UNORM,
    X_DFT_G8R8_G8B8_UNORM,
    X_DFT_BC1_TYPELESS,
    X_DFT_BC1_UNORM,
    X_DFT_BC1_UNORM_SRGB,
    X_DFT_BC2_TYPELESS,
    X_DFT_BC2_UNORM,
    X_DFT_BC2_UNORM_SRGB,
    X_DFT_BC3_TYPELESS,
    X_DFT_BC3_UNORM,
    X_DFT_BC3_UNORM_SRGB,
    X_DFT_BC4_TYPELESS,
    X_DFT_BC4_UNORM,
    X_DFT_BC4_SNORM,
    X_DFT_BC5_TYPELESS,
    X_DFT_BC5_UNORM,
    X_DFT_BC5_SNORM,
    X_DFT_B5G6R5_UNORM,
    X_DFT_B5G5R5A1_UNORM,
    X_DFT_B8G8R8A8_UNORM,
    X_DFT_B8G8R8X8_UNORM
};



//****************************************************************************
//****************************************************************************
enum X_SORT_TYPE
{
	X_ST_NONE,
	X_ST_BACK_TO_FRONT,
	X_ST_FRONT_TO_BACK
};



//****************************************************************************
//****************************************************************************
enum X_COLOR_WRITE_FLAGS
{
    X_CWF_RED = 1,
    X_CWF_GREEN = 2,
    X_CWF_BLUE = 4,
    X_CWF_ALPHA = 8,
    X_CWF_ALL = (X_CWF_RED | X_CWF_GREEN | X_CWF_BLUE | X_CWF_ALPHA),
};



//****************************************************************************
//****************************************************************************
enum X_SCANLINE_ORDER
{
	X_SCANLINE_ORDER_UNKNOWN,
	X_SCANLINE_ORDER_PROGRESSIVE,
	X_SCANLINE_ORDER_UPPER_FIELD_FIRST,
	X_SCANLINE_ORDER_LOWER_FIELD_FIRST
};



//****************************************************************************
//****************************************************************************
enum X_SCALING_MODE
{	
	X_SCALING_UNKNOWN,
	X_SCALING_CENTERED,
	X_SCALING_STRETCHED
};



//****************************************************************************
//****************************************************************************
struct XViewport
{
	XS32 topLeftX;
	XS32 topLeftY;
	XU32 width;
	XU32 height;
	XF32 minDepth;
	XF32 maxDepth;

	XViewport()
	{
		topLeftX = 0;
		topLeftY = 0;
		width = 0;
		height = 0;
		minDepth = 0.0f;
		maxDepth = 1.0f;
	}
};



//****************************************************************************
//****************************************************************************
struct XViewportArrayDesc
{
	XU32 numViewports;
	XViewport viewports[X_MAX_VIEWPORTS];
	XBOOL enablePostEffects[X_MAX_VIEWPORTS];
	XBOOL copyToRenderContext[X_MAX_VIEWPORTS];

	XViewportArrayDesc()
	{
		numViewports = 0;
		for (XU32 i = 0;i < X_MAX_VIEWPORTS;++i)
		{
			enablePostEffects[i] = false;
			copyToRenderContext[i] = true;
		}
	}
};



//****************************************************************************
//****************************************************************************
struct XFullScreenModeDesc
{
	XU32 width;
    XU32 height;
    XU32 refreshRate;
    X_DATA_FORMAT_TYPE format;
    X_SCANLINE_ORDER scanlineOrdering;
    X_SCALING_MODE scalingMode;
};



//****************************************************************************
//****************************************************************************
struct XVertexDesc
{
	XU8 numColors;
	XU8 numNormals; 
	XU8 numUVs; 
	XU8 numTangents; 
	XU8 numBinormals; 
	XU8 numBoneIndices4Tuplets;
	XU8 numBoneWeights4Tuplets;

	XVertexDesc::XVertexDesc()
	{
		numColors = 0;
		numNormals = 0; 
		numUVs = 0; 
		numTangents = 0; 
		numBinormals = 0; 
		numBoneIndices4Tuplets = 0;
		numBoneWeights4Tuplets = 0;
	}
};



//*****************************************************************************
//*****************************************************************************
struct XDepthStencilDesc
{
    XBOOL depthTestEnable;
	XBOOL depthWriteEnable;
    X_CMP_FUNC_TYPE depthFunc;
	XBOOL stencilTestEnable;
    XU8 stencilReadMask;
    XU8 stencilWriteMask;
	X_STENCIL_OP_TYPE stencilFailOpFrontFace;
    X_STENCIL_OP_TYPE stencilDepthFailOpFrontFace;
    X_STENCIL_OP_TYPE stencilPassOpFrontFace;
    X_CMP_FUNC_TYPE stencilFuncFrontFace;
	X_STENCIL_OP_TYPE stencilFailOpBackFace;
    X_STENCIL_OP_TYPE stencilDepthFailOpBackFace;
    X_STENCIL_OP_TYPE stencilPassOpBackFace;
    X_CMP_FUNC_TYPE stencilFuncBackFace;
	XU32 stencilRefValue;

	XDepthStencilDesc::XDepthStencilDesc()
	{
		depthTestEnable = true;
		depthWriteEnable = true;
		depthFunc = X_CFT_LESS;
		stencilTestEnable = false;
		stencilReadMask = 0xFF;
		stencilWriteMask = 0xFF;
		stencilFailOpFrontFace = X_SOT_KEEP;
		stencilDepthFailOpFrontFace = X_SOT_KEEP;
		stencilPassOpFrontFace = X_SOT_KEEP;
		stencilFuncFrontFace = X_CFT_ALWAYS;
		stencilFailOpBackFace = X_SOT_KEEP;
		stencilDepthFailOpBackFace = X_SOT_KEEP;
		stencilPassOpBackFace = X_SOT_KEEP;
		stencilFuncBackFace = X_CFT_ALWAYS;
		stencilRefValue = 0;
	}
}; 	



//*****************************************************************************
//*****************************************************************************
struct XBlendDesc
{
    XBOOL alphaToCoverageEnable;
    XBOOL blendEnable[X_MAX_RENDER_TARGETS];
    X_BLEND_TYPE srcBlend;
    X_BLEND_TYPE destBlend;
    X_BLEND_OP_TYPE blendOp;
    X_BLEND_TYPE srcBlendAlpha;
    X_BLEND_TYPE destBlendAlpha;
    X_BLEND_OP_TYPE blendOpAlpha;
    X_COLOR_WRITE_FLAGS renderTargetWriteMask[X_MAX_RENDER_TARGETS];
	XColor blendFactor;
	XU32 sampleMask;

	XBlendDesc::XBlendDesc()
	{
		alphaToCoverageEnable = false;
		for (XU32 i = 0;i < X_MAX_RENDER_TARGETS;++i)
		{
			blendEnable[i] = false;
			renderTargetWriteMask[i] = X_CWF_ALL;
		}
		srcBlend = X_BT_ONE;
		destBlend = X_BT_ZERO;
		blendOp = X_BOT_ADD;
		srcBlendAlpha = X_BT_ONE;
		destBlendAlpha = X_BT_ZERO;
		blendOpAlpha = X_BOT_ADD;
		blendFactor = XColor(1.0f,1.0f,1.0f,1.0f);
		sampleMask = 0xFFFFFFFF;
	}
};



//*****************************************************************************
//*****************************************************************************
struct XRasterizerDesc
{
	X_FILL_MODE_TYPE fillMode;
    XBOOL frontCounterClockwise;
    X_CULL_TYPE cullMode;
    XS32 depthBias;
    XF32 depthBiasClamp;
    XF32 slopeScaledDepthBias;
    XBOOL depthClipEnable;
    XBOOL scissorEnable;
    XBOOL multisampleEnable;
    XBOOL antialiasedLineEnable;

	XRasterizerDesc::XRasterizerDesc()
	{
		fillMode = X_FMT_SOLID;
		frontCounterClockwise = false;
		cullMode = X_CULLT_BACK;
		depthBias = 0;
		depthBiasClamp = 0.0f;
		slopeScaledDepthBias = 0.0f;
		depthClipEnable = true;
		scissorEnable = true;
		multisampleEnable = false;
		antialiasedLineEnable = false;
	}
};



//*****************************************************************************
//*****************************************************************************
struct XSamplerDesc
{
	X_TEXTURE_FILTER_TYPE filter;
    X_TEXTURE_ADDRESS_TYPE addressU;
    X_TEXTURE_ADDRESS_TYPE addressV;
    X_TEXTURE_ADDRESS_TYPE addressW;
	XF32 mipLODBias;
    XU32 maxAnisotropy;
    X_CMP_FUNC_TYPE comparisonFunc;
    XColor borderColor;
    XF32 minLOD;
    XF32 maxLOD;

	XSamplerDesc::XSamplerDesc()
	{
		filter = X_TFT_MIN_MAG_MIP_LINEAR;
		addressU = X_TAT_WRAP;
		addressV = X_TAT_WRAP;
		addressW = X_TAT_WRAP;
		mipLODBias = 0.0f;
		maxAnisotropy = 1;
		comparisonFunc = X_CFT_NEVER;
		borderColor = XColor(1.0f,1.0f,1.0f,1.0f);
		minLOD = 0.0f;
		maxLOD = 10000.0f;
	}
};



//*****************************************************************************
//*****************************************************************************
struct XRenderTargetArrayDesc
{
	XU32 numRenderTargets;
	XU32 width;
	XU32 height;
	X_DATA_FORMAT_TYPE formats[X_MAX_RENDER_TARGETS];
	//XBOOL enableClears[X_MAX_RENDER_TARGETS];
	XColor clearColors[X_MAX_RENDER_TARGETS];
	XBOOL useDepthStencil;
	//X_DATA_FORMAT_TYPE depthStencilFormat;
	//XBOOL enableDepthStencilClear;
	XF32 depthClearValue;
	XU32 stencilClearValue;
	XU32 msaaCount;
	XU32 msaaQuality;

	XRenderTargetArrayDesc()
	{
		numRenderTargets = 0;
		width = 0;
		height = 0;
		useDepthStencil = false;
		//depthStencilFormat = X_DFT_UNKNOWN;
		for (XU32 i = 0;i < X_MAX_RENDER_TARGETS;++i)	
		{
			formats[i] = X_DFT_UNKNOWN;
			//enableClears[i] = true;
		}
		depthClearValue = 1.0f;
		stencilClearValue = 0;	
		msaaCount = 1;
		msaaQuality = 0;
	}
};


/** @} */ 

#endif