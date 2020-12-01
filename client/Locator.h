#pragma once

__interface INetwork;

/*
서비스 중재자 패턴
*/
class CLocator {
private:
    static INetwork*  netWorkHandle_;
public:               
    static INetwork*  GetNetwork();
    static void       Provide(INetwork* network);
};


