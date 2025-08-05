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

#define N 256

[numthreads(N, 1, 1)]
void ColourDensityCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    float3 originalColor = gInput[int2(dispatchThreadID.x, dispatchThreadID.y)];
    
	
	//for sampling the image - found at https://gamedev.stackexchange.com/questions/116392/sampling-in-the-compute-shader-not-working
    float2 uv = float2(dispatchThreadID.xy) / float2(gInput.Length.xy);
    float4 sampledArea = gOverlay.SampleLevel(samAnisotropic, uv/4, 0);
    
    originalColor = pow(originalColor, Power);
    float intensity = (originalColor.r + originalColor.g + originalColor.b) / 3;
    float3 inverted = float3(1, 1, 1) - originalColor;
    inverted /= (sampledArea * NoiseIntensity + (1 - NoiseIntensity));
    float3 result = float3(1, 1, 1) - inverted;
    float3 toReturn = result * 1 + (result * intensity + originalColor * (1 - intensity)) * 0;
    
    //gOutput[dispatchThreadID.xy] = float4(originalColor, 1);
    gOutput[dispatchThreadID.xy] = float4(toReturn, 1); //sampledArea * 0.1 + originalColor;

}

[numthreads(1, N, 1)]
void ReturnCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    gOutput[dispatchThreadID.xy] = gInput[dispatchThreadID.xy];
}

technique11 ColourDensity
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, ColourDensityCS()));
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