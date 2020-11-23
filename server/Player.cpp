#include"Player.h"

CPlayer::CPlayer(const PositionType x, const PositionType y, const HpType hp, const LevelType level,
    const ExpType exp, const DamageType damage, CInputComponent* inputComponent)
    :socket_(INVALID_SOCKET), isLogin_{ false }, viewLIst_{}, srwLock_{}, prevSize_{ 0 },
    GameObject{ x,y,hp,level,exp,damage,inputComponent }{

    ZeroMemory(packetBuffer_, sizeof(packetBuffer_));
}

