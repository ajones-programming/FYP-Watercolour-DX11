#include "ColourDensity.h"

#include "Effects.h"
#include "WICTextureLoader11.h"

ColourDensityFilter::ColourDensityFilter() : CSFilter(), mOverlayTexSRV(0)
{

}

ColourDensityFilter::~ColourDensityFilter()
{
	ReleaseCOM(mOverlayTexSRV);
}

void ColourDensityFilter::InitEffect()
{
	effect = Effects::ColourDensityFX;
}

void ColourDensityFilter::Init(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	CSFilter::Init(device, width, height, format);
	HR(CreateWICTextureFromFile(device, L"Textures/perlinNoise.png", 0, &mOverlayTexSRV));
	assert(mOverlayTexSRV != nullptr);
}

void ColourDensityFilter::HorizontalConstants()
{
	Effects::ColourDensityFX->SetOverlayMap(mOverlayTexSRV);
}

void ColourDensityFilter::VerticalConstants()
{
}