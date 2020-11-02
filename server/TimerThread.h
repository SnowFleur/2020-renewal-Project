#pragma once
#include"WinSocketHeader.h"
#include"TimerQueue.h"

using Thread = std::thread;


class CTimerThread {
private:
    CTimerQueue queue_;
    HANDLE      iocp_;
public:
    CTimerThread() = default;
    void RunTImerThread(HANDLE& iocp);
    void TimerThread();
};