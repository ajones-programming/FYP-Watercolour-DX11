#include "Prototype1App.h"
#include "Vertex.h"


void PrototypeApp::CreateObject(std::string path, const float* scaling, const float* translation)
{
	allObjects.emplace_back(md3dDevice, path, scaling, translation);
}