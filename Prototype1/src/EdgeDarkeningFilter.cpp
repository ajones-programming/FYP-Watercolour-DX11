#include "EdgeDarkeningFilter.h"
#include "Effects.h"
#include "WICTextureLoader11.h"

EdgeDarkeningFilter::EdgeDarkeningFilter() : CSFilter()
{

}

void EdgeDarkeningFilter::SetGaussianWeights(float sigma)
{
	float d = 2.0f * sigma * sigma;

	float weights[17];
	float sum = 0.0f;
	for (int i = 0; i < 17; ++i)
	{
		float x = (float)i - 8;
		weights[i] = expf(-x * x / d);

		sum += weights[i];
	}

	// Divide by the sum so all the weights add up to 1.0.
	for (int i = 0; i < 17; ++i)
	{
		weights[i] /= sum;
	}

	Effects::EdgeDarkenFX->SetWeights(weights);
}

void EdgeDarkeningFilter::SetWeights(const float weights[17])
{
	Effects::EdgeDarkenFX->SetWeights(weights);
}

void EdgeDarkeningFilter::InitEffect()
{
	effect = Effects::EdgeDarkenFX;
}
