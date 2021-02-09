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
    over_ex.ioEvent_ = IO_EVENT::IO_ACCEPT;
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
        int is_error = GetQueuedCompletionStatus(
            iocp_, &io_byte, &l_key, reinterpret_cast<LPWSAOVERLAPPED*>(&over_ex), INFINITE);

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

        switch (over_ex->ioEvent_) {
        case IO_EVENT::IO_ACCEPT: {

            ObjectIDType new_id{ OBJECT_DEFINDS::MAX_USER };
            for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                if (sector_->gameobjects_[i]->GetUsed() == false) {
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
                sector_->AddObject(new_id, PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y);
                sector_->gameobjects_[new_id]->SetUsed(true);
                sector_->gameobjects_[new_id]->SetSocket(over_ex->socket_);

                // IOCP ��ü�� ���� ���� (socket, hIOCP, key, 0)
                CreateIoCompletionPort(reinterpret_cast<HANDLE>(sector_->gameobjects_[new_id]->GetSocket()), iocp_, new_id, 0);

                std::cout << "Accpet New Clinet[" << new_id << "]\n";
                //Login Ok Packet ����
                NETWORK::SendLoginOk(sector_->gameobjects_[new_id]->GetSocket(),
                    sector_->gameobjects_[new_id]->GetPositionX(), sector_->gameobjects_[new_id]->GetPositionY(), new_id);


                //�� Ŭ���̾�Ʈ ������ �ٸ� Ŭ���̾�Ʈ���� �˸�
                for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                    //���� ���� �� ������� Ŭ���̾�Ʈ��
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

                //�� Ŭ���̾�Ʈ���� ������ ������ �������� ������ ����
                for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                    //���� ���� �� ������� Ŭ���̾�Ʈ��
                    if (i == new_id || sector_->gameobjects_[i]->GetUsed() == false)continue;

                    sector_->gameobjects_[new_id]->srwLock_.Writelock();
                    sector_->gameobjects_[new_id]->GetViewList().insert(i);
                    sector_->gameobjects_[new_id]->srwLock_.Writeunlock();

                    //socket, x, y, ����id, objtype
                    NETWORK::SendAddObject(
                        sector_->gameobjects_[new_id]->GetSocket(),
                        sector_->gameobjects_[i]->GetPositionX(), sector_->gameobjects_[i]->GetPositionY(),
                        i);
                }

                //���� ���� ����
                for (ObjectIDType i = OBJECT_DEFINDS::MAX_USER; i < OBJECT_DEFINDS::MAX_USER + 1; ++i) {

                    //�÷��̾�� ������ �ִ� ���� �����
                    //2020.11.19 �׻� ���ʹ� User��ŭ ������
                    if (sector_->WakeUpNearMonster(i, new_id) == false)continue;


                    sector_->gameobjects_[new_id]->srwLock_.Writelock();
                    //Player�� Monster�� ID�� �����ϱ� ���� ����
                    sector_->gameobjects_[new_id]->GetViewList().insert(i);
                    sector_->gameobjects_[new_id]->srwLock_.Writeunlock();

                    //������ �ִ� ���� ���� ����
                    NETWORK::SendAddObject(sector_->gameobjects_[new_id]->GetSocket(), PRIMARY_MONSTER_X, PRIMARY_MONSTER_Y, i);

                    //TimerQueue�� Event �߰�
                    //20.11.16 WAKEUP���� �ٲٴ��� �ؾ��ҵ�
                    CTimerQueueHandle::GetInstance()->queue_.Emplace(
                        EVENT_ST{ i,new_id,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
                }

                ZeroMemory(&over_ex->over_, sizeof(over_ex->over_));
                SOCKET socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
                over_ex->ioEvent_ = IO_EVENT::IO_ACCEPT;
                over_ex->socket_ = socket;

                //�ٽ� Accpet overlapped ȣ��
                AcceptEx(listenSocket_, socket, &over_ex->messageBuffer_, 0,
                    sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL,
                    &over_ex->over_);

                //Recv ����
                NETWORK::Recv(sector_->gameobjects_[new_id]->GetSocket(), sector_->gameobjects_[new_id]->GetOverEx());
            }
            break;
        }
        case IO_EVENT::IO_MONSTER_EVENT: {
            EVENT_ST ev = over_ex->ev;
            std::cout << "Obj ID" << (int)ev.obj_id << " Target Id" <<(int) ev.target_id << "\n";

            //Monster Event ����(�̵�, ����, ��Ÿ...)
            sector_->ProcessEvent(ev);
            //Process ���� �� ���� ���� ���¿� ���� �ൿ �޾ƿ�
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

            //��Ŷ ������
            int rest = io_byte;
            char* ptr = over_ex->messageBuffer_;
            char packet_size = 0;

            if (0 < sector_->gameobjects_[l_key]->GetPrevSize())
                packet_size = sector_->gameobjects_[l_key]->GetPacketBuffer()[0];

            // ��Ŷ ����
            while (0 < rest) {
                // ���� ����� ���ٸ� ��Ŷ ����� ptr�� �� ó������ �� �� �ִ�.
                if (0 == packet_size) packet_size = ptr[0];
                int required = packet_size - sector_->gameobjects_[l_key]->GetPrevSize();
                if (required <= rest) {
                    // �׳� ��Ŷ ���۸� ���� ��� ��� ������ �ֱ� ������
                    memcpy(sector_->gameobjects_[l_key]->GetPacketBuffer() +
                        sector_->gameobjects_[l_key]->GetPrevSize(), ptr, required);

                    // ��Ŷ ���� ������ ó��
                    ProcessPacket(l_key, sector_->gameobjects_[l_key]->GetPacketBuffer());

                    rest -= required;
                    ptr += required;

                    packet_size = 0;
                    sector_->gameobjects_[l_key]->SetPrevSize(0);
                }
                else {
                    //���ڶ� ���
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
