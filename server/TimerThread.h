#pragma once
#include"WinSocketHeader.h"
#include"TimerQueue.h"

using Thread = std::thread;

class CTimerThread {
private:
    CTimerQueue     timerQueue_;
    HANDLE          iocp_;
    void            TimerThread();
public:
    CTimerThread() = default;

    CTimerThread&   GetTimerThreadClass();
    void            AddEventInTimerQueue(EVENT_ST&& et);
    void            RunTimerThread(HANDLE& iocp);
};

