#pragma once
#include<queue>
#include"EventType.h"
#include"SRWLock.h"


/*
���߿� �����ϰ� Lock Free Queue�� ������
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

    void        Emplace(EVENT_ST& es)noexcept;
    void        Emplace(EVENT_ST&& es)noexcept;
    EVENT_ST    PopTopValue()noexcept;
    bool        CheckTimerOfTopValue()noexcept;
    bool        Empty() noexcept;
};

class CTimerQueueHandle {
private:
    CTimerQueueHandle() = default;
public:
     CTimerQueue    queue_;
    static CTimerQueueHandle* GetInstance() {
        // C++�� �������� static�� ������ �������� �����Ѵ�. (������)
        static CTimerQueueHandle* instance_ = nullptr;

        if (instance_ == nullptr) {
            instance_ = new CTimerQueueHandle();
        }
        return instance_;
    }

};

