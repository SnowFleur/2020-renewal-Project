#pragma once
#include"GraphicsComponent.h"

class CPlayerUI;

/*
이거를 포인터로 할지 생각해보자
*/
class CPlayerGraphicsComponent :public CGraphicsComponent {
private:
    BOB             playerSprite_[MAX_CHARACTER_SEPRITE];
    CPlayerUI*      playerui_;
    unsigned char   direction_;
public:
    CPlayerGraphicsComponent();
    ~CPlayerGraphicsComponent();

    void SetRenderCharacterDirection(unsigned char ChatacterDirection)override;
    void SetVisibility(VISIBILITY_OPTION option)override;
    void GenerateSprite(const wchar_t* fileName, const int width, const int height)override;
    void SetCharacterTexture(const wchar_t* fileName, const int width, const int height)override;
    void SetCharacterTexture(void* texture, const int cycle)override;

    void Update(IGameObject& gameobject)override;
};

