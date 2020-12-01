#pragma once
#include"GameObject.h"


class CInputComponent;
class CGraphicsComponent;

enum class NPC_TYPE {
    QUEST_NPC = 0, HEAL_NPC, SHOP_NPC, NULL_NPC
};

class CNpcObject : public IGameObject {
private:
    CInputComponent* inputHandle_;
    CGraphicsComponent* graphicsHandle_;
    int                 id_;
    unsigned short      x_, y_;
    NPC_TYPE            npcType_;
public:
    CNpcObject(CInputComponent* inputComponent, CGraphicsComponent* graphicsComponent);
    ~CNpcObject();

    ObjectIDType   Getid()const override;
    void           GetPosition(unsigned short& x, unsigned short& y)override;
    int            GetLevel()const override;
    int            GetHp()const override;
    int            GetExp()const override;
    bool           GetUsed()const override { return true; } //NPC는 구지 필요없지만.. 

    void           SetPosition(const unsigned short x, const unsigned short y) override;
    void           SetLevel(const int level) override;
    void           SetHp(const int hp) override;
    void           SetExp(const int exp) override;
    void           SetRenderCharacterDirection(unsigned char renderCharacterDirection)override;
    void           SetVisibility(VISIBILITY_OPTION option)override;
    void           SetCharacterTexture(const wchar_t* fileName, const int width, const int height) override;
    void           SetCharacterTexture(void* texture, const int cycle)override;
    void           SetUsed(const bool used)override {}


    void           Update(CWorld& world) override;
    void           InitGameObject(void* data) override;

    void           SetNpcType(NPC_TYPE type);

};

