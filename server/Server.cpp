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

    // Worker Thread ����
    Threads workerThread;
    for (int i = 0; i < NUMBER_OF_THREAD; ++i)
        workerThread.emplace_back(Thread{ &CServer::WorkThread,this });

    CLogCollector::GetInstance()->PrintLog("Start Worker Thread");

    //Init Socket
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        CLogCollector::GetInstance()->PrintLog("Can not load 'winsock.dll' file");
    }

    // 1. ���ϻ���  
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

    // 2. ���ϼ���
    if (bind(listenSocket_, reinterpret_cast<SOCKADDR*>(&ServerAddr), sizeof(ServerAddr)) == SOCKET_ERROR) {
        CLogCollector::GetInstance()->PrintLog("Fail bind", WSAGetLastError());
        // 6. ��������
        closesocket(listenSocket_);
        // Winsock End
        WSACleanup();
        return;
    }

    // 3. ���Ŵ�⿭���� , ListenSocket ,����
    if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR) {
        CLogCollector::GetInstance()->PrintLog("Fail listen", WSAGetLastError());
        // 6. ��������
        closesocket(listenSocket_);
        // Winsock End
        WSACleanup();
        return;
    }

    // listne()���� accept ���� ���ϰ��Ѵ�.
    BOOL on = true;
    setsockopt(listenSocket_, SOL_SOCKET, SO_CONDITIONAL_ACCEPT,
        reinterpret_cast<char*>(&on), sizeof(on));

    //Client Information
    SOCKET clientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    OverEx over_ex;
    over_ex.ev_ = EV_ACCEPT;
    over_ex.socket_ = clientSocket;

    //listenSocket ���
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

#ifdef  _DEBUG
        std::cout<<"Thread Is sleep: "<< std::this_thread::get_id() << "\n";
#endif //  _DEBUG

        int is_error = GetQueuedCompletionStatus(
            iocp_, &io_byte, &l_key, reinterpret_cast<LPWSAOVERLAPPED*>(&over_ex), INFINITE);
#ifdef  _DEBUG
        std::cout << "Thread Is WakeUp:  " << std::this_thread::get_id() << "\n";
