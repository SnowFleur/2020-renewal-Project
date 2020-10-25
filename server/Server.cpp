#include<iostream>
#include"OverEx.h"            
#include"Server.h"
#include"Protocol.h"
#include"Sector.h"
#include"Network.h"
#include"Player.h"                  //Player Class
#include"LogCollector.h"            //Log Class
#include"GameObject.h"

void CServer::Run() {

    //Init Sector
    sector_ = std::make_shared<CSector>();

    //Init IOCP
    iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    //Accept
    Thread acceptThread{ Thread{ &CServer::AcceptThread,this } };

    // Worker Thread ����
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

        // [2020.10.24] å �����ؼ� �ٽ� ����
        if (is_error == 0) {

        }

        switch (over_ex->ev_){
        case EV_MOVE: {
            break;
        }
        case EV_SEND: {
            //���߿� �޸�Ǯ�� ����
            delete over_ex;
            break;
        }
        case EV_RECV: {

            //��Ŷ ������
            int rest = io_byte;
            char* ptr = over_ex->messageBuffer_;
            char packet_size = 0;

            if (0 < sector_->players_[l_key]->prevSize_)
                packet_size = sector_->players_[l_key]->packetBuffer_[0];


            // ��Ŷ ����
            while (0 < rest) {
                // ���� ����� ���ٸ� ��Ŷ ����� ptr�� �� ó������ �� �� �ִ�.
                if (0 == packet_size) packet_size = ptr[0];
                int required = packet_size - sector_->players_[l_key]->prevSize_;
                if (required <= rest) {
                    // �׳� ��Ŷ ���۸� ���� ��� ��� ������ �ֱ� ������
                    memcpy(sector_->players_[l_key]->packetBuffer_ +
                        sector_->players_[l_key]->prevSize_, ptr, required);

                    // ��Ŷ ���� ������ ó��[���� ����]
                    ProcessPacket(l_key, sector_->players_[l_key]->packetBuffer_);

                    rest -= required;
                    ptr += required;

                    packet_size = 0;
                    sector_->players_[l_key]->prevSize_ = 0;
                }
                else {
                    //���ڶ� ���
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

    // 1. ���ϻ���  
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

    // 2. ���ϼ���
    if (bind(listenSocket, reinterpret_cast<SOCKADDR*>(&ServerAddr), sizeof(ServerAddr)) == SOCKET_ERROR) {
        CLogCollector::GetInstance()->PrintLog("Fail bind", WSAGetLastError());
        // 6. ��������
        closesocket(listenSocket);
        // Winsock End
        WSACleanup();
        return;
    }

    // 3. ���Ŵ�⿭���� , ListenSocket ,����
    if (listen(listenSocket, 5) == SOCKET_ERROR) {
        CLogCollector::GetInstance()->PrintLog("Fail listen", WSAGetLastError());
        // 6. ��������
        closesocket(listenSocket);
        // Winsock End
        WSACleanup();
        return;
    }

    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(SOCKADDR_IN);
    memset(&clientAddr, 0, addrLen);
    SOCKET clientSocket= INVALID_SOCKET;
    DWORD flags;

    while (1) {
        clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET){
            CLogCollector::GetInstance()->PrintLog("Accept Failure", WSAGetLastError());
        }

        //����ִ� Index �޾ƿ���
        //�ϴ� ���⼭ �����Ҵ��� �ް� ���߿� Ǯ���� ����
        IntType new_id{ -1 };
        for (IntType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
            if (sector_->players_[i]->isUsed_ == false) {
                new_id = i;
                break;
            }
        }
        //���� ���� Ȯ��
        if (new_id == -1) {
            CLogCollector::GetInstance()->PrintLog("MAX User");
        }
        else {
            sector_->players_[new_id]->isUsed_ = true;
            sector_->players_[new_id]->socket_ = clientSocket;

            std::cout << "Accpet New Clinet[" << new_id << "]\n";

            //�� Ŭ���̾�Ʈ ������ �ٸ� Ŭ���̾�Ʈ���� �˸�
            for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {

                //���� ���� �� ������� Ŭ���̾�Ʈ��
                if (i == new_id || sector_->players_[i]->isUsed_==false)continue;

                //socket, x, y, id, type
                NETWORK::SendAddObject(
                    sector_->players_[i]->socket_,
                    sector_->players_[new_id]->x_, sector_->players_[new_id]->y_,
                    new_id, OBJECT_DEFINDS::TYPE::PLAYER);


            }

            //�� Ŭ���̾�Ʈ���� ������ ������ �������� ������ ����
            for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {

                //���� ���� �� ������� Ŭ���̾�Ʈ��
                if (i == new_id || sector_->players_[i]->isUsed_ == false)continue;

                //socket, x, y, id, objtype
                NETWORK::SendAddObject(
                    sector_->players_[new_id]->socket_,
                    sector_->players_[i]->x_, sector_->players_[i]->y_,
                    i, OBJECT_DEFINDS::TYPE::PLAYER);
            }


            // IOCP ��ü�� ���� ���� (socket, hIOCP, key, 0)
            CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), iocp_, new_id, 0);

            //Recv ����
            NETWORK::Recv(sector_->players_[new_id]->socket_, sector_->players_[new_id]->overEx_);
        }
    }

    // ���� ��������
    closesocket(listenSocket);

    // Winsock End
    WSACleanup();
}

void CServer::TimerThread() {}

void CServer::ProcessPacket(int id, char* packet) {

    switch (packet[1]){
    case CS_MOVE: {
        cs_packet_move* move = reinterpret_cast<cs_packet_move*>(packet);
        break;
    }
    case CS_LOGIN: {
        break;
    }
    default:
        break;
    }


}

