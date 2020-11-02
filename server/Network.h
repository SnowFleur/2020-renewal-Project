#pragma once
#include"OverEx.h"
#include"LogCollector.h"
#include"DataType.h"
/*
Send Recv�� ������ �ʹ� ������ ����� ����

Send �� Recv�� Thrad Safe �ϴ� Pointer�� �̿��ؼ� ó������.
https://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid
https://cboard.cprogramming.com/c-programming/150774-parallel-threads-socket-send-recv.html
*/
namespace NETWORK {

#pragma region Standard Recv/Send Function
    //WSARecv
    void Recv(SOCKET socket, OverEx& overEx);
    //WSASend
    void SendPacket(SOCKET socket, void* packet);
#pragma endregion

    //Ŭ���̾�Ʈ�� ���� ������ �˸�
    void SendLoginOk(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType addID);

    //���ο� Object(����, ���� ��)�� ���� �� ������ ��Ŷ
    void SendAddObject(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType addID, const ObjectClass objType);

    //Object(����, ���� ��)�� ������ �� ������ ��Ŷ
    void SendMoveObject(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType movedID, const TextureDirection textureDirection);
}