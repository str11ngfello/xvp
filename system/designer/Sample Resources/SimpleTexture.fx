
//$2DTexture	$RGBA	$Base Color Texture		$Base color texture.

#include "XVPShader.hfx"


//*****************************************************************************
//*****************************************************************************
struct VS_INPUT
{
    float4 position			: POSITION;
    float4 diffuse			: COLOR0;
    float3 normal			: NORMAL;
    float3 texCoord0		: TEXCOORD0;
    float3 tangent			: TANGENT;
    float3 binormal			: BINORMAL;
};



//*****************************************************************************
//*****************************************************************************
struct VS_OUTPUT
{
    float4  position		: SV_Position;
	float4 diffuse			: COLOR0;
	float3 normal			: NORMAL;
    float3  texCoord0		: TEXCOORD0;
};


cbuffer NodeConstants : register(USER_SLOT0)
{
	float4x4 worldViewProjMatrix : packoffset(c0);
};


//*****************************************************************************
//*****************************************************************************
VS_OUTPUT mainVS(VS_INPUT IN)				
{
	VS_OUTPUT OUT;
	
	OUT.position = mul(IN.position,worldViewProjMatrix);
	OUT.texCoord0 = IN.texCoord0;

	return OUT;
}




//*****************************************************************************
//*****************************************************************************
struct PS_INPUT
{
	float4  position		: SV_Position;
	float4 diffuse			: COLOR0;
	float3 normal			: NORMAL;
    float3  texCoord0		: TEXCOORD0;
};


//*************************************************************************
//Maps
//*************************************************************************
Texture2D diffuseMap : register(t0);;           // Color texture for mesh
SamplerState MeshTextureSampler : register(s0);



//*****************************************************************************
//*****************************************************************************
float4 mainPS(PS_INPUT IN) : SV_Target			
{
	return diffuseMap.Sample(MeshTextureSampler,IN.texCoord0);
}



