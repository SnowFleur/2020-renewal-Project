#pragma once
#include"InputComponent.h"
class CPlayerInputComponent : public CInputComponent {
private:
public:
    void ExcuteEvent(CSector& sector, EVENT_ST& ev)override {};
    ~CPlayerInputComponent()override = default;
};