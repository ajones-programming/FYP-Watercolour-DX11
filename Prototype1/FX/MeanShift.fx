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
    float magnitudeBoundary = 0.4;
};

cbuffer cbFixed
{
    static const int gBlurRadius = 5;
};

Texture2D gInput;
RWTexture2D<float4> gOutput;

#define N 256
#define CacheSize (N + 2*gBlurRadius)
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]
void HorzMeanShiftCS(int3 groupThreadID : SV_GroupThreadID,
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

    float4 blurColor = float4(0, 0, 0, 0);
    float4 originalColor = gCache[groupThreadID.x + gBlurRadius];
    //add all the weights, then divide result
    float totalWeights = 0;
	
	[unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = groupThreadID.x + gBlurRadius + i;
        float4 result = gCache[k] - originalColor;
        float magnitude = sqrt(result.r * result.r + result.g * result.g + result.b * result.b);
        if (magnitude < magnitudeBoundary)
        {
            blurColor += gWeights[i + gBlurRadius] * gCache[k];
            totalWeights += gWeights[i + gBlurRadius];
        }
    }
    
    blurColor /= totalWeights;
	
    gOutput[dispatchThreadID.xy] = blurColor;
}

[numthreads(1, N, 1)]
void VertMeanShiftCS(int3 groupThreadID : SV_GroupThreadID,
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
    float4 originalColor = gCache[groupThreadID.y + gBlurRadius];
    float4 blurColor = float4(0, 0, 0, 0);
    float totalWeights = 0;
	
	[unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = groupThreadID.y + gBlurRadius + i;
        float4 result = gCache[k] - originalColor;
        
        float magnitude = sqrt(result.r * result.r + result.g * result.g + result.b * result.b);
        if (magnitude < magnitudeBoundary)
        {
            blurColor += gWeights[i + gBlurRadius] * gCache[k];
            totalWeights += gWeights[i + gBlurRadius];
        }
		
        //blurColor += gWeights[i + gBlurRadius] * gCache[k];
    }
    blurColor /= totalWeights;
    gOutput[dispatchThreadID.xy] = blurColor;
}

technique11 HorzMeanShift
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, HorzMeanShiftCS()));
    }
}

technique11 VertMeanShift
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, VertMeanShiftCS()));
    }
}
