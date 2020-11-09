#include"TimerQueue.h"

CTimerQueue::CTimerQueue() :timerQueue_{} {}

void CTimerQueue::Emplace(EVENT_ST& st) noexcept {
    srwLock_.Writelock();
    timerQueue_.emplace(st);
    srwLock_.Writeunlock();
}

void CTimerQueue::Emplace(EVENT_ST&& st)noexcept {
    srwLock_.Writelock();
    timerQueue_.emplace(st);
    srwLock_.Writeunlock();
}

EVENT_ST CTimerQueue::PopTopValue()noexcept {
    srwLock_.Writelock();
    auto topValue = timerQueue_.top();
    timerQueue_.pop();
    srwLock_.Writeunlock();
    return topValue;
}

bool CTimerQueue::CheckTimerOfTopValue()noexcept {
    bool overRunTime = false;
    srwLock_.Readlock();
    if (timerQueue_.top().start_time < high_resolution_clock::now()) {
        srwLock_.Readunlock();
        overRunTime = true;
    }
    else {
        srwLock_.Readunlock();
    }
    return overRunTime;
}

bool CTimerQueue::Empty() noexcept {
    srwLock_.Readlock();
    bool isEmpty = timerQueue_.empty();
    srwLock_.Readunlock();
    return isEmpty;
}
