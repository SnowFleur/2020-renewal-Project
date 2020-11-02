
#include"Server.h"
#include"OverEx.h"            
#include"Protocol.h"
#include"Network.h"
#include"Sector.h"
#include"Player.h"                  //Player Class
#include"LogCollector.h"            //Log Class
#include"GameObject.h"

void CServer::Run() {
    //Init Sector
    sector_ = std::make_shared<CSector>();

    //Init IOCP
    iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    // Worker Thread 생성
    Threads workerThread;
    for (int i = 0; i < NUMBER_OF_THREAD; ++i)
        workerThread.emplace_back(Thread{ &CServer::WorkThread,this });

    //Timer
   // Thread timerThread{ Thread{&CServer::TimerThread,this} };

    //DB Thread


    //Init Socket
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        CLogCollector::GetInstance()->PrintLog("Can not load 'winsock.dll' file");
    }

    // 1. 소켓생성  
    listenSocket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listenSocket_ == INVALID_SOCKET) {
        CLogCollector::GetInstance()->PrintLog("Can not load 'winsock.dll' file");
    }

    //Bind
    SOCKADDR_IN ServerAddr;
    ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
    ServerAddr.sin_family = PF_INET;
    ServerAddr.sin_port = htons(SERVER_PORT);
    ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    // 2. 소켓설정
    if (bind(listenSocket_, reinterpret_cast<SOCKADDR*>(&ServerAddr), sizeof(ServerAddr)) == SOCKET_ERROR) {
        CLogCollector::GetInstance()->PrintLog("Fail bind", WSAGetLastError());
        // 6. 소켓종료
        closesocket(listenSocket_);
        // Winsock End
        WSACleanup();
        return;
    }

    // 3. 수신대기열생성 , ListenSocket ,갯수
    if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR) {
        CLogCollector::GetInstance()->PrintLog("Fail listen", WSAGetLastError());
        // 6. 소켓종료
        closesocket(listenSocket_);
        // Winsock End
        WSACleanup();
        return;
    }

    // listne()에서 accept 받지 못하게한다.
    BOOL on = true;
    setsockopt(listenSocket_, SOL_SOCKET, SO_CONDITIONAL_ACCEPT,
        reinterpret_cast<char*>(&on), sizeof(on));


    //Client Information
    SOCKET clientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    OverEx over_ex;
    over_ex.ev_ = EV_ACCEPT;
    over_ex.socket_ = clientSocket;

    //listenSocket 등록
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(listenSocket_), iocp_, NULL, NULL);

    if (AcceptEx(listenSocket_, clientSocket, &over_ex.messageBuffer_, NULL,
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL,
        &over_ex.over_) == FALSE) {
     
        if (WSAGetLastError() != WSA_IO_PENDING) {
            std::cout << "Error\n";
        }
    }

    //timerThread.join();
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
    OverEx* over_ex = nullptr;

    while (true) {

        int is_error = GetQueuedCompletionStatus(
            iocp_, &io_byte, &l_key, reinterpret_cast<LPWSAOVERLAPPED*>(&over_ex), INFINITE);

        //IOCP 핸드를 닫을경우 나옴
        if (is_error == 0) {
            CLogCollector::GetInstance()->PrintLog("GQCS Return is False");
            if (over_ex == nullptr) {
                //GQCS 오류
            }
            else {
                //SOCKET 오류
            }
        }

        switch (over_ex->ev_) {
        case EV_ACCEPT: {

            IntType new_id{ -1 };
            for (IntType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                if (sector_->players_[i]->isUsed_ == false) {
                    new_id = i;
                    break;
                }
            }
            //오류 여부 확인
            if (new_id == -1) {
                CLogCollector::GetInstance()->PrintLog("MAX User");
            }
            else {
                //Sector에 등록
                sector_->AddObject(new_id, PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y);
                sector_->players_[new_id]->isUsed_ = true;
                sector_->players_[new_id]->socket_ = over_ex->socket_;

                // IOCP 객체와 소켓 연결 (socket, hIOCP, key, 0)
                CreateIoCompletionPort(reinterpret_cast<HANDLE>(sector_->players_[new_id]->socket_), iocp_, new_id, 0);

                std::cout << "Accpet New Clinet[" << new_id << "]\n";
                //Login Ok Packet 전송
                NETWORK::SendLoginOk(sector_->players_[new_id]->socket_,
                    sector_->players_[new_id]->x_, sector_->players_[new_id]->y_, new_id);


                //새 클라이언트 접속을 다른 클라이언트에게 알림
                for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                    //본인 제외 및 사용중인 클라이언트만
                    if (i == new_id || sector_->players_[i]->isUsed_ == false)continue;

                    //socket, x, y, new_id, type
                    NETWORK::SendAddObject(
                        sector_->players_[i]->socket_,
                        sector_->players_[new_id]->x_, sector_->players_[new_id]->y_,
                        new_id, OBJECT_DEFINDS::TYPE::OTHER_PLAYER);
                }

                //새 클라이언트에게 기존에 접속한 유저들의 정보를 보냄
                for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                    //본인 제외 및 사용중인 클라이언트만
                    if (i == new_id || sector_->players_[i]->isUsed_ == false)continue;

                    //socket, x, y, 기존id, objtype
                    NETWORK::SendAddObject(
                        sector_->players_[new_id]->socket_,
                        sector_->players_[i]->x_, sector_->players_[i]->y_,
                        i, OBJECT_DEFINDS::TYPE::OTHER_PLAYER);
                }

                ZeroMemory(&over_ex->over_, sizeof(over_ex->over_));
                SOCKET socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
                over_ex->ev_ = EV_ACCEPT;
                over_ex->socket_ = socket;

                //다시 Accpet overlapped 호출
                AcceptEx(listenSocket_, socket, &over_ex->messageBuffer_, 0,
                    sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL,
                    &over_ex->over_);

                //Recv 시작
                NETWORK::Recv(sector_->players_[new_id]->socket_, sector_->players_[new_id]->overEx_);
            }
            break;
        }
        case EV_MOVE: {
            break;
        }
        case EV_SEND: {
            NETWORK::DeallocateMemory(reinterpret_cast<void*>(over_ex));
            over_ex = nullptr;
            break;
        }
        case EV_RECV: {

            if (io_byte == 0) {
                CLogCollector::GetInstance()->PrintLog("RECV IoByte Zero\n");
            }

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
            CLogCollector::GetInstance()->PrintLog("Not Defind EVENT");
            break;
        }

    } //end while
}



void CServer::TimerThread() {}

void CServer::ProcessPacket(int id, char* packet) {

    switch (packet[1]) {
    case CS_MOVE_OBJECT: {
        cs_packet_move_object* move = reinterpret_cast<cs_packet_move_object*>(packet);
        sector_->MoveObject(id, move->x, move->y, move->textureDirection);
        break;
    }
    case CS_LOGIN: {
        break;
    }
    default:
        break;
    }
}

