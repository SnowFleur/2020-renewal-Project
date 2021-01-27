#pragma once
#include"InputComponent.h"
class CPlayerInputComponent : public CInputComponent {
private:

public:
    void Update(CGameObject& gameobject)override {};
    ~CPlayerInputComponent()override = default;
};