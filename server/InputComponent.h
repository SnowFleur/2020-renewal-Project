#pragma once

class CGameObject;

/*
�������̽� Ŭ����
*/
class CInputComponent{
private:
public:
    virtual void ExcuteEvent(CGameObject& myObject, CGameObject& targetObject) = 0;
    virtual ~CInputComponent() {};
};

