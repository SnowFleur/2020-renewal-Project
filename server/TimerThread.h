#pragma once
#include<thread>
#include"WinSocketHeader.h"
using Thread = std::thread;

class CTimerThread {
private:
    HANDLE          iocp_;
    void            TimerThread();
public:
    CTimerThread() = default;
    void            RunTimerThread(HANDLE& iocp);
};

