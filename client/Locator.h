#pragma once

__interface INetwork;

/*
���� ������ ����
*/
class CLocator {
private:
    static INetwork*  netWorkHandle_;
public:               
    static INetwork*  GetNetwork();
    static void       Provide(INetwork* network);
};


