#pragma once
#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "DirectXMath.h"
#include <string>

class Object
{
	//include transform somehow???

	Object(ID3D11Device * device, std::string path);
	void Draw(ID3D11DeviceContext* context, const XMFLOAT4X4& worldPos, const XMMATRIX& view, const XMMATRIX& proj);

protected:
	ID3D11Buffer* mModelVB = nullptr;
	UINT modelVertexSize = 0;
	Material mModelMat{};

	ID3D11ShaderResourceView* mModelMapSRV = nullptr;
};

