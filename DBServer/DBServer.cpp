#include"DBServer.h"
#include"..\server\OverEx.h"
#include"..\server\LogCollector.h"

void CDBServer::Run() {

    //Init IOCP
    iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 1);

    thread dbThread{ &CDBServer::WorkThread,this };




    CLogCollector::GetInstance()->PrintLog("Start DB Server");
    dbThread.join();
}

void CDBServer::WorkThread() {

    DWORD       io_byte;
#ifdef  _WIN64
    ULONGLONG   l_key;
#else 
    ULONG   l_key;
#endif //  _WIN64
    OverEx* over_ex = nullptr;

    while (1) {
        int is_error = GetQueuedCompletionStatus(
            iocp_, &io_byte, &l_key, reinterpret_cast<LPWSAOVERLAPPED*>(&over_ex), INFINITE);

        //IOCP 핸드를 닫을경우 나옴
        if (is_error == 0) {
            if (over_ex == nullptr) {
                //GQCS 오류
            }
            else {
                //SOCKET 오류
            }
        }
    }

}

void CDBServer::ProcessPacket(int id, char* packet) {

}