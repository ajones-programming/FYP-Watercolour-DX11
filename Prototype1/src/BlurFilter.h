//***************************************************************************************
// BlurFilter.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs a blur operation on the topmost mip level of an input texture.
//***************************************************************************************

#ifndef BLURFILTER_H
#define BLURFILTER_H

#include <Windows.h>
#include <DirectXMath.h>
#include "CSFilter.h"
#include "d3dUtil.h"

class BlurFilter : public CSFilter
{
public:

	// Generate Gaussian blur weights.
	void SetGaussianWeights(float sigma);

	// Manually specify blur weights.
	void SetWeights(const float weights[11]);

	///<summary>
	/// Blurs the input texture blurCount times.  Note that this modifies the input texture, not a copy of it.
	///</summary>
	void BlurInPlace(ID3D11DeviceContext* dc, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* inputUAV, ID3D11ShaderResourceView** outputSRV, int blurCount);
	void InitEffect();

protected:
	void HorizontalConstants();
	void VerticalConstants();
};

#endif // BLURFILTER_H