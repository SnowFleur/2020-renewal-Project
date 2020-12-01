#pragma once
#include"GameObject.h"

class CInputComponent;
class CGraphicsComponent;

class CMonsterObject : public IGameObject {
private:
    CInputComponent* inputHandle_;
    CGraphicsComponent* graphicsHandle_;
    ObjectIDType id_;
    unsigned short x_, y_;
    int level_;
    int hp_;
    int exp_;
    bool isused_;
public:
    CMonsterObject(CInputComponent* inputComponent, CGraphicsComponent* graphicsComponent);
    ~CMonsterObject();

    ObjectIDType   Getid()const override;
    void           GetPosition(unsigned short& x, unsigned short& y)override;
    int            GetLevel()const override;
    int            GetHp()const override;
    int            GetExp()const override;
    bool           GetUsed()const override;

    void           SetPosition(const unsigned short x, const unsigned short y)override;
    void           SetLevel(const int level)override;
    void           SetHp(const int hp)override;
    void           SetExp(const int exp)override;
    void           SetRenderCharacterDirection(unsigned char renderCharacterDirection)override;
    void           SetVisibility(VISIBILITY_OPTION option)override;
    void           SetCharacterTexture(const wchar_t* fileName, const int width, const int height) override;
    void           SetCharacterTexture(void* texture, const int cycle)override;
    void           SetUsed(const bool used)override;

    void           Update(CWorld& world)override;
    void           InitGameObject(void* data)override;
};
