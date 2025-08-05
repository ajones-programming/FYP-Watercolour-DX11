#ifndef CSFILTER_H
#define CSFILTER_H

#include <Windows.h>
#include <DirectXMath.h>
#include "d3dUtil.h"
#include "Effects.h"

class CSFilter
{
public:
	CSFilter();
	~CSFilter();


	ID3D11ShaderResourceView* GetOutput();

	///<summary>
	/// The width and height should match the dimensions of the input texture to blur.
	/// It is OK to call Init() again to reinitialize the blur filter with a different 
	/// dimension or format.
	///</summary>
	void virtual Init(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format);
	void virtual InitEffect() = 0;

	///<summary>
	/// Blurs the input texture blurCount times.  Note that this modifies the input texture, not a copy of it.
	///</summary>
	void Apply(ID3D11DeviceContext* dc, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* inputUAV, ID3D11ShaderResourceView** outputSRV);

protected:

	virtual void HorizontalConstants() {};
	virtual void VerticalConstants() {};

	UINT mWidth;
	UINT mHeight;
	DXGI_FORMAT mFormat;

	ID3D11ShaderResourceView* mOutputTexSRV;
	ID3D11UnorderedAccessView* mOutputTexUAV;

	CSEffect* effect;
};

#endif // BLURFILTER_H