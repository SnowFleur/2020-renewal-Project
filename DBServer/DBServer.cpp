#include"DBServer.h"
#include"..\server\OverEx.h"
#include"..\server\LogCollector.h"
#include"..\server\Protocol.h"

void CDBServer::Run() {

    //Init IOCP
    iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 1);

    if (iocp_ == NULL) {
        CLogCollector::GetInstance()->PrintLog("IOCP HANDLE IS NULL");
        return;
    }

    //Init Socket
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        CLogCollector::GetInstance()->PrintLog("Can not load 'winsock.dll' file");
    }

    // 1. 소켓생성  
    dbSocket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (dbSocket_ == INVALID_SOCKET) {
        CLogCollector::GetInstance()->PrintLog("Can not Init Socket");
    }

    //Bind
    SOCKADDR_IN ServerAddr;
    ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
    ServerAddr.sin_family = PF_INET;
    ServerAddr.sin_port = htons(DB_SERVER_PORT);
    ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    // 2. Bind(ConnectEx는 미리 Bind가 되어야 한다.)
    if (bind(dbSocket_, reinterpret_cast<SOCKADDR*>(&ServerAddr), sizeof(ServerAddr)) == SOCKET_ERROR) {
        CLogCollector::GetInstance()->PrintLog("Fail bind", WSAGetLastError());
        // 6. 소켓종료
        closesocket(dbSocket_);
        // Winsock End
        WSACleanup();
        return;
    }

    //Main Server Address
    SOCKADDR_IN MainServerAddr;
    ZeroMemory(&MainServerAddr, sizeof(SOCKADDR_IN));
    MainServerAddr.sin_family = PF_INET;
    MainServerAddr.sin_port = htons(MAIN_SERVER_PORT);
    MainServerAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_ENDPOINT);

    // 3. Main Server에 Connect 요청
    if (LPFN_CONNECTEX(dbSocket_, (sockaddr*)&MainServerAddr, sizeof(MainServerAddr), NULL, NULL, NULL, NULL) == FALSE) {

        if (WSAGetLastError() != WSA_IO_PENDING) {
            std::cout << "Error Connect: To Main Server" << WSAGetLastError() << "\n";
            closesocket(dbSocket_);
        }
    }

    while (true) {

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

}

void CDBServer::ConnectEx(SOCKET& socket, GUID guid) {
    DWORD dwbyte{ 0 };
    WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
        &guid, sizeof(guid),
        &LpfnConnectex, sizeof(LpfnConnectex),
        &dwbyte, NULL, NULL);
}

void CDBServer::ProcessPacket(int id, char* packet) {

}