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

bool App_1::Init(std::string model)
{
	return app->Init(model);
}
 int App_1::Run()
{
	return app->Run();
}





