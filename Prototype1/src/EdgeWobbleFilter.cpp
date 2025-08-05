// DryBrushFilter.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Effects.h"
#include "EdgeWobbleFilter.h"
#include "WICTextureLoader11.h"

EdgeWobbleFilter::EdgeWobbleFilter() : CSFilter(), mPerlinNoiseTexSRV(0)
{

}

EdgeWobbleFilter::~EdgeWobbleFilter()
{
	ReleaseCOM(mPerlinNoiseTexSRV);
}

void EdgeWobbleFilter::InitEffect()
{
	effect = Effects::EdgeWobbleFX;
}

void EdgeWobbleFilter::Init(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	CSFilter::Init(device, width, height, format);
	HR(CreateWICTextureFromFile(device, L"Textures/perlinNoise.png", 0, &mPerlinNoiseTexSRV));
}

void EdgeWobbleFilter::setSmallEdgeNoiseIntensity(float intensity)
{
	Effects::EdgeWobbleFX->SetSmallEdgeNoiseIntensity(intensity);
}

void EdgeWobbleFilter::HorizontalConstants()
{
	Effects::EdgeWobbleFX->SetPerlinMap(mPerlinNoiseTexSRV);
}

void EdgeWobbleFilter::VerticalConstants()
{
}
