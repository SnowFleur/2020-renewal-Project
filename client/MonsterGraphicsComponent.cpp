#include"global.h"

#include "MonsterGraphicsComponent.h"
#include"PlayerObject.h"

CMonsterGraphicsComponent::CMonsterGraphicsComponent() :monsterSprite_{}, direction_{} {}

void CMonsterGraphicsComponent::Update(IGameObject& gameobject) {
    
    

    unsigned short x, y;
    gameobject.GetPosition(x, y);
    monsterSprite_[direction_].x = x;
    monsterSprite_[direction_].y = y;

    //Draw Sprhite
    Draw_BOB32(&monsterSprite_[direction_]);

    //Draw MonsterInfo
    wchar_t text[300]{};

    D3DXVECTOR3 pos = D3DXVECTOR3((x - g_left_x) * 65.0f + 8,
        (y - g_top_y) * 65.0f + 8, 0.0);

    wsprintf(text, L"Level:%d", gameobject.GetLevel());
    Draw_Text_D3D(text, static_cast<int>(pos.x - 5), static_cast<int>(pos.y - 15), D3DCOLOR_ARGB(255, 0, 0, 0));

    wsprintf(text, L" %3d (%d)", gameobject.Getid(), gameobject.GetHp());
    Draw_Text_D3D(text, static_cast<int>(pos.x + 70), static_cast<int>(pos.y + 15), D3DCOLOR_ARGB(255, 0, 0, 0));

}

#pragma region Set Functions

void CMonsterGraphicsComponent::SetRenderCharacterDirection(unsigned char ChatacterDirection) {
    direction_ = ChatacterDirection;
}

void CMonsterGraphicsComponent::SetVisibility(VISIBILITY_OPTION option) {

    if (option == VISIBILITY_OPTION::VISIABLE) {
        monsterSprite_[direction_].attr |= BOB_ATTR_VISIBLE;;
    }

    else if (option == VISIBILITY_OPTION::INVISIABLE) {
        monsterSprite_[direction_].attr &= ~BOB_ATTR_VISIBLE;
    }
}

void CMonsterGraphicsComponent::GenerateSprite(const wchar_t* fileName, const int width, const int height) {

    Load_Texture(fileName, UNIT_TEXTURE, width, height);

    for (int i = 0; i < MAX_CHARACTER_SEPRITE; ++i) {

        if (!::Create_BOB32(&monsterSprite_[i], 0, 0, width, height, 0, BOB_ATTR_SINGLE_FRAME)) {
            //std::cout << "Fail Create_BOB32" << std::endl;
        }

        //Frame은 출력할 이미지 칸
        Load_Frame_BOB32(&monsterSprite_[i], UNIT_TEXTURE, 0, i, 0, BITMAP_EXTRACT_MODE_CELL);
        monsterSprite_[i].attr |= BOB_ATTR_VISIBLE;

    }
}

void CMonsterGraphicsComponent::SetCharacterTexture(const wchar_t* fileName, const int width, const int height) {
    GenerateSprite(fileName, width, height);
}

void CMonsterGraphicsComponent::SetCharacterTexture(void* texture, const int cycle) {
    for (int i = 0; i < cycle; ++i) {
        monsterSprite_[i] = *(reinterpret_cast<BOB*>(texture)+i);
    }
}
#pragma endregion