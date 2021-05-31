#include <iostream>

#ifdef _DEBUG

#define _D(x) std::cout << x << std::endl;

#else

#define _D(x)

#endif // _DEBUG

void createDbgConsole();
void _S(const char* txt);
void _S(std::string txt);
void _E(const char* txt);
void _E(std::string txt);