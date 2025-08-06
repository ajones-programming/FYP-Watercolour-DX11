//=============================================================================
// Blur.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs a separable blur with a blur radius of 5.  
//=============================================================================

cbuffer cbSettings
{
    float gWeights[17] =
    {
        0,0,0,0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f, 0,0,0,
    };
};

cbuffer cbFixed
{
    static const int gBlurRadius = 8;
};

Texture2D gInput;
RWTexture2D<float4> gOutput;

#define N 256
#define CacheSize (N + 2*gBlurRadius)
groupshared float4 gCache[CacheSize];

float3 ColourBurnMaths(float3 originalColour, float3 burnColour)
{
    if (burnColour.r == 0 && burnColour.g == 0 && burnColour.b == 0)
    {
        return originalColour;
    }
    float3 inverted = float3(1, 1, 1) - originalColour;
    inverted /= burnColour;
    float3 result = float3(1, 1, 1) - inverted;
    return result;
}

[numthreads(N, 1, 1)]
void HorzEdgeDarkeningCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
	//
	// Fill local thread storage to reduce bandwidth.  To blur 
	// N pixels, we will need to load N + 2*BlurRadius pixels
	// due to the blur radius.
	//
	
	// This thread group runs N threads.  To get the extra 2*BlurRadius pixels, 
	// have 2*BlurRadius threads sample an extra pixel.
    if (groupThreadID.x < gBlurRadius)
    {
		// Clamp out of bound samples that occur at image borders.
        int x = max(dispatchThreadID.x - gBlurRadius, 0);
        gCache[groupThreadID.x] = gInput[int2(x, dispatchThreadID.y)];
    }
    if (groupThreadID.x >= N - gBlurRadius)
    {
		// Clamp out of bound samples that occur at image borders.
        int x = min(dispatchThreadID.x + gBlurRadius, gInput.Length.x - 1);
        gCache[groupThreadID.x + 2 * gBlurRadius] = gInput[int2(x, dispatchThreadID.y)];
    }

	// Clamp out of bound samples that occur at image borders.
    gCache[groupThreadID.x + gBlurRadius] = gInput[min(dispatchThreadID.xy, gInput.Length.xy - 1)];

	// Wait for all threads to finish.
    GroupMemoryBarrierWithGroupSync();
	
	//
	// Now blur each pixel.
	//

    float3 blurColor = float3(0, 0, 0);
    float3 originalColour = gInput[dispatchThreadID.xy];
    
	[unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = groupThreadID.x + gBlurRadius + i;
		
        blurColor += gWeights[i + gBlurRadius] * gCache[k].xyz;
    }
    
    float difference = sqrt(pow(blurColor.r - originalColour.r, 2) + pow(blurColor.g - originalColour.g, 2) + pow(blurColor.b - originalColour.b, 2));
    float3 newColour = originalColour * (1 - 1 * difference);
    
    float differenceIntensity = 0.5;
    float burnColour = (1 - difference) * differenceIntensity + (1 - differenceIntensity);
    gOutput[dispatchThreadID.xy] = float4(ColourBurnMaths(originalColour, float3(1, 1, 1) * burnColour), 1); //float4(newColour, 1);

}

[numthreads(1, N, 1)]
void VertEdgeDarkeningCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
	//
	// Fill local thread storage to reduce bandwidth.  To blur 
	// N pixels, we will need to load N + 2*BlurRadius pixels
	// due to the blur radius.
	//
	
	// This thread group runs N threads.  To get the extra 2*BlurRadius pixels, 
	// have 2*BlurRadius threads sample an extra pixel.
    if (groupThreadID.y < gBlurRadius)
    {
		// Clamp out of bound samples that occur at image borders.
        int y = max(dispatchThreadID.y - gBlurRadius, 0);
        gCache[groupThreadID.y] = gInput[int2(dispatchThreadID.x, y)];
    }
    if (groupThreadID.y >= N - gBlurRadius)
    {
		// Clamp out of bound samples that occur at image borders.
        int y = min(dispatchThreadID.y + gBlurRadius, gInput.Length.y - 1);
        gCache[groupThreadID.y + 2 * gBlurRadius] = gInput[int2(dispatchThreadID.x, y)];
    }
	
	// Clamp out of bound samples that occur at image borders.
    gCache[groupThreadID.y + gBlurRadius] = gInput[min(dispatchThreadID.xy, gInput.Length.xy - 1)];


	// Wait for all threads to finish.
    GroupMemoryBarrierWithGroupSync();
	
	//
	// Now blur each pixel.
	//
    
    float3 blurColor = float3(0, 0, 0);
    float3 originalColour = gInput[dispatchThreadID.xy];
    
	[unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = groupThreadID.y + gBlurRadius + i;
		
        blurColor += gWeights[i + gBlurRadius] * gCache[k].xyz;
    }
    
    float difference = sqrt(pow(blurColor.r - originalColour.r, 2) + pow(blurColor.g - originalColour.g, 2) + pow(blurColor.b - originalColour.b, 2));
    float3 newColour = originalColour * (1 - 2 * difference);
    
    float differenceIntensity = 0.5;
    float burnColour = (1 - difference) * differenceIntensity + (1 - differenceIntensity);
	
    gOutput[dispatchThreadID.xy] = float4(ColourBurnMaths(originalColour, float3(1, 1, 1) * burnColour), 1);
}

technique11 HorzEdgeDarkening
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, HorzEdgeDarkeningCS()));
    }
}

technique11 VertEdgeDarkening
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, VertEdgeDarkeningCS()));
    }
}
