SamplerState samPointMirror : register(s0)
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = MIRROR;
    AddressV = MIRROR;
};

Texture2D gTexture : register(t0);

cbuffer Constants : register(b0)
{
    float gGrainAmount = 0.1;
    float gGrainSize = 2.5;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.Position = float4(input.Position, 1.0);
    output.TexCoord = input.TexCoord;
    return output;
}

float GetNoise(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}

float4 PS(PS_INPUT input) : SV_Target
{
    float4 baseColor = gTexture.Sample(samPointMirror, input.TexCoord);

    float grainAmount = gGrainAmount;
    float grainSize = gGrainSize;
    float noise = GetNoise(input.TexCoord) * grainAmount;

    float4 filmGrain = float4(noise, noise, noise, 0.0);

    return baseColor - filmGrain;
}

technique GrainTechnique
{
    pass P0
    {
        SetDepthStencilState(EnableDepth, 0);
        SetRasterizerState(BackCulling);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
    
    pass P1
    {
        SetDepthStencilState(EnableDepth, 0);
        SetRasterizerState(BackCulling);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
    
    // Add more passes as needed
}
