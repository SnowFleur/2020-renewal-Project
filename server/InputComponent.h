#pragma once

class CGameObject;

/*
인터페이스 클래스
*/
class CInputComponent{
private:
public:
    virtual void ExcuteEvent(CGameObject& myObject, CGameObject& targetObject) = 0;
    virtual ~CInputComponent() {};
};

