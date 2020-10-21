#include<memory>
#include<iostream>
#include "Server.h"

int main() {
#ifdef _DEBUG
    std::cout << "=========Debug Mode=========\n\n";
#endif
    std::unique_ptr<CServer>uniPtrServer = std::make_unique<CServer>();
    uniPtrServer->Run();
}