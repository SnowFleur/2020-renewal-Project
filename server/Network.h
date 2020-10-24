#pragma once
#include<iostream>
#include"OverEx.h"
#include"LogCollector.h"

/*
Send Recv�� ������ �ʹ� ������ ����� ����

Send �� Recv�� Thrad Safe �ϴ� Pointer�� �̿��ؼ� ó������.
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
        
        //���߿� �޸� Ǯ�� ����
        OverEx* ov = new OverEx;
        ov->ev_ = EV_SEND;
        ov->dataBuffer_.len = p[0];
        // ���� �����ͷ� ä���
        memcpy(ov->messageBuffer_, p, p[0]);

        int err_no = WSASend(socket, &ov->dataBuffer_, 1, NULL, 0, &ov->over_, NULL);
        if (0 != err_no) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                CLogCollector::GetInstance()->PrintLog("Send Fail", WSAGetLastError());
            }
        }
    }
#pragma endregion


    //���ο� Object(����, ���� ��)�� ���� �� ������ ��Ŷ
    void SendAddObject(SOCKET& socket, 
        const PositionType x, const PositionType y,const 
        ObjectIDType objId,const ObjectType objType) {
       
        sc_packet_add_object packet;
        packet.size = sizeof(packet);
        packet.type = SC_ADD_OBJECT;
        packet.id = objId;
        packet.objectClass = objType;
        packet.x = x;
        packet.y = y;
        SendPacket(socket, &packet);
    }

}