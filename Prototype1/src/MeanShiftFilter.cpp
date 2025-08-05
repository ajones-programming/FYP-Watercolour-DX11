
//***************************************************************************************
// MeanShiftFilter.cpp
// Based on the BlurFilter.cpp by Frank D. Luna
//***************************************************************************************

#include "MeanShiftFilter.h"
#include "Effects.h"


void MeanShiftFilter::SetGaussianWeights(float sigma)
{
	float d = 2.0f * sigma * sigma;

	float weights[11];
	float sum = 0.0f;
	for (int i = 0; i < 11; ++i)
	{
		float x = (float)i - 5;
		weights[i] = expf(-x * x / d);

		sum += weights[i];
	}

	// Divide by the sum so all the weights add up to 1.0.
	for (int i = 0; i < 11; ++i)
	{
		weights[i] /= sum;
	}

	Effects::MeanShiftFX->SetWeights(weights);
}

void MeanShiftFilter::SetWeights(const float weights[11])
{
	Effects::MeanShiftFX->SetWeights(weights);
}

void MeanShiftFilter::SetBlurBoundary(float boundary) {
	Effects::MeanShiftFX->SetMagnitudeBoundary(boundary);
}

void MeanShiftFilter::BlurInPlace(ID3D11DeviceContext* dc,
	ID3D11ShaderResourceView* inputSRV,
	ID3D11UnorderedAccessView* inputUAV,
	ID3D11ShaderResourceView** outputSRV,
	int blurCount)
{

	for (int i = 0; i < blurCount; ++i)
	{
		CSFilter::Apply(dc, inputSRV, inputUAV, outputSRV);
	}
}

void MeanShiftFilter::InitEffect()
{
	effect = Effects::MeanShiftFX;
}

void MeanShiftFilter::HorizontalConstants()
{
}

void MeanShiftFilter::VerticalConstants()
{
}