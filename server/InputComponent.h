#pragma once

class CGameObject;

/*
인터페이스 클래스
*/
class CInputComponent{
private:
public:
    virtual void Update(CGameObject& gameobject) = 0;
    virtual ~CInputComponent() {};
};

