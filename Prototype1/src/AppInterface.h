#pragma once  
#include <wtypes.h>  
#include <string> // Include this to ensure std::string is recognized  

class AppInterface  
{  
public:  
   AppInterface(HINSTANCE hInstance);  
   ~AppInterface(); 

   void CreateObject(std::string path, const float* scaling, const float* translation);

   bool Init();  
   int Run();  
};
