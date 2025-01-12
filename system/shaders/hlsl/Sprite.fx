


//*****************************************************************************
//*****************************************************************************
struct VS_INPUT
{
    float4 position			: POSITION;
    float4 diffuse			: COLOR0;
    float2 texCoord0		: TEXCOORD0;
};



//*****************************************************************************
//*****************************************************************************
struct VS_OUTPUT
{
    float4  position		: SV_POSITION;
    float4  diffuse		: COLOR0;
    float2  texCoord0		: TEXCOORD0;
};



//*****************************************************************************
//*****************************************************************************
VS_OUTPUT mainVS(VS_INPUT IN)				
{
	VS_OUTPUT OUT;
	
	
	//OUT.position = float4(
	//						(((IN.position.x/1280.0f) -.5f)*2.0f),
	//						(((IN.position.y/1024.0f) -.5f)* 2.0f * -1),
	//						.5,1);
	
	OUT.position = IN.position;
	OUT.diffuse = IN.diffuse;
	OUT.texCoord0 = IN.texCoord0;

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

   
    Output.RGBColor = diffuseMap.Sample(MeshTextureSampler, In.texCoord0) * In.diffuse;
    

    return Output;
}


