#pragma once

enum class PLAYER_ACTION;
#include"../server/Protocol.h"
/*
Network 인터페이스
*/
__interface INetwork{
public:

    virtual void        ReadPacket() = 0;
    virtual void        CloseSocket() = 0;
    virtual void        SendPlayerPosition(const PositionType x, const PositionType y, 
                        const TextureDirection textureDirectione) = 0;
    virtual void        SendChatMeesage(const wchar_t* message) = 0;
    virtual void        SendLogin(const wchar_t* id, size_t lenSize) =0;
    virtual void        SendPlayerAction(PLAYER_ACTION action)=0;
};

