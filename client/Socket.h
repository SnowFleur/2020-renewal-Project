#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")

#include <WinSock2.h>
#include <windowsx.h>
#include<ws2tcpip.h>
#include"Network.h"
extern void ProcessPacketEvent(char*);


#include<iostream>

/*
Send, Recv ���� ����ü
*/

struct Buffer {
    WSABUF              recvWsabuf;
    char                recvBuffer[BUF_SIZE]; //Ŀ�ο�
    WSABUF              sendWsabuf;
    char                sendBuffer[BUF_SIZE]; //Ŀ�ο�
    char                packetBuffer[BUF_SIZE]; //������ ��Ŷ ����
    int                 prevSize;

    //�ʱ�ȭ�� �ڱ��ڽ��� ����
    Buffer();
};

enum class SOCKET_TYPE {
    TCP, UDP
};

#pragma region CSOCKET
class CSocket : public INetwork {
private:
    SOCKET      socket_;
    char        clientBuffer_[BUF_SIZE];
    Buffer      buffer_;
    HWND        hWnd_;
    void        ProcessPacket(char* packet);
    void        InitSocket(SOCKET_TYPE socketType, bool isOverlapped);
    void        SendPacket(void* packet);
public:
    ~CSocket() = default;
    CSocket() = delete;
    CSocket(SOCKET_TYPE type, const char* endPoint, const unsigned short serverPort, HWND& hwnd);
    CSocket(const CSocket&) = delete;
    CSocket(CSocket&&) = delete;
    CSocket& operator=(const CSocket&) = delete;
    CSocket& operator=(const CSocket&&) = delete;

    void        ReadPacket()override;
    void        CloseSocket()override;
    void        SendPlayerPosition(const PositionType x,const PositionType y,
                const TextureDirection textureDirection)override;
    void        SendChatMeesage(const wchar_t* message)override;
    void        SendLogin(const wchar_t* id, size_t lenSize)override;
    void        SendPlayerAction(PLAYER_ACTION action)override;

};
#pragma endregion




