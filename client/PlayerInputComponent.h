#pragma once
#include"InputComponent.h"


class CPlayerInputComponent : public CInputComponent {
private:
public:
    void Update(IGameObject& gameobject, CWorld& world)override;
};

