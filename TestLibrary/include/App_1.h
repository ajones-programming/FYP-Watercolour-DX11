#pragma once  
#include <wtypes.h>  
#include <string> // Include this to ensure std::string is recognized  

class App_1  
{  
public:  
   App_1(HINSTANCE hInstance);  
   ~App_1();  

   bool Init(std::string model);  
   int Run();  
};
