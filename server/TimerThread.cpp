#include<thread>
#include"TimerThread.h"
#include"OverEx.h"
#include"LogCollector.h"

CTimerThread& CTimerThread::GetTimerThreadClass() {
    return *this;
}

void CTimerThread::AddEventInTimerQueue(EVENT_ST&& st) {
    timerQueue_.Emplace(std::move(st));
}

void CTimerThread::RunTimerThread(HANDLE& iocp) {
    iocp_ = iocp;
    Thread timerThread{ Thread{&CTimerThread::TimerThread,this} };
    CLogCollector::GetInstance()->PrintLog("Start Timer Thread\n");
    timerThread.join();
}

void CTimerThread::TimerThread() {
    while (true) {
        //쓰레드 하나가 CPU하나를 잡아먹는것을 방지
        std::this_thread::sleep_for(10ms);

        while (true) {
            if (timerQueue_.Empty() == true) {
                break;
            }
            if (timerQueue_.CheckTimerOfTopValue() == false) {
                break;
            }


            EVENT_ST ev = timerQueue_.PopTopValue();
            OverEx* overEx = new OverEx;

            switch (ev.type) {
            case EV_MONSTER_MOVE: {
                overEx->ev_ = EV_MONSTER_MOVE;
                overEx->target_player_ = ev.target_id;
                break;
            }
            case EV_MONSER_ATTACK_PLAYER: {
                break;
            }
            default:
                CLogCollector::GetInstance()->PrintLog("Not Defind EVENT In TimerThread");
                break;
            }
            PostQueuedCompletionStatus(iocp_, 1, ev.obj_id, &overEx->over_);
        }
    }
}