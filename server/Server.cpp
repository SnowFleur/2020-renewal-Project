#include<iostream>
#include"OverEx.h"            
#include"Server.h"
#include"Protocol.h"
#include"Sector.h"
#include"Network.h"
#include"Player.h"                  //Player Class
#include"LogCollector.h"            //Log Class


void CServer::Run() {

    //Init Sector
    sector_ = std::make_shared<CSector>();

    //Init IOCP
    iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    //Accept
    Thread acceptThread{ Thread{ &CServer::AcceptThread,this } };

    // Worker Thread 생성
    Threads workerThread;
    for (int i = 0; i < NUMBER_OF_THREAD; ++i)
        workerThread.emplace_back(Thread{ &CServer::WorkThread,this });

    //Timer
    Thread timerThread{ Thread{&CServer::TimerThread,this} };

    //DB Thread


    acceptThread.join();
    timerThread.join();
    for (auto& i : workerThread) {
        i.join();
    }

}

void CServer::WorkThread() {

    DWORD       io_byte;
#ifdef  _WIN64
    ULONGLONG   l_key;
#else 
    ULONG   l_key;
#endif //  _WIN64
    OverEx*     over_ex;

    while (true) {

        int is_error = GetQueuedCompletionStatus(
            iocp_, &io_byte, &l_key, reinterpret_cast<LPWSAOVERLAPPED*>(&over_ex), INFINITE);


        switch (over_ex->ev_){
        case EV_MOVE: {
            break;
        }
        case EV_SEND: {
            break;
        }
        case EV_RECV: {

            //패킷 재조립
            int rest = io_byte;
            char* ptr = over_ex->messageBuffer_;
            char packet_size = 0;

            if (0 < sector_->players_[l_key]->prevSize_)
                packet_size = sector_->players_[l_key]->packetBuffer_[0];


            // 패킷 조립
            while (0 < rest) {
                // 이전 기록이 없다면 패킷 사이즈를 ptr의 맨 처음으로 알 수 있다.
                if (0 == packet_size) packet_size = ptr[0];
                int required = packet_size - sector_->players_[l_key]->prevSize_;
                if (required <= rest) {
                    // 그냥 패킷 버퍼를 담을 경우 덮어쓸 위험이 있기 때문에
                    memcpy(sector_->players_[l_key]->packetBuffer_ +
                        sector_->players_[l_key]->prevSize_, ptr, required);

                    // 패킷 내의 데이터 처리[여기 까지]
                    ProcessPacket(l_key, sector_->players_[l_key]->packetBuffer_);

                    rest -= required;
                    ptr += required;

                    packet_size = 0;
                    sector_->players_[l_key]->prevSize_ = 0;
                }
                else {
                    //모자랄 경우
                    memcpy(sector_->players_[l_key]->packetBuffer_ + 
                        sector_->players_[l_key]->prevSize_, ptr, rest);
                    rest = 0;
                    sector_->players_[l_key]->prevSize_ += rest;

                }
            }
            //Recv
            NETWORK::Recv(sector_->players_[l_key]->socket_, sector_->players_[l_key]->overEx_);
            break;
        }

        default:
            break;
        }

    }

}

void CServer::AcceptThread() {

    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        CLogCollector::GetInstance()->PrintLog("Can not load 'winsock.dll' file");
    }

    // 1. 소켓생성  
    SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listenSocket == INVALID_SOCKET) {
        CLogCollector::GetInstance()->PrintLog("Can not load 'winsock.dll' file");
    }

    //Bind
    SOCKADDR_IN ServerAddr;
    ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
    ServerAddr.sin_family = PF_INET;
    ServerAddr.sin_port = htons(SERVER_PORT);
    ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    // 2. 소켓설정
    if (bind(listenSocket, reinterpret_cast<SOCKADDR*>(&ServerAddr), sizeof(ServerAddr)) == SOCKET_ERROR) {
        CLogCollector::GetInstance()->PrintLog("Fail bind", WSAGetLastError());
        // 6. 소켓종료
        closesocket(listenSocket);
        // Winsock End
        WSACleanup();
        return;
    }

    // 3. 수신대기열생성 , ListenSocket ,갯수
    if (listen(listenSocket, 5) == SOCKET_ERROR) {
        CLogCollector::GetInstance()->PrintLog("Fail listen", WSAGetLastError());
        // 6. 소켓종료
        closesocket(listenSocket);
        // Winsock End
        WSACleanup();
        return;
    }

    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(SOCKADDR_IN);
    memset(&clientAddr, 0, addrLen);
    SOCKET clientSocket;
    DWORD flags;

    while (1) {
        clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET){
            CLogCollector::GetInstance()->PrintLog("Accept Failure", WSAGetLastError());
        }

        //비어있는 Index 받아오기
        //일단 여기서 동적할당을 받고 나중에 풀에서 받자
        uIntType new_id;
        for (uIntType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
            if (sector_->players_[i]->isUsed_ == false) {
                new_id = i;
              
            }
        }
        //오류 여부 확인
        if (new_id == -1) {
            CLogCollector::GetInstance()->PrintLog("MAX User");
        }

        sector_->players_[new_id]->isUsed_ = true;
        sector_->players_[new_id]->socket_ = clientSocket;

#ifdef _DEBUG
        std::cout << "Accpet New Clinet\n";
#endif // _DEBUG


        // IOCP 객체와 소켓 연결 (socket, hIOCP, key, 0)
        CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), iocp_, new_id, 0);

        //Recv 시작
        NETWORK::Recv(sector_->players_[new_id]->socket_, sector_->players_[new_id]->overEx_);
    }

    // 리슨 소켓종료
    closesocket(listenSocket);

    // Winsock End
    WSACleanup();
}

void CServer::TimerThread() {}

void CServer::ProcessPacket(int id, char* packet) {

   

    switch (packet[1]){
    case CS_MOVE: {



        break;
    }
    case CS_LOGIN: {
        break;
    }
    default:
        break;
    }


}

