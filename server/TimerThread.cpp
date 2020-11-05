#include<thread>
#include"TimerThread.h"
#include"OverEx.h"
#include"LogCollector.h"

void CTimerThread::AddEventInTimerQueue(EVENT_ST&& st) {
    queue_.Emplace(std::move(st));
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



        //Test용 
        OverEx overEx;
        overEx.ev_ = EV_MONSTER_MOVE;
        PostQueuedCompletionStatus(iocp_, 0, 1, &overEx.over_);

    }
}