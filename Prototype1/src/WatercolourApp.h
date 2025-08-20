#pragma once

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"
#include "DirectXMath.h"
#include "Object.h"


#include "CSFilter.h"

enum RenderOptions
{
	Lighting = 0,
	Textures = 1,
	TexturesAndFog = 2
};

class WatercolourApp : public D3DApp
{
public:
	WatercolourApp(HINSTANCE hInstance);
	~WatercolourApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	Object* CreateObject(std::string path, const float* scaling, const float* translation);

private:

	std::vector<const Object*> allObjects{};
	XMVECTORF32 clearColour{ 0.95,0.95,0.95,1 };
	DirectionalLight mDirLights[3];

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	RenderOptions mRenderOptions;

	XMFLOAT3 mEyePosW;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;

	//for compute shader
	CSFilter mPreMeanShiftBlur;
	CSFilter mMeanShift;
	CSFilter mPostMeanShiftBlur;
	CSFilter mDryBrushFilter;
	CSFilter mColourDensityFilter;
	CSFilter mEdgeWobbleFilter;
	CSFilter mEdgeDarkeningFilter;

	void DrawWrapper();
	void DrawScreenQuad(ID3D11ShaderResourceView* toDraw);
	void BuildScreenQuadGeometryBuffers();
	void BuildOffscreenViews();

	void InitCSFilters();

	ID3D11Buffer* mScreenQuadVB;
	ID3D11Buffer* mScreenQuadIB;

	ID3D11ShaderResourceView* mOffscreenSRV;
	ID3D11UnorderedAccessView* mOffscreenUAV;
	ID3D11RenderTargetView* mOffscreenRTV;
};