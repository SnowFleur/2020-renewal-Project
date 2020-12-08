#pragma once
#include<iostream>
#include<string>
#include<queue>
#include<mutex>

/*
개요 및 역할: Thread Safe한반 Log 출력 및 Log 저장용 싱글턴 기반 Class

- 20.10.22
- 헤더파일에 작성
- 출력만 가능하게 적용
- 추후 Queue를 이용해 Log 입출력까지 실행
*/

class CLogCollector {
private:
    std::mutex      lock_;
    std::queue<int> logQueue_;
    CLogCollector() = default;
public:
    static CLogCollector* GetInstance() {
        // C++은 지역변수 static에 스레드 세이프를 보장한다. (생성만)
        static CLogCollector* instance_ = nullptr;

        if (instance_ == nullptr) {
            instance_ = new CLogCollector();
        }
        return instance_;
    }

    /*
    C++ 최적화 코드 보고 수정하자
    나중에 Mutex 추가
    */
    void PrintLog(std::string&& errorMessage, int ErrorCode = -777) {
        if (ErrorCode != -777) {
            std::lock_guard<std::mutex>guard{ lock_ };
            std::cout << "ErrorCode: [" << ErrorCode << "] - " << errorMessage << "\n";
        }
        else {
            std::lock_guard<std::mutex>guard{ lock_ };
            std::cout << errorMessage << "\n";
        }
    }
};
