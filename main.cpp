#include "src/App.hpp"

#ifdef _WIN32
    #include <Windows.h>
#endif

#include <iostream>

int main()
{
#ifdef _WIN32
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

#ifdef NDEBUG
    std::cout << "Release mode\n";
#else 
    std::cout << "Debug mode\n";
#endif

    App app{};
    return 0;
}
