#include "Prototype1App.h"
#include "OBJ_Loader.h"
#include "Vertex.h"

void PrototypeApp::TEST_CreateGeometryBuffers(std::string objectAddress)
{
	objl::Loader loader;
	bool loaded = loader.LoadFile(objectAddress);
	auto& oldVertices = loader.LoadedVertices;
	auto& material = loader.LoadedMaterials[0];

	modelVertexSize = oldVertices.size();

	std::vector<Vertex::Basic32> newVertices;

	//this is inefficient, CHANGE THIS!!
	for (auto& v : oldVertices) {
		Vertex::Basic32 newV;
		newV.Pos = DirectX::XMFLOAT3(v.Position.X, v.Position.Y, v.Position.Z);
		newV.Normal = DirectX::XMFLOAT3(v.Normal.X, v.Normal.Y, v.Normal.Z);
		newV.Tex = DirectX::XMFLOAT2(v.TextureCoordinate.X, v.TextureCoordinate.Y);
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
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mModelVB));

	mModelMat = Material();
	mModelMat.Ambient = DirectX::XMFLOAT4( material.Ka.X, material.Ka.Y, material.Ka.Z, 1);
	mModelMat.Diffuse = DirectX::XMFLOAT4(material.Kd.X, material.Kd.Y, material.Kd.Z, 1);
	mModelMat.Specular = DirectX::XMFLOAT4(material.Ks.X, material.Ks.Y, material.Ks.Z, 1);
	mModelMat.Reflect = DirectX::XMFLOAT4(1,1,1, 1);
	
}