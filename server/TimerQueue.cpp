#include"TimerQueue.h"
#include"SRWLock.h"

CTimerQueue::CTimerQueue() :timerQueue_{} {}

void CTimerQueue::Emplace(EVENT_ST& st) {
    srwLock_.Writelock();
    timerQueue_.emplace(st);
    srwLock_.Writeunlock();
}

EVENT_ST CTimerQueue::PopTopValue() {
    srwLock_.Writelock();
    auto topValue = timerQueue_.top();
    timerQueue_.pop();
    srwLock_.Writeunlock();
    return topValue;
}