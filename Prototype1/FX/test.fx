//=============================================================================
// Blur.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs a separable blur with a blur radius of 5.  
//=============================================================================

Texture2D gInput;
RWTexture2D<float4> gOutput;

#define N 256
#define CacheSize (N)
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]
void HorzTESTCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{

	// Clamp out of bound samples that occur at image borders.
    gCache[groupThreadID.x] = gInput[min(dispatchThreadID.xy, gInput.Length.xy - 1)];

	// Wait for all threads to finish.
    GroupMemoryBarrierWithGroupSync();
	
	//
	// Now blur each pixel.
	//

    float4 returnColour = gCache[groupThreadID.x];
	
    gOutput[dispatchThreadID.xy] = returnColour * float4(1, 0.8, 0.8, 1);
}

[numthreads(1, N, 1)]
void VertTESTCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
	
	// Clamp out of bound samples that occur at image borders.
    gCache[groupThreadID.y] = gInput[min(dispatchThreadID.xy, gInput.Length.xy - 1)];


	// Wait for all threads to finish.
    GroupMemoryBarrierWithGroupSync();
	
	//
	// Now blur each pixel.
	//

    float4 returnColour = gCache[groupThreadID.y];
	
    gOutput[dispatchThreadID.xy] = returnColour * float4(0.8, 0.8, 1, 1);
}

technique11 HorzTEST
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, HorzTESTCS()));
    }
}

technique11 VertTEST
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, VertTESTCS()));
    }
}
