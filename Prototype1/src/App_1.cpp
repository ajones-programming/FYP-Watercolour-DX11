#include "App_1.h"
#include "Prototype1App.h"

static PrototypeApp* app = nullptr;

App_1::App_1(HINSTANCE hInstance) {
	app = new PrototypeApp(hInstance);
}

App_1::~App_1()
{
	delete app;
	app = nullptr;
}

bool App_1::Init()
{
	return app->Init();
}

void App_1::CreateObject(std::string path, const float* scaling, const float* translation) {
	app->CreateObject(path, scaling, translation);
}

 int App_1::Run()
{
	return app->Run();
}





