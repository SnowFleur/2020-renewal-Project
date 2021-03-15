#pragma once
#include"Network.h"
#include"Protocol.h"
#include<iostream>

/*
Send Recv�� ������ �ʹ� ������ ����� ����
Send �� Recv�� Thrad Safe �ϴ� Pointer�� �̿��ؼ� ó������.
https://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid
https://cboard.cprogramming.com/c-programming/150774-parallel-threads-socket-send-recv.html
*/
namespace NETWORK {

#pragma region Standard Recv/Send Function
    //WSARecv
    void Recv(SOCKET socket, OverEx& overEx) {
        DWORD flag = 0;
        overEx.ioEvent_ = IO_EVENT::IO_RECV;

        int err_no = WSARecv(socket, &overEx.dataBuffer_, 1, NULL, &flag, &(overEx.over_), 0);
        if (err_no != 0) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                CLogCollector::GetInstance()->PrintLog("Recv Fail", WSAGetLastError());
                while (true);
            }
        }
    }

    //WSASend
    void SendPacket(SOCKET socket, void* packet) {
        char* p = reinterpret_cast<char*> (packet);

        //Send Memory Pool���� �Ҵ�
        OverEx* ov = nullptr;
        ov = reinterpret_cast<OverEx*>(uPtrSendPoolHandle->Allocate(sizeof(OverEx)));

        if (ov == nullptr) {
            CLogCollector::GetInstance()->PrintLog("Error Send Packet OverEx Is nullptr");
        }

        //OverEX �ʱ�ȭ
        ov->InitOverEx();
        ov->ioEvent_ = IO_EVENT::IO_SEND;
        ov->dataBuffer_.len = p[0];
        // ���� �����ͷ� ä���
        memcpy(ov->messageBuffer_, p, p[0]);

        int err_no = WSASend(socket, &ov->dataBuffer_, 1, 0, 0, &ov->over_, NULL);
        if (0 != err_no) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                CLogCollector::GetInstance()->PrintLog("Send Fail", WSAGetLastError());
                DeallocateMemory(ov);
            }
        }
    }

    void DeallocateMemory(void* memoryPtr) {
        uPtrSendPoolHandle->Deallocate(memoryPtr);
    }

#pragma endregion

    //Ŭ���̾�Ʈ�� ���� ������ �˸�
    void SendLoginOk(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType loginid) {

        sc_packet_login_ok packet;
        packet.size = sizeof(packet);
        packet.type = SC_LOGIN_OK;
        packet.id = loginid;
        packet.x = x;
        packet.y = y;
        SendPacket(socket, &packet);

    }
    //���ο� Object(����, ���� ��)�� ���� �� ������ ��Ŷ
    void SendAddObject(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType addID) {

        sc_packet_add_object packet;
        packet.size = sizeof(packet);
        packet.type = SC_ADD_OBJECT;
        packet.addID = addID;
        packet.x = x;
        packet.y = y;
        SendPacket(socket, &packet);
    }
    // Object�� Ŭ���̾�Ʈ���� �������� �����ϰ��� �� �� ������ ��Ŷ
    void SendRemoveObject(SOCKET socket, const ObjectIDType removeID) {

        sc_packet_remove_object packet;
        packet.size = sizeof(packet);
        packet.type = SC_REMOVE_OBJECT;
        packet.removeID = removeID;
        SendPacket(socket, &packet);
    }
    //Object(����, ���� ��)�� ������ �� ������ ��Ŷ
    void SendMoveObject(SOCKET socket, const PositionType x, const PositionType y,
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
    //Object(����, ���� ��)�� ������ �޾��� �� ������ ��Ŷ
    void SendHitObject(SOCKET socket, const HpType hp, const ObjectIDType hitID, const TextureDirection attackDirection,
        const ObjectIDType attackID) {

        sc_packet_hit_object packet;
        packet.size = sizeof(packet);
        packet.type = SC_HIT_OBJECT;
        packet.hp = hp;
        packet.hitID = hitID;
        packet.attackDirection = attackDirection;
        packet.attackID = attackID;
        SendPacket(socket, &packet);
    }

};