#pragma once
#ifndef EdgeDarkeningFilter_H
#define EdgeDarkeningFilter_H

#include <Windows.h>
#include <DirectXMath.h>
#include "d3dUtil.h"
#include "CSFilter.h"

class EdgeDarkeningFilter : public CSFilter
{
public:
	EdgeDarkeningFilter();

	// Generate Gaussian blur weights.
	void SetGaussianWeights(float sigma);

	// Manually specify blur weights.
	void SetWeights(const float weights[17]);

	///<summary>
	/// Blurs the input texture blurCount times.  Note that this modifies the input texture, not a copy of it.
	///</summary>
	void InitEffect();
protected:
	void HorizontalConstants() {}
	void VerticalConstants() {}
};

#endif