#pragma once

class CSector;
struct EVENT_ST;
/*
�������̽� Ŭ����
*/
class CInputComponent{
private:
public:
    virtual void ExcuteEvent(CSector& sector,EVENT_ST& ev) = 0;
    virtual ~CInputComponent() {};
};

