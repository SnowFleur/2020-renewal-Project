#pragma once
#include<iostream>
#include<string>

/*
���� �� ����: Thread Safe�ѹ� Log ��� �� Log ����� �̱��� ��� Class

- 20.10.22
- ������Ͽ� �ۼ�
- ��¸� �����ϰ� ����
- ���� Queue�� �̿��� Log ����±��� ����
*/

class CLogCollector {
private:
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
    void PrintLog(std::string&& errorMessage, int ErrorCode = -777)const {
        if (ErrorCode != -777)
            std::cout << "ErrorCode: [" << ErrorCode << "] - ";
        std::cout << errorMessage << "\n";
    }
};
