#pragma once

#include"DataType.h"
#include"OverEx.h"

/*
���Ͱ� ������ ��ü�� ����°� �ƴ϶� ��Ȱ���ؼ� ����� ���̱� ������ 
�������, �̵������� �� �����.
*/
class CInputComponent;

struct GameObject {
public:
    OverEx                  overEx_;
    PositionType            x_;
    PositionType            y_;
    HpType                  hp_;
    DamageType              damage_;
    LevelType               level_;
    ExpType                 exp_;
    BoolType                isUsed_;
    CInputComponent*        inputComponent_;

    GameObject(const PositionType x, const PositionType y, const HpType hp,
        const LevelType level, const ExpType exp, const DamageType damage, CInputComponent* inputComponent) :
        x_(x), y_(y), hp_(hp), level_(level), exp_(exp), damage_(damage),
        overEx_{}, isUsed_{ false }, inputComponent_{ nullptr } {
        inputComponent_ = inputComponent;
    }

    virtual ~GameObject();

};