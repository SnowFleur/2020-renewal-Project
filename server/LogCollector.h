#pragma once
#include<iostream>
#include<string>
#include<queue>
#include<mutex>

/*
���� �� ����: Thread Safe�ѹ� Log ��� �� Log ����� �̱��� ��� Class

- 20.10.22
- ������Ͽ� �ۼ�
- ��¸� �����ϰ� ����
- ���� Queue�� �̿��� Log ����±��� ����
*/

class CLogCollector {
private:
    std::mutex      lock_;
    std::queue<int> logQueue_;
    CLogCollector() = default;
public:
    static CLogCollector* GetInstance() {
        // C++�� �������� static�� ������ �������� �����Ѵ�. (������)
        static CLogCollector* instance_ = nullptr;

        if (instance_ == nullptr) {
            instance_ = new CLogCollector();
        }
        return instance_;
    }

    /*
    C++ ����ȭ �ڵ� ���� ��������
    ���߿� Mutex �߰�
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
