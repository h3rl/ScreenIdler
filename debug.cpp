#include "debug.h"
#include <Windows.h>

HANDLE hConsole;

void createDbgConsole(){
#ifdef _DEBUG
    SetConsoleTitle("Debugging Console");
    if (!AllocConsole()) {
        // Add some error handling here.
        // You can call GetLastError() to get more info about the error.
        return;
    }

    // std::cout, std::clog, std::cerr, std::cin
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

#endif
}

void _S(const char* txt) {
#ifdef _DEBUG
    SetConsoleTextAttribute(hConsole, 10);
    _D("[OK] - " << txt);
    SetConsoleTextAttribute(hConsole, 15);
#endif
}

void _S(std::string txt) {
#ifdef _DEBUG
    _S(txt.c_str());
#endif
}

void _E(const char* txt) {
#ifdef _DEBUG
    SetConsoleTextAttribute(hConsole, 12);
    _D("[ERR] - " << txt);
    SetConsoleTextAttribute(hConsole, 15);
#endif
}

void _E(std::string txt) {
#ifdef _DEBUG
    _E(txt.c_str());
#endif
}