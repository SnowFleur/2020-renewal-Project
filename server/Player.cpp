#include"Player.h"

CPlayer::CPlayer() :
    socket_(INVALID_SOCKET),isLogin_{ false }, viewLIst_{}, srwLock_{},
    prevSize_{0}{}

