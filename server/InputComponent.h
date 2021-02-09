#pragma once

class CSector;
struct EVENT_ST;
/*
인터페이스 클래스
*/
class CInputComponent{
private:
public:
    virtual void ExcuteEvent(CSector& sector,EVENT_ST& ev) = 0;
    virtual ~CInputComponent() {};
};

