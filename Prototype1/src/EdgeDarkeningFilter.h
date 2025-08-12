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
protected:
	void HorizontalConstants() {}
	void VerticalConstants() {}
};

#endif