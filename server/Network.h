#pragma once
#include"OverEx.h"
#include"LogCollector.h"
#include"DataType.h"
/*
Send Recv는 관련이 너무 많으니 여기로 빼자

Send 와 Recv는 Thrad Safe 하니 Pointer를 이용해서 처리하자.
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

    //클라이언트의 접속 성공을 알림
    void SendLoginOk(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType addID);

    //새로운 Object(몬스터, 유저 등)이 생길 때 보내는 패킷
    void SendAddObject(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType addID, const ObjectClass objType);

    //Object(몬스터, 유저 등)이 움직일 때 보내는 패킷
    void SendMoveObject(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType movedID, const TextureDirection textureDirection);
}