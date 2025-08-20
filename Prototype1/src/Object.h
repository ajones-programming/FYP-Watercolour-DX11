#pragma once
#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "DirectXMath.h"
#include <string>
#include <stdlib.h>

class Object
{
	//include transform somehow???
public:
	Object(ID3D11Device * device, std::string path, const float* scaling, const float* translation);
	Object(Object&& o);
	~Object();

	bool IsValid() { return isValid; }

	void Draw(ID3D11DeviceContext* context, const XMMATRIX& view, const XMMATRIX& proj) const;
	
	void setScale(const float* array)
	{
		FXMVECTOR scaleVec{ *(array), *(array + 1), *(array + 2) };
		XMStoreFloat3(&_scale, scaleVec);
		XMMATRIX boxScale = XMMatrixScaling(*(array), *(array + 1), *(array + 2));
		XMMATRIX boxOffset = XMMatrixTranslation(_translation.x, _translation.y, _translation.z);
		XMStoreFloat4x4(&transform, boxScale * boxOffset);
	}

	void setTranslation(const float* array)
	{
		FXMVECTOR translateVec{ *(array), *(array + 1), *(array + 2) };
		XMStoreFloat3(&_translation, translateVec);
		XMMATRIX boxScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
		XMMATRIX boxOffset = XMMatrixTranslation(*(array), *(array + 1), *(array + 2));
		XMStoreFloat4x4(&transform, boxScale * boxOffset);
	}

	XMFLOAT3 getScale() const { return _scale; };
	XMFLOAT3 getTranslation() const { return _translation; };
	XMFLOAT4X4 getTransform() const { return transform; }


protected:

	bool isValid = true;
	int id = 0;
	
	XMFLOAT4X4 transform;
	XMFLOAT3 _scale{1,1,1};
	XMFLOAT3 _translation{0,0,0};

	ID3D11Buffer* mModelVB = nullptr;
	UINT modelVertexSize = 0;
	Material mModelMat{};

	ID3D11ShaderResourceView* mModelMapSRV = nullptr;
	XMVECTORF32 altColour;
};

