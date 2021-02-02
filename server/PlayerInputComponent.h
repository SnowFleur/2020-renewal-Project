#pragma once
#include"InputComponent.h"
class CPlayerInputComponent : public CInputComponent {
private:
public:
    void ExcuteEvent(CGameObject& myObject, CGameObject& targetObject)override {};
    ~CPlayerInputComponent()override = default;
};