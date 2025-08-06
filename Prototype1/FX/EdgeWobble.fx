//=============================================================================
// Blur.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs a separable blur with a blur radius of 5.  
//=============================================================================

//ADAPTED FOR MEAN SHIFT
cbuffer cbSettings
{
    float NoiseIntensity = 5;
};

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = MIRROR;
    AddressV = MIRROR;
};

Texture2D gInput;
RWTexture2D<float4> gOutput;
Texture2D gPerlinNoise;

#define N 256
#define CacheSize (N)
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]
void EdgeWobbleCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    //float4 originalColor = gInput[int2(dispatchThreadID.x, dispatchThreadID.y)];
    
	
	//for sampling the image - found at https://gamedev.stackexchange.com/questions/116392/sampling-in-the-compute-shader-not-working
    float2 uv = float2(dispatchThreadID.xy) / float2(gInput.Length.xy);
    float2 sampledArea1 = gPerlinNoise.SampleLevel(samAnisotropic, uv*2, 0);
    float2 sampledArea2 = gPerlinNoise.SampleLevel(samAnisotropic, uv/2, 0);
	
    float2 newPosition = uv + (NoiseIntensity * (sampledArea1 - float2(0.5, 0.5)) / gInput.Length.xy) + (5 * (sampledArea2 - float2(0.5, 0.5)) / gInput.Length.xy);
    float4 sampleColour = gInput.SampleLevel(samAnisotropic, newPosition, 0);
    
    gCache[groupThreadID.x] = sampleColour;
    
    GroupMemoryBarrierWithGroupSync();
    
    gOutput[dispatchThreadID.xy] = gCache[groupThreadID.x];

}

[numthreads(1, N, 1)]
void ReturnCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    float4 sampleColour = gInput[min(dispatchThreadID.xy, gInput.Length.xy - 1)];
    gCache[groupThreadID.y] = sampleColour;
    GroupMemoryBarrierWithGroupSync();
    gOutput[dispatchThreadID.xy] = gCache[groupThreadID.y];
}

technique11 EdgeWobble
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, EdgeWobbleCS()));
    }
}

technique11 ReturnEdgeWobble
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, ReturnCS()));
    }
}