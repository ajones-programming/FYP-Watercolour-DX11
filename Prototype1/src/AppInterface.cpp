#include "AppInterface.h"
#include "WatercolourApp.h"

static WatercolourApp* app = nullptr;

AppInterface::AppInterface(HINSTANCE hInstance) {
	app = new WatercolourApp(hInstance);
}

AppInterface::~AppInterface()
{
	delete app;
	app = nullptr;
}

bool AppInterface::Init()
{
	return app->Init();
}

void AppInterface::CreateObject(std::string path, const float* scaling, const float* translation) {
	app->CreateObject(path, scaling, translation);
}

int AppInterface::Run()
{
	return app->Run();
}


bool isEqual(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

bool isEqual(const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
}



bool isEqual(const XMFLOAT4X4& m1, const XMFLOAT4X4& m2)
{

	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			if (m1.m[x][y] != m2.m[x][y]) {
				return false;
			}
		}
	}
	return true;
}


const std::string route = "../models/exported/";


void AppInterface::RunTests()
{
	//TEST 1
	{
		float scaling[] = { 1,1,1 };
		float translation[] = { 0,0,0 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);
	}

	//TEST 2
	{
		float scaling[] = { 1,1,1 };
		float translation[] = { 0,0,0 };
		assert(app->CreateObject("INVALID PATH", scaling, translation) == nullptr);
	}
	
	//TEST 3
	{
		float scaling[] = { 1,1,1 };
		float translation[] = { 0,0,0 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(1, 1, 1)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(0, 0, 0)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)));
	}

	//TEST 4
	{
		float scaling[] = { 1,1,1 };
		float translation[] = { 10,11,12 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(1, 1, 1)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(10, 11, 12)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 10, 11, 12, 1)));
	}

	//TEST 5
	{
		float scaling[] = { 1,1,1 };
		float translation[] = { -10,-11,-12 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(1, 1, 1)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(-10, -11, -12)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -10, -11, -12, 1)));
	}


	//TEST 6
	{
		float scaling[] = { 1,1,1 };
		float translation[] = { 10000,11000,12000 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(1, 1, 1)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(10000, 11000, 12000)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 10000, 11000, 12000, 1)));
	}

	//TEST 7
	{
		float scaling[] = { 1,1,1 };
		float translation[] = { -10000,-11000,-12000 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(1, 1, 1)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(-10000, -11000, -12000)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -10000, -11000, -12000, 1)));
	}
	
	//TEST 8
	{
		float scaling[] = { 1,1,1 };
		float translation[] = { 0,0,0 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(1, 1, 1)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(0, 0, 0)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)));
	}
	
	//TEST 9
	{
		float scaling[] = { 0,0,0 };
		float translation[] = { 0,0,0 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(0, 0, 0)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(0, 0, 0)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)));
	}

	//TEST 10
	{
		float scaling[] = { 10,11,12 };
		float translation[] = { 0,0,0 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(10, 11, 12)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(0, 0, 0)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(10, 0, 0, 0, 0, 11, 0, 0, 0, 0, 12, 0, 0, 0, 0, 1)));
	}

	//TEST 11
	{
		float scaling[] = { -10,-11,-12 };
		float translation[] = { 0,0,0 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(-10, -11, -12)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(0, 0, 0)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(-10, 0, 0, 0, 0, -11, 0, 0, 0, 0, -12, 0, 0, 0, 0, 1)));
	}

	//TEST 12
	{
		float scaling[] = { 10000,11000,12000 };
		float translation[] = { 0,0,0 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(10000, 11000, 12000)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(0, 0, 0)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(10000, 0, 0, 0, 0, 11000, 0, 0, 0, 0, 12000, 0, 0, 0, 0, 1)));
	}

	//TEST 13
	{
		float scaling[] = { -10000,-11000,-12000 };
		float translation[] = { 0,0,0 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(-10000, -11000, -12000)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(0, 0, 0)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(-10000, 0, 0, 0, 0, -11000, 0, 0, 0, 0, -12000, 0, 0, 0, 0, 1)));
	}

	//TEST 10
	{
		float scaling[] = { 10,11,12 };
		float translation[] = { 4,5,6 };
		const Object* object = app->CreateObject(route + "cube.obj", scaling, translation);
		assert(object != nullptr);

		assert(isEqual(object->getScale(), XMFLOAT3(10, 11, 12)));
		assert(isEqual(object->getTranslation(), XMFLOAT3(4, 5, 6)));
		assert(isEqual(object->getTransform(), XMFLOAT4X4(10, 0, 0, 0, 0, 11, 0, 0, 0, 0, 12, 0, 4, 5, 6, 1)));
	}
}