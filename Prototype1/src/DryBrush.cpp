
//***************************************************************************************
// DryBrushFilter.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Effects.h"
#include "DryBrush.h"
#include "WICTextureLoader11.h"

DryBrushFilter::DryBrushFilter() : CSFilter(), mOverlayTexSRV(0), mPerlinNoiseTexSRV(0)
{

}

DryBrushFilter::~DryBrushFilter()
{
	ReleaseCOM(mOverlayTexSRV);
	ReleaseCOM(mPerlinNoiseTexSRV);
}

void DryBrushFilter::InitEffect()
{
	effect = Effects::DryBrushFX;
}

void DryBrushFilter::Init(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	CSFilter::Init(device, width, height, format);
	HR(CreateWICTextureFromFile(device, L"Textures/papertexture.png", 0, &mOverlayTexSRV));
	HR(CreateWICTextureFromFile(device, L"Textures/perlinNoise.png", 0, &mPerlinNoiseTexSRV));
}

void DryBrushFilter::HorizontalConstants()
{
	Effects::DryBrushFX->SetOverlayMap(mOverlayTexSRV);
	Effects::DryBrushFX->SetPerlinMap(mPerlinNoiseTexSRV);
}

void DryBrushFilter::VerticalConstants()
{
}
