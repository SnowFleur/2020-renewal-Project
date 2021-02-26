#pragma once
#include<thread>
#include"..\server\WinSocketHeader.h"

using thread = std::thread;

/*
DB Server�� �����ϴ� Class
DB Server�� Main Server�� network ����� �����Ѵ�.
DB Server�� IOCP�� �����Ǹ� �ϳ��� ������θ� ���μ����� �����Ѵ�.
*/
class CDBServer {
private:
    HANDLE      iocp_;              //Iopc �ص�
    SOCKET      dbSocket_;          // dbSocket;
    void        ProcessPacket(int id, char* packet);
public:
    CDBServer() = default;
    ~CDBServer() = default;
    void Run();
};

