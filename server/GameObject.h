#pragma once

#include"DataType.h"
#include"OverEx.h"

struct GameObject {
public:
    OverEx          overEx_;
    HpType          hp_;
    PositionType    x_;
    PositionType    y_;
    DamageType      damage_;
    LevelType       level_;
    ExpType         exp_;
    BoolType        isUsed_;


    GameObject() :
        x_{}, y_{}, hp_{}, level_{}, exp_{}, damage_{},
        overEx_{}, isUsed_{ false } {}

    GameObject(const PositionType x, const PositionType y, const HpType hp,
        const LevelType level, const ExpType exp, const DamageType damage) :
        x_(x), y_(y), hp_(hp), level_(level), exp_(exp), damage_(damage), 
        overEx_{},isUsed_{ false } {}
};