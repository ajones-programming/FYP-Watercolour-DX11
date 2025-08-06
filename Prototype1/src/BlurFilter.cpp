//***************************************************************************************
// BlurFilter.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "BlurFilter.h"
#include "Effects.h"


void BlurFilter::SetGaussianWeights(float sigma)
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

	Effects::BlurFX->SetWeights(weights);
}

void BlurFilter::SetWeights(const float weights[11])
{
	Effects::BlurFX->SetWeights(weights);
}

void BlurFilter::BlurInPlace(ID3D11DeviceContext* dc,
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

void BlurFilter::InitEffect()
{
	effect = Effects::BlurFX;
}

void BlurFilter::HorizontalConstants()
{
}

void BlurFilter::VerticalConstants()
{
}