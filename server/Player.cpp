#include"Player.h"

CPlayer::CPlayer() :
    socket_(INVALID_SOCKET), overEx_{}, isLogin_{ false }, viewLIst_{}, lock_{},
    prevSize_{0}{}

