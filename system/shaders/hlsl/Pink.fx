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
};


//*****************************************************************************
//*****************************************************************************
VS_OUTPUT mainVS(VS_INPUT IN)				
{
	VS_OUTPUT OUT;
	
	OUT.position = mul(IN.position,worldViewProjMatrix);

	return OUT;
}



//*****************************************************************************
//*****************************************************************************
struct PS_INPUT
{
	float4 position         : SV_Position;
};



//*****************************************************************************
//*****************************************************************************
void mainPS(PS_INPUT IN,out float4 color : SV_Target)
{
	color = float4(1,0,1,1);
}



