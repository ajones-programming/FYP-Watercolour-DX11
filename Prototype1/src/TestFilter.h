#pragma once
#ifndef TestFilter_H
#define TestFilter_H

#include <Windows.h>
#include <DirectXMath.h>
#include "d3dUtil.h"
#include "CSFilter.h"

class TestFilter : public CSFilter
{
public:
	TestFilter();
	///<summary>
	/// Blurs the input texture blurCount times.  Note that this modifies the input texture, not a copy of it.
	///</summary>
	void InitEffect();
protected:
	void HorizontalConstants() {}
	void VerticalConstants() {}
};

#endif