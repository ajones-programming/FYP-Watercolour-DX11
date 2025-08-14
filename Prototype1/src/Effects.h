//***************************************************************************************
// Adapted version of Effects.h by Frank Luna (C) 2011 All Rights Reserved.
// Do not redistribute for commercial purposes.
// 
// Defines lightweight effect wrappers to group an effect and its variables.
// Also defines a static Effects class from which we can access all of our effects.
//***************************************************************************************

#ifndef EFFECTS_H
#define EFFECTS_H

#include "d3dUtil.h"
#include <map>

#pragma region Effect
class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion

#pragma region BasicEffect
class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* device, const std::wstring& filename);
	~BasicEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique* Light2TexTech;
	ID3DX11EffectTechnique* Light3TexTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech;

	ID3DX11EffectTechnique* Light1FogTech;
	ID3DX11EffectTechnique* Light2FogTech;
	ID3DX11EffectTechnique* Light3FogTech;

	ID3DX11EffectTechnique* Light0TexFogTech;
	ID3DX11EffectTechnique* Light1TexFogTech;
	ID3DX11EffectTechnique* Light2TexFogTech;
	ID3DX11EffectTechnique* Light3TexFogTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
};
#pragma endregion

#pragma region CSEffect

class CSEffect : public Effect
{
public:
	CSEffect(ID3D11Device* device, const std::wstring& filename, const char* horzEffect, const char* vertEffect);
	~CSEffect();

	void SetInputAndOutputMaps(ID3D11ShaderResourceView* input, ID3D11UnorderedAccessView* output)
	{ 
		InputMap->SetResource(input);
		OutputMap->SetUnorderedAccessView(output);
	}

	ID3DX11EffectTechnique* GetHorzTechnique() { return HorzTech; }
	ID3DX11EffectTechnique* GetVertTechnique() { return VertTech; }





	void setShaderVariable(const std::string id, const float* array, const uint32_t size) {
		auto variable = floatArrayVariables.find(id);
		ID3DX11EffectScalarVariable* var;
		if (variable != floatArrayVariables.end())
		{
			var = variable->second;
		}
		else {
			auto newDefinition = mFX->GetVariableByName(id.c_str())->AsScalar();
			floatArrayVariables.insert({ id, newDefinition });
			var = newDefinition;
		}
		var->SetFloatArray(array, 0, size);
	}

	void setShaderVariable(const std::string id, const float value)
	{
		auto variable = floatVariables.find(id);
		ID3DX11EffectScalarVariable* var;
		if (variable != floatVariables.end())
		{
			var = variable->second;
		}
		else {
			auto newDefinition = mFX->GetVariableByName(id.c_str())->AsScalar();
			floatVariables.insert({ id, newDefinition });
			var = newDefinition;
		}
		var->SetFloat(value);
	}

	void setTexture(const std::string id, ID3D11ShaderResourceView* tex) {

		auto variable = textureVariables.find(id);
		ID3DX11EffectShaderResourceVariable * var;
		if (variable != textureVariables.end())
		{
			var = variable->second;
		}
		else {
			auto newDefinition = mFX->GetVariableByName(id.c_str())->AsShaderResource();
			textureVariables.insert({ id, newDefinition });
			var = newDefinition;
		}
		var->SetResource(tex);
	}


private:

	std::map<std::string, ID3DX11EffectScalarVariable*> floatArrayVariables{};
	std::map<std::string, ID3DX11EffectScalarVariable*> floatVariables{};
	std::map<std::string, ID3DX11EffectShaderResourceVariable*> textureVariables{};

	ID3DX11EffectShaderResourceVariable* InputMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputMap;

	ID3DX11EffectTechnique* HorzTech;
	ID3DX11EffectTechnique* VertTech;
};
#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
	static BasicEffect* ToonShaderBasicFX;

};
#pragma endregion

#endif // EFFECTS_H