#pragma once
#include"Navigation.h"
#include"Protocol.h"

/*
���� navigation Class
�� ������ ���� navigaion�� ������ �ְ� �� ������ ���� SRWLock�� ������ �ִ´�.
*/
class CNavigationHandle {
private:
    CNavigationHandle() {
        
        navigation[0].SetMapData(0, 15, 15);
        navigation[1].SetMapData(1, 15, 15);
    }
public:
    CNavigation                     navigation[MAP_DEFINDS::DIVISION_SECTOR];
    static CNavigationHandle*       handle_;

    static CNavigationHandle* GetInstance() {
        // C++�� �������� static�� ������ �������� �����Ѵ�. (������)
        static CNavigationHandle* instance_ = nullptr;

        if (instance_ == nullptr) {
            instance_ = new CNavigationHandle();
        }
        return instance_;
    }
};

