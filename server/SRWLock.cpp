#include"SRWLock.h"

/*
나중에 직접 RAII 기반으로 구현
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
