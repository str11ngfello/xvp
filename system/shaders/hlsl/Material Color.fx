
//$$ |color |Material Color |Material diffuse color



#include "XVPShader.hfx"


//*****************************************************************************
//*****************************************************************************
struct VS_INPUT
{
    float4 position			: POSITION;
};



//*****************************************************************************
//*****************************************************************************
struct VS_OUTPUT
{
    float4  position		: SV_Position;
};



//*************************************************************************
//Transforms
//*************************************************************************
cbuffer NodeProperties : register(USER_SLOT0)
{
	float4x4 worldViewProjMatrix : packoffset(c0);
	float depthBias : packoffset(c4);
};



//*****************************************************************************
//*****************************************************************************
VS_OUTPUT mainVS(VS_INPUT IN)				
{
	VS_OUTPUT OUT;
	OUT.position = mul(IN.position,worldViewProjMatrix);
	OUT.position.z += depthBias;
	return OUT;
}



cbuffer PSConstants : register(USER_SLOT1)
{
	float4 color : packoffset(c0);
};


//*****************************************************************************
//*****************************************************************************
struct PS_INPUT
{
	float4 position         : SV_Position;
};



//*****************************************************************************
//*****************************************************************************
void mainPS(PS_INPUT IN,out float4 outputColor : SV_Target)
{
	outputColor = color;
}


