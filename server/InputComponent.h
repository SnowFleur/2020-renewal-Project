#pragma once

struct GameObject;

class CInputComponent{
private:
public:
    virtual void Update(GameObject& gameobject) = 0;
    virtual ~CInputComponent() {};
};

