#pragma once

class CGameObject;

/*
�������̽� Ŭ����
*/
class CInputComponent{
private:
public:
    virtual void Update(CGameObject& gameobject) = 0;
    virtual ~CInputComponent() {};
};

