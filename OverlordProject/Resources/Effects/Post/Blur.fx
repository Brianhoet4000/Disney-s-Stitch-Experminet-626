//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Mirror;
	AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState depthWrite
{
	DEPTHWRITEMASK = ALL;
};
/// Create Rasterizer State (Backface culling) 
RasterizerState backCull
{
	CullMode = back;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
	output.Position = float4(input.Position, 1.f);
	// Set the TexCoord
	output.TexCoord = input.TexCoord;

	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input) : SV_Target
{
	// Step 1: find the dimensions of the texture (the texture has a method for that)	
	// Step 2: calculate dx and dy (UV space for 1 pixel)


	// Step 3: Create a double for loop (5 iterations each)
	//		   Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//			Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
	// Step 4: Divide the final color by the number of passes (in this case 5*5)	
	// Step 5: return the final color


	uint width = 0;
	uint height = 0;
	gTexture.GetDimensions(width, height);

	float2 uv = input.TexCoord;

	float uvWidthOffset = (1.f / (float)width);
	float uvHeightOffset = (1.f / (float)height);

	float3 totalColor = (float3)0;
	int iterations = 5;

	float startW = uv.x - uvWidthOffset * (iterations / 2);
	float startH = uv.y - uvHeightOffset * (iterations / 2);
	for (int w = 0; w < iterations; ++w)
	{
		for (int h = 0; h < iterations; ++h)
		{
			float2 samUV = float2(startW + (w * uvWidthOffset), startH + (h * uvHeightOffset));
			totalColor = totalColor + gTexture.Sample(samPoint, samUV);
		}
	}


	totalColor = totalColor / (iterations * iterations);

	return float4(totalColor, 1.f);
}


//TECHNIQUE
//---------
technique11 Blur
{
	pass P0
	{
		// Set states...
		SetRasterizerState(backCull);
		SetDepthStencilState(depthWrite, 0);
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}