#endif //  _DEBUG

        //IOCP �ڵ带 ������� ����
        if (is_error == 0) {
            CLogCollector::GetInstance()->PrintLog("GQCS Return is False");
            if (over_ex == nullptr) {
                //GQCS ����
            }
            else {
                //SOCKET ����
            }
        }

        switch (over_ex->ev_) {
        case EV_ACCEPT: {

            ObjectIDType new_id{ OBJECT_DEFINDS::MAX_USER};
            for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                if (sector_->players_[i]->isUsed_ == false) {
                    new_id = i;
                    break;
                }
            }
            //���� ���� Ȯ��
            if (new_id == OBJECT_DEFINDS::MAX_USER) {
                CLogCollector::GetInstance()->PrintLog("MAX User");
            }
            else {
                //Sector�� ���
                sector_->AddObject(new_id, OBJECT_DEFINDS::OTHER_PLAYER,PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y);
                sector_->players_[new_id]->isUsed_ = true;
                sector_->players_[new_id]->socket_ = over_ex->socket_;

                // IOCP ��ü�� ���� ���� (socket, hIOCP, key, 0)
                CreateIoCompletionPort(reinterpret_cast<HANDLE>(sector_->players_[new_id]->socket_), iocp_, new_id, 0);

                std::cout << "Accpet New Clinet[" << new_id << "]\n";
                //Login Ok Packet ����
                NETWORK::SendLoginOk(sector_->players_[new_id]->socket_,
                    sector_->players_[new_id]->x_, sector_->players_[new_id]->y_, new_id);


                //�� Ŭ���̾�Ʈ ������ �ٸ� Ŭ���̾�Ʈ���� �˸�
                for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                    //���� ���� �� ������� Ŭ���̾�Ʈ��
                    if (i == new_id || sector_->players_[i]->isUsed_ == false)continue;
                    CLogCollector::GetInstance()->PrintLog("Send Old Player\n");

                    sector_->players_[i]->srwLock_.Writelock();
                    sector_->players_[i]->viewLIst_.insert(new_id);
                    sector_->players_[i]->srwLock_.Writeunlock();


                    //socket, x, y, new_id, type
                    NETWORK::SendAddObject(
                        sector_->players_[i]->socket_,
                        sector_->players_[new_id]->x_, sector_->players_[new_id]->y_,
                        new_id, OBJECT_DEFINDS::TYPE::OTHER_PLAYER);
                }

                //�� Ŭ���̾�Ʈ���� ������ ������ �������� ������ ����
                for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                    //���� ���� �� ������� Ŭ���̾�Ʈ��
                    if (i == new_id || sector_->players_[i]->isUsed_ == false)continue;

                    sector_->players_[new_id]->srwLock_.Writelock();
                    sector_->players_[new_id]->viewLIst_.insert(i);
                    sector_->players_[new_id]->srwLock_.Writeunlock();

                    //socket, x, y, ����id, objtype
                    NETWORK::SendAddObject(
                        sector_->players_[new_id]->socket_,
                        sector_->players_[i]->x_, sector_->players_[i]->y_,
                        i, OBJECT_DEFINDS::TYPE::OTHER_PLAYER);
                }

                //���� ���� ����
                //20.11.19 timer Thread �� �ذ��ϸ� index ������OBJECT_DEFINDS::MAX_USER~MAX_OBJECT�� ����
                for (ObjectIDType i = 0; i < 1; ++i) {

                    //�÷��̾�� ������ �ִ� ���� �����
                    //2020.11.19 �׻� ���ʹ� User��ŭ ������
                    if (sector_->WakeUpNearMonster(i+OBJECT_DEFINDS::MAX_USER, new_id) == false)continue;

                    sector_->players_[new_id]->srwLock_.Writelock();
                    //Player�� Monster�� ID�� �����ϱ� ���� ����
                    sector_->players_[new_id]->viewLIst_.insert(i + OBJECT_DEFINDS::MAX_USER);
                    sector_->players_[new_id]->srwLock_.Writeunlock();

                    //������ �ִ� ���� ���� ����
                    //2020.11.19 �׻� ���ʹ� User��ŭ ������
                    NETWORK::SendAddObject(
                        sector_->players_[new_id]->socket_,
                        PRIMARY_MONSTER_X, PRIMARY_MONSTER_Y,
                        i + OBJECT_DEFINDS::MAX_USER, OBJECT_DEFINDS::TYPE::MONSTER);

                    //TimerQueue�� Event �߰�
                    //20.11.16 WAKEUP���� �ٲٴ��� �ؾ��ҵ�
                    CTimerQueueHandle::GetInstance()->queue_.Emplace(
                        EVENT_ST{ i,new_id,EVENT_TYPE::EV_MONSTER_MOVE,high_resolution_clock::now() + 1s });
                }

                ZeroMemory(&over_ex->over_, sizeof(over_ex->over_));
                SOCKET socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
                over_ex->ev_ = EV_ACCEPT;
                over_ex->socket_ = socket;

                //�ٽ� Accpet overlapped ȣ��
                AcceptEx(listenSocket_, socket, &over_ex->messageBuffer_, 0,
                    sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL,
                    &over_ex->over_);

                //Recv ����
                NETWORK::Recv(sector_->players_[new_id]->socket_, sector_->players_[new_id]->overEx_);
            }
            break;
        }
        case EV_MONSTER_MOVE: {
            EVENT_ST ev;
            ev.type = over_ex->ev_;
            ev.target_id = over_ex->target_player_;
            ev.obj_id = l_key;
            sector_->ProcessEvent(ev);

            //���� ����, ���� id, ���� x,y ,������ Ÿ��(����), ������ �ؽ��� ����
            for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                //������� Ŭ���̾�Ʈ��
                if (sector_->players_[i]->isUsed_ == false)continue;

                //���� ID ����
                NETWORK::SendMoveObject(sector_->players_[i]->socket_, sector_->monsters_[ev.obj_id]->x_,
                    sector_->monsters_[ev.obj_id]->y_, ev.obj_id + OBJECT_DEFINDS::MAX_USER,
                    OBJECT_DEFINDS::MONSTER, sector_->monsters_[ev.obj_id]->diretion_);
            }

            

            //�þ߿� �ִٸ� �ٽ� �̵�(����, �÷��̾�)
            //���� ID ����
            if (sector_->IsNearMonsterAndPlayer(ev.obj_id + OBJECT_DEFINDS::MAX_USER, ev.target_id) == true) {

                /*
                �þ� �ȿ� �ִٸ� ������ Timer�� ������ ������ ���������� ����
                1�ʸ��� ���͸� �̵����Ѿ� �ϱ� ������ TImerQueue�� Enq
                */

                //TimerQueue�� Event �߰�
                CTimerQueueHandle::GetInstance()->queue_.Emplace(
                    EVENT_ST{ ev.obj_id,ev.target_id,EVENT_TYPE::EV_MONSTER_MOVE,high_resolution_clock::now() + 1s });

            }

                //���� targetId�� �� ª���Ÿ��� �ִٸ� �� Ÿ������ ����
                if (sector_->TestFunction(ev.obj_id, ev.target_id) == true) {

                }
                else {

                }


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

        //�̵��� �þ߿� Monster�� ������ �����.

        break;
    }
    case CS_LOGIN: {
        break;
    }
    default:
        break;
    }
}

