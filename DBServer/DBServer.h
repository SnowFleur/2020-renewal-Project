#pragma once
#include<thread>
#include"..\server\WinSocketHeader.h"

using thread = std::thread;

/*
DB Server를 관리하는 Class
DB Server는 Main Server와 network 통신을 진행한다.
DB Server는 IOCP로 구성되며 하나의 스레드로만 프로세스를 진행한다.
*/
class CDBServer {
private:
    HANDLE          iocp_;              //Iopc 해들
    SOCKET          dbSocket_;          // dbSocket;
    LPFN_CONNECTEX  LpfnConnectex;
    void            ProcessPacket(int id, char* packet);
    void            ConnectEx(SOCKET& socket, GUID guid);
public:
    CDBServer() = default;
    ~CDBServer() = default;
    void Run();
};

