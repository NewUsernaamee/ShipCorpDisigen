#include "MainWnd.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <fstream>
#include <string>
#include <tool_enabler.h>

#include <iostream>
#include <fstream>
#include <string>

#include <tool_enabler.h>

int main(int argc, char* argv[])
{
#if defined(WIN32) /*&& defined(HUNT_FOR_MEM_LEAKS)*/
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    std::ifstream F;
    std::string Key;
    std::string Signature;
    Application vapp;
    QApplication a(argc, argv);
    Math::SetMultithreadedMode(mtm_Off);
    F.open("c3d.lic");
    std::getline(F, Key);
    std::getline(F, Signature);
    F.close();
    EnableMathModules(Key.c_str(), Key.size(), Signature.c_str(), Signature.size());
    MainWnd w;
    if (!QtVision::activateLicense())
        return 0;
    
    w.show();
    return a.exec();

}
