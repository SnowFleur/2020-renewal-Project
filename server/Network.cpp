#pragma once
#include"Network.h"
#include"Protocol.h"
#include<iostream>


/*
Send Recv는 관련이 너무 많으니 여기로 빼자

Send 와 Recv는 Thrad Safe 하니 Pointer를 이용해서 처리하자.
https://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid
https://cboard.cprogramming.com/c-programming/150774-parallel-threads-socket-send-recv.html
*/
namespace NETWORK {

#pragma region Standard Recv/Send Function
    //WSARecv
    void Recv(SOCKET& socket, OverEx& overEx) {
        DWORD flag = 0;

        overEx.ev_ = EV_RECV;

        int err_no = WSARecv(socket, &overEx.dataBuffer_, 1, NULL, &flag, &(overEx.over_), 0);
        if (err_no != 0) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                CLogCollector::GetInstance()->PrintLog("Recv Fail", WSAGetLastError());
                while (true);
            }
        }
    }

    //WSASend
    void SendPacket(SOCKET& socket, void* packet) {
        char* p = reinterpret_cast<char*> (packet);

        //나중에 메모리 풀로 변경
        OverEx* ov = new OverEx;
        ov->ev_ = EV_SEND;
        ov->dataBuffer_.len = p[0];
        // 보낼 데이터로 채우기
        memcpy(ov->messageBuffer_, p, p[0]);

        int err_no = WSASend(socket, &ov->dataBuffer_, 1, NULL, 0, &ov->over_, NULL);
        if (0 != err_no) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                CLogCollector::GetInstance()->PrintLog("Send Fail", WSAGetLastError());
            }
        }
    }
#pragma endregion


    //새로운 Object(몬스터, 유저 등)이 생길 때 보내는 패킷
    void SendAddObject(SOCKET& socket, const PositionType x, const PositionType y,
        const ObjectIDType addID, const ObjectClass objType) {

        sc_packet_add_object packet;
        packet.size = sizeof(packet);
        packet.type = SC_ADD_OBJECT;
        packet.addID = addID;
        packet.objectClass = objType;
        packet.x = x;
        packet.y = y;
        SendPacket(socket, &packet);
    }

    //Object(몬스터, 유저 등)이 움직일 때 보내는 패킷
    void SendMoveObject(SOCKET& socket, const PositionType x, const PositionType y,
        const ObjectIDType movedID, const TextureDirection textureDirection) {

        sc_packet_move_object packet;
        packet.size = sizeof(packet);
        packet.type = SC_MOVE_OBJECT;
        packet.x = x;
        packet.y = y;
        packet.movedID = movedID;
        packet.textureDirection = textureDirection;
        SendPacket(socket, &packet);

    }

}