#pragma once

struct GameObject;

/*
�������̽� Ŭ����
*/
class CInputComponent{
private:
public:
    virtual void Update(GameObject& gameobject) = 0;
    virtual ~CInputComponent() {};
};

