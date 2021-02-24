#pragma once
#include<thread>
#include"..\server\WinSocketHeader.h"

using thread = std::thread;

class CDBServer {
private:
    HANDLE              iocp_;              //Iopc วุต้
    SOCKET              dbSocket_;          // dbSocket;

    void            ProcessPacket(int id, char* packet);
    void            WorkThread();
public:
    CDBServer() = default;
    ~CDBServer() = default;
    void Run();
};

