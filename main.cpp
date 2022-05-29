#include "src/App.hpp"

#ifdef _WIN32
    #include <Windows.h>
#endif

int main()
{
#ifdef _WIN32
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

    App app{};
    return 0;
}
