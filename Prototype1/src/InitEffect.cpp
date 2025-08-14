#include "WatercolourApp.h"

void setWeights(float sigma, CSFilter& filter, std::string effectID, const uint32_t size) {
	float d = 2.0f * sigma * sigma;

	float* weights = new float[size];
	float sum = 0.0f;
	for (int i = 0; i < size; ++i)
	{
		float x = (float)i - (size/2);
		*(weights + i) = expf(-x * x / d);

		sum += *(weights + i);
	}

	// Divide by the sum so all the weights add up to 1.0.
	for (int i = 0; i < size; ++i)
	{
		*(weights + i) /= sum;
	}
	filter.setShaderVariable(effectID, weights, size);

	delete[] weights;
}

void WatercolourApp::InitCSFilters()
{
	mPreMeanShiftBlur.InitEffects(md3dDevice, L"FX/Blur.fxo", "HorzBlur", "VertBlur");
	mPostMeanShiftBlur.InitEffects(md3dDevice, L"FX/Blur.fxo", "HorzBlur", "VertBlur");
	mMeanShift.InitEffects(md3dDevice, L"FX/MeanShift.fxo", "HorzMeanShift", "VertMeanShift");
	mDryBrushFilter.InitEffects(md3dDevice, L"FX/drybrush.fxo", "DryBrush", "Return");
	mColourDensityFilter.InitEffects(md3dDevice, L"FX/ColourDensity.fxo", "ColourDensity", "Return");
	mEdgeWobbleFilter.InitEffects(md3dDevice, L"FX/EdgeWobble.fxo", "EdgeWobble", "ReturnEdgeWobble");
	mEdgeDarkeningFilter.InitEffects(md3dDevice, L"FX/EdgeDarkening.fxo", "HorzEdgeDarkening", "VertEdgeDarkening");


	//move this to other part idk
	mMeanShift.setShaderVariable("magnitudeBoundary",0.03);
	mEdgeWobbleFilter.setShaderVariable("NoiseIntensity", 3);
	
	setWeights(3, mMeanShift, "gWeights", 11);
	setWeights(0.5, mPreMeanShiftBlur, "gWeights", 11);
	setWeights(0.4, mPostMeanShiftBlur, "gWeights", 11);
	setWeights(5, mEdgeDarkeningFilter, "gWeights", 17);
	
	mEdgeWobbleFilter.CreateAndApplyTexture(md3dDevice, L"Textures/perlinNoise.png", "gPerlinNoise");
	mDryBrushFilter.CreateAndApplyTexture(md3dDevice, L"Textures/papertexture.png", "gOverlay");
	mColourDensityFilter.CreateAndApplyTexture(md3dDevice, L"Textures/perlinNoise.png", "gOverlay");


}
