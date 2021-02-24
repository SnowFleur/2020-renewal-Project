#include<memory>
#include<iostream>
#include"DBServer.h"


int main() {
#ifdef _DEBUG
    std::cout << "=========Debug Mode=========\n\n";
#endif
    std::unique_ptr<CDBServer>uniPtrServer = std::make_unique<CDBServer>();
    uniPtrServer->Run();
}