#pragma once

#include"WinSocketHeader.h"
#include"GameObject.h"

using mutex     = std::mutex;

class CPlayer:public CGameObject {
public:
    SOCKET       socket_;
    ViewList     viewLIst_;
    char         packetBuffer_[MAX_BUFFER];  //응용단 버퍼
    uInt32Type   prevSize_;

    CPlayer() = delete;
    CPlayer(const PositionType x, const PositionType y, const HpType hp, const LevelType level,
        const ExpType exp, const DamageType damage, CInputComponent* inputComponent);
    ~CPlayer() = default;

    SOCKET       GetSocket()const override;
    char*        GetPacketBuffer() override;
    uInt32Type   GetPrevSize()const override;
    ViewList&    GetViewList() override;
    void         SetSocket(const SOCKET socket)override;
    void         SetPrevSize(const uInt32Type size)override;

};