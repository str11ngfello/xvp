


//*****************************************************************************
//*****************************************************************************
struct VS_INPUT
{
    float4 position			: POSITION;
    float2 texCoord0			: TEXCOORD0;
    float2 texCoord1			: TEXCOORD1;
    float2 texCoord2			: TEXCOORD2;
};



//*****************************************************************************
//*****************************************************************************
struct VS_OUTPUT
{
    float4 position			: SV_POSITION;
    float2 texCoord0			: TEXCOORD0;
    float2 texCoord1			: TEXCOORD1;
    float2 texCoord2			: TEXCOORD2;
};



//*****************************************************************************
//*****************************************************************************
VS_OUTPUT mainVS(VS_INPUT IN)				
{
	VS_OUTPUT OUT;
	
	OUT.position = IN.position;
	OUT.texCoord0 = IN.texCoord0;
	OUT.texCoord1 = IN.texCoord1;
	OUT.texCoord2 = IN.texCoord2;

	return OUT;
}







//*****************************************************************************
//*****************************************************************************
struct PS_INPUT
{
	float4 position         : SV_Position;
	float4 diffuse			: COLOR0;
    float2 texCoord0		: TEXCOORD0;
};

Texture2D diffuseMap;            // Color texture for mesh
SamplerState MeshTextureSampler;




struct PS_OUTPUT
{
    float4 RGBColor : SV_TARGET;  // Pixel color
};

PS_OUTPUT mainPS( PS_INPUT In ) 
{ 
    PS_OUTPUT Output;

   
    Output.RGBColor = diffuseMap.Sample(MeshTextureSampler, In.texCoord0);// * float4(1,1,1,.2);//In.diffuse;
    

    return Output;
}

/*
//*****************************************************************************
//*****************************************************************************
void main(PS_INPUT IN,out float4 color : COLOR0,
			uniform sampler2D diffuseMap)
{
	color = IN.diffuse * tex2D(diffuseMap,IN.texCoord0);
}*/


