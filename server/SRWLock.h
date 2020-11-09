#pragma once
#include<mutex>
#include<atomic>
#include<Windows.h>

class CSRWLock {
private:
    SRWLOCK srwlock_;
public:
    void    Readlock() noexcept;
    void    Readunlock()noexcept;
    void    Writelock()noexcept;
    void    Writeunlock()noexcept;

    CSRWLock();
    ~CSRWLock() = default;
};