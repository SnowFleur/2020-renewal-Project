#include"SRWLock.h"

/*
���߿� ���� RAII ������� ����
*/
CSRWLock::CSRWLock() {
    //Initialization SRWLock
    InitializeSRWLock(&srwlock_);
}

void CSRWLock::Writelock()noexcept {
    AcquireSRWLockExclusive(&srwlock_);
}

void CSRWLock::Writeunlock()noexcept {
    ReleaseSRWLockExclusive(&srwlock_);
}

void CSRWLock::Readlock()noexcept {
    AcquireSRWLockShared(&srwlock_);
}

void CSRWLock::Readunlock()noexcept {
    ReleaseSRWLockShared(&srwlock_);
}
