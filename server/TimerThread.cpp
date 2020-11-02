#include<thread>
#include"TimerThread.h"
#include"OverEx.h"
#include"LogCollector.h"

void CTimerThread::RunTImerThread(HANDLE& iocp) {
    iocp_ = iocp;
    Thread timerThread{ Thread{&CTimerThread::TimerThread,this} };
    CLogCollector::GetInstance()->PrintLog("Start Timer Thread\n");
    timerThread.join();
}

void CTimerThread::TimerThread() {
    while (true) {
        //������ �ϳ��� CPU�ϳ��� ��ƸԴ°��� ����
        std::this_thread::sleep_for(10ms);

        //Test�� 
        OverEx overEx;
        overEx.ev_ = EV_MONSTER_MOVE;
        PostQueuedCompletionStatus(iocp_, 0, 1, &overEx.over_);

    }
}