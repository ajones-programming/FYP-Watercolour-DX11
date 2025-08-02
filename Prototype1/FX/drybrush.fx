//=============================================================================
// Blur.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs a separable blur with a blur radius of 5.  
//=============================================================================

//ADAPTED FOR MEAN SHIFT

cbuffer cbSettings
{
    float gWeights[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
    };
};

cbuffer cbFixed
{
    static const int gBlurRadius = 5;
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

#define N 256
#define CacheSize (N + 2*gBlurRadius)
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]
void DryBrushCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    float4 originalColor = gInput[int2(dispatchThreadID.x, dispatchThreadID.y)];
    
	
	//for sampling the image - found at https://gamedev.stackexchange.com/questions/116392/sampling-in-the-compute-shader-not-working
    float2 uv = float2(dispatchThreadID.xy) / float2(gInput.Length.xy);
    float4 sampledArea = gOverlay.SampleLevel(samAnisotropic, uv,0);
	
    //float4 sampledArea = gOverlay[int2(uv)];
	
    gOutput[dispatchThreadID.xy] = sampledArea * 0.1 + originalColor;

}

[numthreads(N, 1, 1)]
void ReturnCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    gOutput[dispatchThreadID.xy] = gInput[dispatchThreadID.xy];
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