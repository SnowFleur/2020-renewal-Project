#pragma once

#include"DataType.h"
#include"OverEx.h"


/*
몬스터가 죽으면 객체를 지우는게 아니라 재활용해서 사용할 것이기 때문에 
복사생성, 이동생성을 다 지운다.

*/

// 20.11.16: 바이트정렬 생각해서 변수 자리 옮기기
struct GameObject {
public:
    OverEx                  overEx_;
    ObjectIDType            id_;
    HpType                  hp_;
    PositionType            x_;
    PositionType            y_;
    DamageType              damage_;
    LevelType               level_;
    ExpType                 exp_;
    BoolType                isUsed_;


    GameObject() :
        x_{}, y_{}, hp_{}, level_{}, exp_{}, damage_{},
        overEx_{}, isUsed_{ false }{}

    GameObject(const PositionType x, const PositionType y, const HpType hp,
        const LevelType level, const ExpType exp, const DamageType damage) :
        x_(x), y_(y), hp_(hp), level_(level), exp_(exp), damage_(damage), 
        overEx_{},isUsed_{ false } {}

};