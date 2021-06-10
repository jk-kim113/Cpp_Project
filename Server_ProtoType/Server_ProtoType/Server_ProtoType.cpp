
#include <iostream>
#include <WinSock2.h>
#include "MainServer.h"

int main()
{
    std::cout << "Hello World!\n";

    MainServer* mainServer = new MainServer();

    std::cout << "End Process!\n";

    return 0;
}