#pragma once
//***************************************************************************************
// DryBrushFilter.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs a blur operation on the topmost mip level of an input texture.
//***************************************************************************************

#ifndef EdgeWobbleFilter_H
#define EdgeWobbleFilter_H

#include <Windows.h>
#include <DirectXMath.h>
#include "d3dUtil.h"
#include "CSFilter.h"

class EdgeWobbleFilter : public CSFilter
{
public:
	EdgeWobbleFilter();
	~EdgeWobbleFilter();
	///<summary>
	/// Blurs the input texture blurCount times.  Note that this modifies the input texture, not a copy of it.
	///</summary>
	void InitEffect();
	void Init(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format);
	void setSmallEdgeNoiseIntensity(float intensity);

protected:
	void HorizontalConstants();
	void VerticalConstants();

	ID3D11ShaderResourceView* mPerlinNoiseTexSRV;
};

#endif