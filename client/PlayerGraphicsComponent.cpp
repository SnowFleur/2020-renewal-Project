#include"global.h"
#include"PlayerGraphicsComponent.h"
#include"PlayerObject.h"
#include"PlayerUI.h"

// 20.10.25 기본적으로 캐릭터 가리기
CPlayerGraphicsComponent::CPlayerGraphicsComponent() :playerSprite_{}, direction_{},
playerui_{ new CPlayerUI } {
    playerSprite_[direction_].attr &= ~BOB_ATTR_VISIBLE;
}

CPlayerGraphicsComponent::~CPlayerGraphicsComponent() {
    if (playerui_ != nullptr) {
        delete playerui_;
    }
}

void CPlayerGraphicsComponent::Update(IGameObject& gameobject) {
    unsigned short x, y;
    gameobject.GetPosition(x, y);
    playerSprite_[direction_].x = x;
    playerSprite_[direction_].y = y;

    //Draw Sprhite
    Draw_BOB32(&playerSprite_[direction_]);
    
    //Draw PlayerUI
    playerui_->Update(gameobject);
}

#pragma region Set Functions

void CPlayerGraphicsComponent::SetRenderCharacterDirection(unsigned char ChatacterDirection) {
    direction_ = ChatacterDirection;
}

void CPlayerGraphicsComponent::SetVisibility(VISIBILITY_OPTION option) {

    if (option == VISIBILITY_OPTION::VISIABLE) {
        playerSprite_[direction_].attr |= BOB_ATTR_VISIBLE;;
    }

    else if (option == VISIBILITY_OPTION::INVISIABLE) {
        playerSprite_[direction_].attr &= ~BOB_ATTR_VISIBLE;
    }
}

void CPlayerGraphicsComponent::GenerateSprite(const wchar_t* fileName, const int width, const int height) {

    Load_Texture(fileName, UNIT_TEXTURE, width, height);

    for (int i = 0; i < MAX_CHARACTER_SEPRITE; ++i) {
        if (!Create_BOB32(&playerSprite_[i], 0, 0, 70, height, 0, BOB_ATTR_SINGLE_FRAME)) {
            //std::cout << "Fail Create_BOB32" << std::endl;
        }
        //Frame은 출력할 이미지 칸
        Load_Frame_BOB32(&playerSprite_[i], UNIT_TEXTURE, 0, i, 0, BITMAP_EXTRACT_MODE_CELL);
        playerSprite_[i].attr |= BOB_ATTR_VISIBLE;
    }
}

void CPlayerGraphicsComponent::SetCharacterTexture(const wchar_t* fileName, const int width, const int height) {
    GenerateSprite(fileName, width, height);
}

void CPlayerGraphicsComponent::SetCharacterTexture(void* texture, const int cycle) {
    for (int i = 0; i < cycle; ++i) {
        playerSprite_[i] = *(reinterpret_cast<BOB*>(texture) + i);
    }
}
#pragma endregion
