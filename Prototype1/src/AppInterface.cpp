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





