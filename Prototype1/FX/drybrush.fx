//=============================================================================
// Blur.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs a separable blur with a blur radius of 5.  
//=============================================================================

//ADAPTED FOR MEAN SHIFT
cbuffer cbSettings
{
    float NoiseIntensity = 0.5;
    float Power = 0.5;
};

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

Texture2D gInput;
RWTexture2D<float4> gOutput;
Texture2D gOverlay;
Texture2D gPerlinNoise;

#define N 256
#define CacheSize (N)
groupshared float3 gCache[CacheSize];

#define N 256

float3 ColourBurn(float3 originalColor, float2 uv)
{
    //for sampling the image - found at https://gamedev.stackexchange.com/questions/116392/sampling-in-the-compute-shader-not-working
    float4 sampledArea = gPerlinNoise.SampleLevel(samAnisotropic, uv / 4, 0);
    
    originalColor = pow(originalColor, Power);
    float intensity = (originalColor.r + originalColor.g + originalColor.b) / 3;
    float3 inverted = float3(1, 1, 1) - originalColor;
    inverted /= (sampledArea * NoiseIntensity + (1 - NoiseIntensity));
    float3 result = float3(1, 1, 1) - inverted;
    return result * 1 + (result * intensity + originalColor * (1 - intensity)) * 0;
}

[numthreads(N, 1, 1)]
void DryBrushCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    float4 originalColor = gInput[int2(dispatchThreadID.x, dispatchThreadID.y)];
    
	
	//for sampling the image - found at https://gamedev.stackexchange.com/questions/116392/sampling-in-the-compute-shader-not-working
    float2 uv = float2(dispatchThreadID.xy) / float2(gInput.Length.xy);
    float4 sampledArea = gOverlay.SampleLevel(samAnisotropic, uv,0);
	
    float4 addedBrush = sampledArea * 0.1 + originalColor;
    
    //float3 output = ColourBurn(addedBrush.rgb, uv);
    
    gCache[groupThreadID.x] = addedBrush.xyz;
    
    GroupMemoryBarrierWithGroupSync();
    
    gOutput[dispatchThreadID.xy] = float4(gCache[groupThreadID.x], 1);

}

[numthreads(1, N, 1)]
void ReturnCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    float4 sampledColour = gInput[dispatchThreadID.xy];
    GroupMemoryBarrierWithGroupSync();
    gOutput[dispatchThreadID.xy] = sampledColour;
}

technique11 DryBrush
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, DryBrushCS()));
    }
}

technique11 Return
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, ReturnCS()));
    }
}