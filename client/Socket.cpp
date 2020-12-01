#include"Socket.h"

Buffer::Buffer() :recvBuffer{}, sendBuffer{}, packetBuffer{},prevSize{ 0 } {
    recvWsabuf.buf = recvBuffer;
    recvWsabuf.len = BUF_SIZE;
    sendWsabuf.buf = sendBuffer;
    sendWsabuf.len = BUF_SIZE;
}

CSocket::CSocket(SOCKET_TYPE socketType, const char* endPoint,
    const unsigned short serverPort, HWND& hwnd)
    :hWnd_(hwnd) {

    //Socket init
    InitSocket(socketType, false);

    //Connect
    SOCKADDR_IN ServerAddr;
    ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, endPoint, &ServerAddr.sin_addr);

    if (WSAConnect(socket_, (sockaddr*)&ServerAddr, sizeof(ServerAddr),NULL,NULL,NULL,NULL) == SOCKET_ERROR) {
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
            std::cout << "Error Connect: " << WSAGetLastError() << "\n";
            closesocket(socket_);
        }
    }

    //WSAAsyncSelect
    WSAAsyncSelect(socket_, hWnd_, WM_SOCKET, FD_READ | FD_CLOSE);
}

void CSocket::InitSocket(SOCKET_TYPE socketType, bool isOverlapped) {

    WSADATA	wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    if (socketType == SOCKET_TYPE::TCP) {
        if (isOverlapped == true)
            socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

        else
            socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, NULL);
    }
    else if (socketType == SOCKET_TYPE::UDP) {
        if (isOverlapped == true)
            socket_ = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
        else
            socket_ = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, NULL);
    }
}

void CSocket::ReadPacket() {

    DWORD ioByte{}, ioFlag{};
    DWORD packet_size{};
    char* p = reinterpret_cast<char*>(buffer_.recvBuffer);

    int err_no = WSARecv(socket_, &buffer_.recvWsabuf, 1, &ioByte, &ioFlag, NULL, NULL);
    if (err_no!=0) {
        std::cout << "Recv Error: " << WSAGetLastError() << "\n";
    }
    while (ioByte > 0) {
        //전에 받은 패킷이 있다면 
        if (0 < buffer_.prevSize)
            packet_size = buffer_.prevSize;

        //이전에 받은게 없다면 패킷사이즈를 받는다
        if (packet_size == 0)
            packet_size = p[0];

        DWORD required = packet_size - buffer_.prevSize;

        if (required <= ioByte) {
            // 그냥 패킷 버퍼를 담을 경우 덮어쓸 위험이 있기 때문에
            memcpy(buffer_.packetBuffer + buffer_.prevSize, p, required);

            //memcpy_s(buffer_.recvBuffer + buffer_.prevSize, BUF_SIZE, buffer_.recvBuffer, required);

            // 패킷 내의 데이터 처리
            ProcessPacket(buffer_.packetBuffer);

            ioByte -= required;
            p += required;

            packet_size = 0;
            buffer_.prevSize = 0;
        }
        else {
            //부족한 경우
            memcpy(buffer_.packetBuffer + buffer_.prevSize, p, ioByte);
            //memcpy_s(buffer_.recvBuffer + buffer_.prevSize, BUF_SIZE, buffer_.recvBuffer, ioByte);

            ioByte = 0;
            buffer_.prevSize += ioByte;
        }
    }
}

void CSocket::CloseSocket() {
    closesocket(socket_);
    WSACleanup();
}

/*
일단 전역함수를 불러서 해결하는 방법으로 했다. 방법을 모르겠음;;;;
이거 하나를 붙잡고 있기엔 가야할길이 너무 많음
*/
void CSocket::ProcessPacket(char* packetHandle) {
        ProcessPacketEvent(packetHandle);
}

#pragma region Send FunctionS

void CSocket::SendPlayerPosition(const PositionType x, const PositionType y,
                const TextureDirection textureDirection) {

    cs_packet_move_object packet;
    packet.size = sizeof(packet);
    packet.type = CS_MOVE_OBJECT;
    packet.x = x;
    packet.y = y;
    packet.textureDirection = textureDirection;
    SendPacket(&packet);
}

void CSocket::SendChatMeesage(const wchar_t* message) {

    int packet;

    SendPacket(&packet);
}

void CSocket::SendLogin(const wchar_t* id, size_t lenSize) {

    cs_packet_login packet;
    packet.size = sizeof(packet);
    packet.type = CS_LOGIN;
    packet.is_user = true;

    memcpy_s(packet.player_id, MAX_ID_SIZE, id, lenSize);
    SendPacket(&packet);
}

void CSocket::SendPlayerAction(PLAYER_ACTION action) {

    cs_packet_player_action packet;
    packet.size = sizeof(packet);
    packet.type = CS_ACTION;

    switch (action) {
    case PLAYER_ACTION::PA_ATTACK: {
        packet.action_type = PLAYER_ACTION::PA_ATTACK;
        break;
    }
    case PLAYER_ACTION::PA_REQUEST_QUEST: {
        packet.action_type = PLAYER_ACTION::PA_REQUEST_QUEST;
        break;
    }
    case PLAYER_ACTION::PA_REQUEST_HEAL: {
        packet.action_type = PLAYER_ACTION::PA_REQUEST_HEAL;
        break;
    }
    case PLAYER_ACTION::PA_REQUEST_SHOP: {
        packet.action_type = PLAYER_ACTION::PA_REQUEST_SHOP;
        break;
    }
    case PLAYER_ACTION::PA_REQUEST_QUEST_CELAR: {
        packet.action_type = PLAYER_ACTION::PA_REQUEST_QUEST_CELAR;
        break;
    }
    default: {
        //std::cout << "not defined event" << std::endl;
        break;
    }
    }
    SendPacket(&packet);
}

void CSocket::SendPacket(void* packet) {

    char* p = reinterpret_cast<char*> (packet);
    DWORD iobyte{};

    buffer_.sendWsabuf.len = p[0];

    // 보낼 데이터로 채우기
    memcpy_s(buffer_.sendBuffer, BUF_SIZE, p, p[0]);

    int errorCode = WSASend(socket_, &buffer_.sendWsabuf, 1, &iobyte, 0, NULL, NULL);
    if (0 != errorCode) {
        if (WSAGetLastError() != WSA_IO_PENDING) {
            //CLog::PrintError("WSASend", WSAGetLastError());
            //closesocket(socket_);
        }
    }
}

#pragma endregion
