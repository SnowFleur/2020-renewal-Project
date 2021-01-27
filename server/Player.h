#pragma once

#include"WinSocketHeader.h"
#include"GameObject.h"

using mutex     = std::mutex;

class CPlayer:public CGameObject {
public:
    SOCKET      socket_;
    ViewList    viewLIst_;
    char        packetBuffer_[MAX_BUFFER];  //응용단 버퍼
    uIntType    prevSize_;

    CPlayer() = delete;
    CPlayer(const PositionType x, const PositionType y, const HpType hp, const LevelType level,
        const ExpType exp, const DamageType damage, CInputComponent* inputComponent);
    ~CPlayer() = default;

    SOCKET    GetSocket()const override;
    void      SetSocket(const SOCKET socket)override;
    char*     GetPacketBuffer() override;
    uIntType  GetPrevSize()const override;
    ViewList& GetViewList() override;
};