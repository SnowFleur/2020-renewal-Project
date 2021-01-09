#include"TimerThread.h"
#include"OverEx.h"
#include"LogCollector.h"
#include"TimerQueue.h"


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
            if (CTimerQueueHandle::GetInstance()->queue_.Empty() == true) {
                break;
            }
            if (CTimerQueueHandle::GetInstance()->queue_.CheckTimerOfTopValue() == false) {
                break;
            }


            EVENT_ST ev = CTimerQueueHandle::GetInstance()->queue_.PopTopValue();
            OverEx* overEx = new OverEx;

            switch (ev.type) {
            case EV_MONSTER_MOVE: {
                overEx->ioEvent_ = IO_EVENT::IO_MONSTER_EVENT;
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