#include "Object.h"
#include "OBJ_Loader.h"
#include "Vertex.h"
#include "d3dUtil.h"
#include "Effects.h"
#include "RenderStates.h"
#include "WICTextureLoader11.h"


static int totalID = 0;

Object::Object(ID3D11Device* device, std::string path, const float* scaling, const float* translation) : id(totalID++) {

	

	XMMATRIX boxScale = XMMatrixScaling(*(scaling), *(scaling + 1), *(scaling + 2));
	XMMATRIX boxOffset = XMMatrixTranslation(*(translation), *(translation + 1), *(translation + 2));
	XMStoreFloat4x4(&transform, boxScale * boxOffset);

	objl::Loader loader;
	bool loaded = loader.LoadFile(path);
	auto& oldVertices = loader.LoadedVertices;
	auto& material = loader.LoadedMaterials[0];

	modelVertexSize = oldVertices.size();

	std::vector<Vertex::Basic32> newVertices;

	//this is inefficient, CHANGE THIS!!
	for (auto& v : oldVertices) {
		Vertex::Basic32 newV;
		newV.Pos = DirectX::XMFLOAT3(-v.Position.X, v.Position.Y, v.Position.Z);
		newV.Normal = DirectX::XMFLOAT3(-v.Normal.X, v.Normal.Y, v.Normal.Z);
		newV.Tex = DirectX::XMFLOAT2(v.TextureCoordinate.X, 1-  v.TextureCoordinate.Y);
		newVertices.emplace_back(std::move(newV));
	}


	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * newVertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &newVertices[0];
	HR(device->CreateBuffer(&vbd, &vinitData, &mModelVB));

	mModelMat = Material();
	mModelMat.Ambient = DirectX::XMFLOAT4(material.Ka.X, material.Ka.Y, material.Ka.Z, 1);
	mModelMat.Diffuse = DirectX::XMFLOAT4(material.Kd.X, material.Kd.Y, material.Kd.Z, 1);
	mModelMat.Specular = DirectX::XMFLOAT4(material.Ks.X, material.Ks.Y, material.Ks.Z, 1);
	mModelMat.Reflect = DirectX::XMFLOAT4(1, 1, 1, 1);

	
	if (material.map_Kd != "") {
		std::wstring str(material.map_Kd.begin(), material.map_Kd.end());
		HR(CreateWICTextureFromFile(device,str.c_str(), 0, &mModelMapSRV));
	}
	else {
		HR(CreateWICTextureFromFile(device, L"../models/textures/CubeTexture.png", 0, &mModelMapSRV));
	}

	assert(mModelVB != nullptr);
	assert(&mModelMapSRV != nullptr);
}

Object::Object(Object&& o) :
	mModelVB(std::exchange(o.mModelVB, nullptr)),
	mModelMapSRV(std::exchange(o.mModelMapSRV, nullptr)),
	transform(o.transform),
	modelVertexSize(o.modelVertexSize),
	mModelMat(std::exchange(o.mModelMat, {}))
{

}

Object::~Object()
{
	if (mModelVB != nullptr) {
		ReleaseCOM(mModelVB);
		mModelVB = nullptr;
	}
	if (mModelMapSRV != nullptr) {
		ReleaseCOM(mModelMapSRV);
		mModelMapSRV = nullptr;
	}
}

void Object::Draw(ID3D11DeviceContext* context, const XMMATRIX& view, const XMMATRIX& proj) const
{

	ID3DX11EffectTechnique* boxTech = Effects::ToonShaderBasicFX->Light3TexAlphaClipFogTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	boxTech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{

		UINT offset = 0;
		UINT stride = sizeof(Vertex::Basic32);

		context->IASetVertexBuffers(0, 1, &mModelVB, &stride, &offset);

		//// Set per object constants.
		XMMATRIX world = XMLoadFloat4x4(&transform);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world * view * proj;

		Effects::ToonShaderBasicFX->SetWorld(world);
		Effects::ToonShaderBasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::ToonShaderBasicFX->SetWorldViewProj(worldViewProj);
		Effects::ToonShaderBasicFX->SetTexTransform(XMMatrixIdentity());
		Effects::ToonShaderBasicFX->SetMaterial(mModelMat);
		Effects::ToonShaderBasicFX->SetDiffuseMap(mModelMapSRV);

		context->RSSetState(RenderStates::NoCullRS);
		boxTech->GetPassByIndex(p)->Apply(0, context);
		context->Draw(modelVertexSize, 0);

		// Restore default render state.
		context->RSSetState(0);
	}
}
