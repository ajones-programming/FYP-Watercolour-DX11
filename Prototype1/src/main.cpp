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


#include "AppInterface.h"

const std::string route = "../models/exported/";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	AppInterface theApp(hInstance);

	if (!theApp.Init())
		return 0;
	{
		float scaling[3] = { 3,3,3 };
		float translation[3] = { 0,0,0 };
		theApp.CreateObject(route + "landscape.obj", scaling, translation);
	}

	return theApp.Run();
}


