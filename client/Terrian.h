#pragma once
#include"global.h"

constexpr int TEXTURE_WIDTH         = 69;
constexpr int TEXTURE_HEIGHT        = 69;
constexpr int TEXTURE_MAX_WIDTH     = 770;
constexpr int TEXTURE_MAX_HEIGHT    = 70;


enum TEXTURE_NUMBERS {
    TEXTURE_GROUND = 2, TEXTURE_WATER = 3
};


class CTerrian{
private:
public:
    int             movementCost_;
    bool            moveAble_;
    BOB             texture_;

    CTerrian() = default;
    ~CTerrian() = default;

    int			    GetMovementCost()const;
    void            SetTexturePosition(const int x, const int y);
    bool		    MoveAble()const;
    BOB             GetTexture()const;
    void            InitData(const int cost, const bool moveable, 
                             const wchar_t* fileName,int textureNumber);
};

