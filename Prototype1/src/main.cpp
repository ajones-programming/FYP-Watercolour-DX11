//***************************************************************************************
// BoxDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates rendering a colored box.
//
// Controls:
//		Hold the left mouse button down and move the mouse to rotate.
//      Hold the right mouse button down to zoom in and out.
//
//***************************************************************************************


#include "App_1.h"

const std::string route = "D://Files/University/Modules/TM470 Final Year Project/ExampleModels/exported/";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	App_1 theApp(hInstance);

	if (!theApp.Init())
		return 0;
	//{
	//	float scaling[3] = { 3,3,3 };
	//	float translation[3] = { 0,0,0 };
	//	theApp.CreateObject(route + "texturedcube.obj", scaling, translation);
	//}
	//{
	//	float scaling[3] = { 3,3,3 };
	//	float translation[3] = { 18,1,1 };
	//	theApp.CreateObject(route + "cube.obj", scaling, translation);
	//}

	//{
	//	float scaling[3] = { 1,1,1 };
	//	float translation[3] = { -5,5,1 };
	//	theApp.CreateObject(route + "apple.obj", scaling, translation);
	//}

	//{
	//	float scaling[3] = { 1,1,1 };
	//	float translation[3] = { 5,6,1 };
	//	theApp.CreateObject(route + "apple.obj", scaling, translation);
	//}

	{
		float scaling[3] = { 3,3,3 };
		float translation[3] = { 0,0,0 };
		theApp.CreateObject(route + "landscape.obj", scaling, translation);
	}

	return theApp.Run();
}


