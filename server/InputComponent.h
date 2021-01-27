#pragma once

struct GameObject;

/*
인터페이스 클래스
*/
class CInputComponent{
private:
public:
    virtual void Update(GameObject& gameobject) = 0;
    virtual ~CInputComponent() {};
};

