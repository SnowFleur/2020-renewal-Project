#pragma once
#include"GraphicsComponent.h"

class CPlayerUI;

/*
�̰Ÿ� �����ͷ� ���� �����غ���
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

