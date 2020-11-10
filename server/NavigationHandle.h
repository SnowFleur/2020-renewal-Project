#pragma once
#include"Navigation.h"
#include"Protocol.h"

/*
전역 navigation Class
각 구역에 따른 navigaion을 가지고 있고 각 구역에 따른 SRWLock을 가지고 있는다.
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
        // C++은 지역변수 static에 스레드 세이프를 보장한다. (생성만)
        static CNavigationHandle* instance_ = nullptr;

        if (instance_ == nullptr) {
            instance_ = new CNavigationHandle();
        }
        return instance_;
    }
};

