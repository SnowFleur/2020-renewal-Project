#include"Player.h"

CPlayer::CPlayer(const PositionType x, const PositionType y, const HpType hp, const LevelType level,
    const ExpType exp, const DamageType damage, CInputComponent* inputComponent)
    :socket_(INVALID_SOCKET), viewLIst_{}, prevSize_{ 0 }{
    ZeroMemory(packetBuffer_, sizeof(packetBuffer_));
}


SOCKET CPlayer::GetSocket()const {
    return socket_;
}

void CPlayer::SetSocket(const SOCKET socket) {
    socket_ = socket;
}

char* CPlayer::GetPacketBuffer() {
    return packetBuffer_;
}

uIntType CPlayer::GetPrevSize()const {
    return prevSize_;
}

ViewList& CPlayer::GetViewList() {
    return viewLIst_;
}