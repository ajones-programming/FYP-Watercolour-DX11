#pragma once

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "DirectXMath.h"

enum RenderOptions
{
	Lighting = 0,
	Textures = 1,
	TexturesAndFog = 2
};

class PrototypeApp : public D3DApp
{
public:
	PrototypeApp(HINSTANCE hInstance);
	~PrototypeApp();

	bool Init(std::string objectAddress);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildCrateGeometryBuffers();
	void TEST_CreateGeometryBuffers(std::string objectAddress);

private:

	ID3D11Buffer* mBoxVB;
	ID3D11Buffer* mBoxIB;


	ID3D11Buffer* mModelVB;
	UINT modelVertexSize;
	Material mModelMat;

	ID3D11ShaderResourceView* mBoxMapSRV;

	ID3D11ShaderResourceView* mExampleSRV;

	DirectionalLight mDirLights[3];
	Material mBoxMat;

	XMFLOAT4X4 mBoxWorld;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	RenderOptions mRenderOptions;

	XMFLOAT3 mEyePosW;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};