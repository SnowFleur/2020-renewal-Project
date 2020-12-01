#pragma once
#include"InputComponent.h"

class CMonsterInputComponent : public CInputComponent {
private:
public:
    virtual void Update(IGameObject& gameobject,CWorld& world);
};

