#pragma once

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"
#include "DirectXMath.h"
#include "Object.h"


#include "BlurFilter.h"
#include "MeanShiftFilter.h"
#include "DryBrush.h"
#include "ColourDensity.h"
#include "EdgeWobbleFilter.h"
#include "EdgeDarkeningFilter.h"
#include "TestFilter.h"

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

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	void CreateObject(std::string path, const float* scaling, const float* translation);

private:

	std::vector<Object> allObjects{};
	XMVECTORF32 clearColour{ 0.9,0.9,0.9,1 };
	DirectionalLight mDirLights[3];

	XMFLOAT4X4 mBoxWorld;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	RenderOptions mRenderOptions;

	XMFLOAT3 mEyePosW;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;

	//for compute shader
	BlurFilter mPreMeanShiftBlur;
	MeanShiftFilter mMeanShift;
	BlurFilter mPostMeanShiftBlur;
	DryBrushFilter mDryBrushFilter;
	ColourDensityFilter mColourDensityFilter;
	EdgeWobbleFilter mEdgeWobbleFilter;
	EdgeDarkeningFilter mEdgeDarkeningFilter;
	TestFilter mTestFilter;

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