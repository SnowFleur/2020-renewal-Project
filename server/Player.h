#pragma once

#include <unordered_set>
#include<mutex>

#include"WinSocketHeader.h"
#include"GameObject.h"


using ViewList  = std::unordered_set<int>;
using mutex     = std::mutex;

class CPlayer:public GameObject {
private:
public:
    SOCKET      socket_;
    BoolType    isLogin_;               
    std::unordered_set<int> viewLIst_; 
    char        packetBuffer_[MAX_BUFFER];  //응용단 버퍼
    uIntType    prevSize_;                      
    mutex       lock_;

    CPlayer();
    ~CPlayer() = default;
};