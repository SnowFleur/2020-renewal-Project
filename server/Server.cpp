#include"Server.h"
#include"OverEx.h"            
#include"Protocol.h"
#include"Network.h"
#include"Sector.h"
#include"Player.h"                  //Player Class
#include"LogCollector.h"            //Log Class
#include"GameObject.h"
#include"Monster.h"
#include"TimerQueue.h"

void CServer::Run() {
    //Init Sector
    sector_ = std::make_shared<CSector>();

    //Init IOCP
    iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    // Worker Thread 생성
    Threads workerThread;
    for (int i = 0; i < NUMBER_OF_THREAD; ++i)
        workerThread.emplace_back(Thread{ &CServer::WorkThread,this });

    CLogCollector::GetInstance()->PrintLog("Start Worker Thread");

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
    over_ex.ioEvent_ = IO_EVENT::IO_ACCEPT;
    over_ex.socket_ = clientSocket;

    //listenSocket 등록
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(listenSocket_), iocp_, NULL, NULL);

    //AcceptEx
    if (AcceptEx(listenSocket_, clientSocket, &over_ex.messageBuffer_, NULL,
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL,
        &over_ex.over_) == FALSE) {

        if (WSAGetLastError() != WSA_IO_PENDING) {
            std::cout << "Error\n";
        }
    }


    //Run Timer Thread
    timerThread_.RunTimerThread(iocp_);

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

        switch (over_ex->ioEvent_) {
        case IO_EVENT::IO_ACCEPT: {

            ObjectIDType new_id{ OBJECT_DEFINDS::MAX_USER };
            for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                if (sector_->gameobjects_[i]->GetUsed() == false) {
                    new_id = i;
                    break;
                }
            }
            //오류 여부 확인
            if (new_id == OBJECT_DEFINDS::MAX_USER) {
                CLogCollector::GetInstance()->PrintLog("MAX User");
            }
            else {
                //Sector에 등록
                sector_->AddObject(new_id, PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y);
                sector_->gameobjects_[new_id]->SetUsed(true);
                sector_->gameobjects_[new_id]->SetSocket(over_ex->socket_);

                // IOCP 객체와 소켓 연결 (socket, hIOCP, key, 0)
                CreateIoCompletionPort(reinterpret_cast<HANDLE>(sector_->gameobjects_[new_id]->GetSocket()), iocp_, new_id, 0);

                std::cout << "Accpet New Clinet[" << new_id << "]\n";
                //Login Ok Packet 전송
                NETWORK::SendLoginOk(sector_->gameobjects_[new_id]->GetSocket(),
                    sector_->gameobjects_[new_id]->GetPositionX(), sector_->gameobjects_[new_id]->GetPositionY(), new_id);


                //새 클라이언트 접속을 다른 클라이언트에게 알림
                for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                    //본인 제외 및 사용중인 클라이언트만
                    if (i == new_id || sector_->gameobjects_[i]->GetUsed() == false)continue;
                    CLogCollector::GetInstance()->PrintLog("Send Old Player\n");

                    sector_->gameobjects_[i]->srwLock_.Writelock();
                    sector_->gameobjects_[i]->GetViewList().insert(new_id);
                    sector_->gameobjects_[i]->srwLock_.Writeunlock();

                    //socket, x, y, new_id
                    NETWORK::SendAddObject(
                        sector_->gameobjects_[i]->GetSocket(),
                        sector_->gameobjects_[new_id]->GetPositionX(), sector_->gameobjects_[new_id]->GetPositionY(),
                        new_id);
                }

                //새 클라이언트에게 기존에 접속한 유저들의 정보를 보냄
                for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                    //본인 제외 및 사용중인 클라이언트만
                    if (i == new_id || sector_->gameobjects_[i]->GetUsed() == false)continue;

                    sector_->gameobjects_[new_id]->srwLock_.Writelock();
                    sector_->gameobjects_[new_id]->GetViewList().insert(i);
                    sector_->gameobjects_[new_id]->srwLock_.Writeunlock();

                    //socket, x, y, 기존id, objtype
                    NETWORK::SendAddObject(
                        sector_->gameobjects_[new_id]->GetSocket(),
                        sector_->gameobjects_[i]->GetPositionX(), sector_->gameobjects_[i]->GetPositionY(),
                        i);
                }

                //몬스터 정보 전송
                for (ObjectIDType i = OBJECT_DEFINDS::MAX_USER; i < OBJECT_DEFINDS::MAX_USER + 1; ++i) {

                    //플레이어와 가까이 있는 몬스터 깨우기
                    //2020.11.19 항상 몬스터는 User만큼 더하자
                    if (sector_->WakeUpNearMonster(i, new_id) == false)continue;


                    sector_->gameobjects_[new_id]->srwLock_.Writelock();
                    //Player와 Monster의 ID를 구별하기 위한 덧셈
                    sector_->gameobjects_[new_id]->GetViewList().insert(i);
                    sector_->gameobjects_[new_id]->srwLock_.Writeunlock();

                    //가까이 있는 몬스터 정보 전송
                    NETWORK::SendAddObject(sector_->gameobjects_[new_id]->GetSocket(), PRIMARY_MONSTER_X, PRIMARY_MONSTER_Y, i);

                    //TimerQueue에 Event 추가
                    //20.11.16 WAKEUP으로 바꾸던지 해야할듯
                    CTimerQueueHandle::GetInstance()->queue_.Emplace(
                        EVENT_ST{ i,new_id,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
                }

                ZeroMemory(&over_ex->over_, sizeof(over_ex->over_));
                SOCKET socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
                over_ex->ioEvent_ = IO_EVENT::IO_ACCEPT;
                over_ex->socket_ = socket;

                //다시 Accpet overlapped 호출
                AcceptEx(listenSocket_, socket, &over_ex->messageBuffer_, 0,
                    sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL,
                    &over_ex->over_);

                //Recv 시작
                NETWORK::Recv(sector_->gameobjects_[new_id]->GetSocket(), sector_->gameobjects_[new_id]->GetOverEx());
            }
            break;
        }
        case IO_EVENT::IO_MONSTER_EVENT: {
            EVENT_ST ev = over_ex->ev;
            std::cout << "Obj ID" << (int)ev.obj_id << " Target Id" <<(int) ev.target_id << "\n";

            //Monster Event 실행(이동, 공격, 기타...)
            sector_->ProcessEvent(ev);
            //Process 실행 후 현재 몬스터 상태에 따른 행동 받아옴
            // SendMonsterPacket(monsterState, ev);
            break;
        }
        case IO_EVENT::IO_SEND: {
            NETWORK::DeallocateMemory(reinterpret_cast<void*>(over_ex));
            over_ex = nullptr;
            break;
        }
        case IO_EVENT::IO_RECV: {

            if (io_byte == 0) {
                CLogCollector::GetInstance()->PrintLog("RECV IoByte Zero\n");
            }

            //패킷 재조립
            int rest = io_byte;
            char* ptr = over_ex->messageBuffer_;
            char packet_size = 0;

            if (0 < sector_->gameobjects_[l_key]->GetPrevSize())
                packet_size = sector_->gameobjects_[l_key]->GetPacketBuffer()[0];

            // 패킷 조립
            while (0 < rest) {
                // 이전 기록이 없다면 패킷 사이즈를 ptr의 맨 처음으로 알 수 있다.
                if (0 == packet_size) packet_size = ptr[0];
                int required = packet_size - sector_->gameobjects_[l_key]->GetPrevSize();
                if (required <= rest) {
                    // 그냥 패킷 버퍼를 담을 경우 덮어쓸 위험이 있기 때문에
                    memcpy(sector_->gameobjects_[l_key]->GetPacketBuffer() +
                        sector_->gameobjects_[l_key]->GetPrevSize(), ptr, required);

                    // 패킷 내의 데이터 처리
                    ProcessPacket(l_key, sector_->gameobjects_[l_key]->GetPacketBuffer());

                    rest -= required;
                    ptr += required;

                    packet_size = 0;
                    sector_->gameobjects_[l_key]->SetPrevSize(0);
                }
                else {
                    //모자랄 경우
                    memcpy(sector_->gameobjects_[l_key]->GetPacketBuffer() +
                        sector_->gameobjects_[l_key]->GetPrevSize(), ptr, rest);
                    rest = 0;
                    sector_->gameobjects_[l_key]->SetPrevSize(sector_->gameobjects_[l_key]->GetPrevSize() + rest);
                }
            }
            //Recv
            NETWORK::Recv(sector_->gameobjects_[l_key]->GetSocket(), sector_->gameobjects_[l_key]->GetOverEx());
            break;
        }
        default:
            CLogCollector::GetInstance()->PrintLog("Not Defind EVENT In WorkThread");
            break;
        }

    } //end while
}

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
