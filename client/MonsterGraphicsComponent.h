#pragma once
#include"GraphicsComponent.h"
#include"gpdumb1.h"

class CMonsterGraphicsComponent :public CGraphicsComponent {
private:
    BOB           monsterSprite_[4];
    unsigned char direction_;
public:
    CMonsterGraphicsComponent();
    ~CMonsterGraphicsComponent() = default;

    void SetRenderCharacterDirection(unsigned char ChatacterDirection)override;
    void SetVisibility(VISIBILITY_OPTION option)override;
    void GenerateSprite(const wchar_t* fileName, const int width, const int height)override;
    void SetCharacterTexture(const wchar_t* fileName, const int width, const int height)override;
    void SetCharacterTexture(void* texture, const int cycle)override;

    void Update(IGameObject& gameobject)override;
    
};

