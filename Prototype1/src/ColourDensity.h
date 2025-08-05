#ifndef COLOURDENSITYFILTER_H
#define COLOURDENSITYFILTER_H


#include <Windows.h>
#include <DirectXMath.h>
#include "d3dUtil.h"
#include "CSFilter.h"

class ColourDensityFilter : public CSFilter
{
public:
	ColourDensityFilter();
	~ColourDensityFilter();
	///<summary>
	/// Blurs the input texture blurCount times.  Note that this modifies the input texture, not a copy of it.
	///</summary>
	void InitEffect();
	void Init(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format);

protected:
	void HorizontalConstants();
	void VerticalConstants();

	ID3D11ShaderResourceView* mOverlayTexSRV;
};

#endif