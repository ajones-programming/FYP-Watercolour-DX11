#ifndef CSFILTER_H
#define CSFILTER_H

#include <Windows.h>
#include <DirectXMath.h>
#include "d3dUtil.h"
#include "Effects.h"
#include <map>

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
	void InitEffects(ID3D11Device* device, const std::wstring& filename, const char* horzEffect, const char* vertEffect);

	///<summary>
	/// Blurs the input texture blurCount times.  Note that this modifies the input texture, not a copy of it.
	///</summary>
	void Apply(ID3D11DeviceContext* dc, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* inputUAV, ID3D11ShaderResourceView** outputSRV);
	void Apply(ID3D11DeviceContext* dc, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* inputUAV, ID3D11ShaderResourceView** outputSRV, int loopCount);
	void CreateAndApplyTexture(ID3D11Device* device, std::wstring url, std::string textureID);

	void setFloat(std::string id, float value) { effect->setFloat(id, value); }
	void setFloatArray(std::string id, const float* array, uint32_t size) { effect->setFloatArray(id, array, size); }

	

protected:

	std::map<std::string, ID3D11ShaderResourceView*> allTextures{};

	virtual void HorizontalConstants() {};
	virtual void VerticalConstants() {};

	UINT mWidth;
	UINT mHeight;
	DXGI_FORMAT mFormat;

	CSEffect* effect;

	ID3D11ShaderResourceView* mOutputTexSRV;
	ID3D11UnorderedAccessView* mOutputTexUAV;

	
};

#endif // BLURFILTER_H