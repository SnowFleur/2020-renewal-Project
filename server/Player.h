#pragma once

#include <unordered_set>

#include"WinSocketHeader.h"
#include"GameObject.h"
#include"SRWLock.h"

using ViewList  = std::unordered_set<int>;
using mutex     = std::mutex;

class CPlayer:public GameObject {
private:
public:
    SOCKET      socket_;
    BoolType    isLogin_;               
    std::unordered_set<ObjectIDType> viewLIst_; 
    char        packetBuffer_[MAX_BUFFER];  //응용단 버퍼
    uIntType    prevSize_;                      
    CSRWLock    srwLock_;

    CPlayer();
    ~CPlayer() = default;
};