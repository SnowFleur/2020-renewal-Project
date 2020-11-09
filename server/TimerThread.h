#pragma once
#include"WinSocketHeader.h"
#include"TimerQueue.h"

using Thread = std::thread;

class CTimerThread {
private:
    CTimerQueue timerQueue_;
    HANDLE      iocp_;
public:
    CTimerThread() = default;

    void AddEventInTimerQueue(EVENT_ST&& et);
    void RunTimerThread(HANDLE& iocp);
    void TimerThread();
};

