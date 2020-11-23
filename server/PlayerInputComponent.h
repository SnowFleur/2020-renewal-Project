#pragma once
#include"InputComponent.h"
class CPlayerInputComponent : public CInputComponent {
private:

public:
    void Update(GameObject& gameobject)override {};
    ~CPlayerInputComponent()override = default;
};