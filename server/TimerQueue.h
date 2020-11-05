#pragma once
#include<queue>
#include"EventType.h"
#include"SRWLock.h"


/*
나중에 공부하고 Lock Free Queue로 변경요망
*/

class CTimerQueue {
private:
    std::priority_queue<EVENT_ST>   timerQueue_;
    CSRWLock                        srwLock_;
public:
    CTimerQueue();
    ~CTimerQueue() = default;
    CTimerQueue(const CTimerQueue&) = delete;
    CTimerQueue(CTimerQueue&&) = delete;
    CTimerQueue operator=(const CTimerQueue&) = delete;
    CTimerQueue operator=( CTimerQueue&&) = delete;

    void        Emplace(EVENT_ST& es);
    void        Emplace(EVENT_ST&& es);
    EVENT_ST    PopTopValue();
};