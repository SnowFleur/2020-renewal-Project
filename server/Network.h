#pragma once
#include"OverEx.h"
#include"LogCollector.h"
#include"DataType.h"
#include"MemoryPool.h"
#include"FixedMemoryBlock.h"

constexpr int MAX_MEMORYPOOL_SIZE=10000;
using UPtrPool = std::unique_ptr<CMemoryPool<CFixedMemoryBlock>>;

/*
Send Recv는 관련이 너무 많으니 여기로 빼자
Send 와 Recv는 Thrad Safe 하니 Pointer를 이용해서 처리하자.
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
    //메모리 풀 회수
    void DeallocateMemory(void* memoryPtr);
#pragma endregion

    //클라이언트의 접속 성공을 알림
    //보낼 소켓, 접속한 유저 X, 접속한 유저 Y, 접속한 유저 ID
    void SendLoginOk(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType addID);

    //새로운 Object(몬스터, 유저 등)이 생길 때 보내는 패킷
    //보낼 socket, 새로들어온 X, 새로들어온 Y, 새로들어온 ID, 새로들어온 타입
    void SendAddObject(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType addID, const ObjectClass objType);

    // Object를 클라이언트에서 렌더링을 중지하고자 할 때 보내는 패킷
    //보낼 socket, 지우고자 하는 ID, 지우고자 하는 타입
    void SendRemoveObject(SOCKET socket, const ObjectIDType removeID, const ObjectClass objType);

    //Object(몬스터, 유저 등)이 움직일 때 보내는 패킷
    //보낼 socket, 이동x, 이동y, 이동한id, 클래스(플레이어),이동한id의 텍스쳐정보
    void SendMoveObject(SOCKET socket, const PositionType x, const PositionType y,
        const ObjectIDType movedID, const ObjectClass objType, const TextureDirection textureDirection);
}