#pragma once
#include"OverEx.h"
#include"LogCollector.h"
#include"DataType.h"
#include"MemoryPool.h"
#include"FixedMemoryBlock.h"

constexpr int MAX_MEMORYPOOL_SIZE=10000;
using UPtrPool = std::unique_ptr<CMemoryPool<CFixedMemoryBlock>>;

/*
Send Recv�� ������ �ʹ� ������ ����� ����
Send �� Recv�� Thrad Safe �ϴ� Pointer�� �̿��ؼ� ó������.
https://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid
https://cboard.cprogramming.com/c-programming/150774-parallel-threads-socket-send-recv.html
*/
namespace NETWORK {
    static UPtrPool uPtrSendPoolHandle=
        std::make_unique<CMemoryPool<CFixedMemoryBlock>>(MAX_MEMORYPOOL_SIZE);

#pragma region Standard Recv/Send Function
    //WSARecv
    void Recv(SOCKET socket, OverEx& overEx);
    //WSASend
    void SendPacket(SOCKET socket, void* packet);
    //�޸� Ǯ ȸ��
    void DeallocateMemory(void* memoryPtr);
#pragma endregion

    //Ŭ���̾�Ʈ�� ���� ������ �˸�
    //���� ����, ������ ���� X, ������ ���� Y, ������ ���� ID
    void SendLoginOk(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType addID);

    //���ο� Object(����, ���� ��)�� ���� �� ������ ��Ŷ
    //���� socket, ���ε��� X, ���ε��� Y, ���ε��� ID, ���ε��� Ÿ��
    void SendAddObject(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType addID, const ObjectClass objType);

    // Object�� Ŭ���̾�Ʈ���� �������� �����ϰ��� �� �� ������ ��Ŷ
    //���� socket, ������� �ϴ� ID, ������� �ϴ� Ÿ��
    void SendRemoveObject(SOCKET socket, const ObjectIDType removeID, const ObjectClass objType);

    //Object(����, ���� ��)�� ������ �� ������ ��Ŷ
    //���� socket, �̵�x, �̵�y, �̵���id, Ŭ����(�÷��̾�),�̵���id�� �ؽ�������
    void SendMoveObject(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType movedID, const ObjectClass objType, const TextureDirection textureDirection);
}