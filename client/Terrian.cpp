#include "Terrian.h"


int CTerrian::GetMovementCost()const {
    return movementCost_;
}

bool CTerrian::MoveAble()const {
    return moveAble_;
}

BOB CTerrian::GetTexture()const {
    return texture_;
}

void CTerrian::SetTexturePosition(const int x, const int y) {
    texture_.x = x;
    texture_.y = y;
}

void CTerrian::InitData(int cost, bool moveable, 
    const wchar_t* fileName, int textureNumber) {

    Load_Texture(fileName, UNIT_TEXTURE,770 ,70);

    if (!::Create_BOB32(&texture_, 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, BOB_ATTR_SINGLE_FRAME)) {
        //std::cout << "Fail Create_BOB32" << std::endl;
    }

    //Frame은 출력할 이미지 칸
    Load_Frame_BOB32(&texture_, UNIT_TEXTURE, 0,textureNumber,0, BITMAP_EXTRACT_MODE_CELL);
    texture_.attr |= BOB_ATTR_VISIBLE;


    movementCost_ = cost;
    moveAble_ = moveable;
}
