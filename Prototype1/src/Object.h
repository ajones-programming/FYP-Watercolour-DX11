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

	void Draw(ID3D11DeviceContext* context, const XMMATRIX& view, const XMMATRIX& proj) const;

protected:
	int id = 0;
	
	XMFLOAT4X4 transform;
	
	ID3D11Buffer* mModelVB = nullptr;
	UINT modelVertexSize = 0;
	Material mModelMat{};

	ID3D11ShaderResourceView* mModelMapSRV = nullptr;
	XMVECTORF32 altColour;
};

