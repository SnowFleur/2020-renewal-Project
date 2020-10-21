#pragma once

#include"DataType.h"

struct GameObject {
public:
    HpType          hp_;
    PositionType    x_;
    PositionType    y_;
    LevelType       level_;
    ExpType         exp_;
    BoolType        isUsed_;

    GameObject() :
        hp_(100), x_(0), y_(0), level_(1), exp_(0), isUsed_{ false } {}
};