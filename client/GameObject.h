#pragma once
#include"..\server\Protocol.h"

/*
GameObject 인터페이스
*/

enum class VISIBILITY_OPTION;

class CWorld;


__interface IGameObject {
public:
    virtual ObjectIDType   Getid()const = 0;
    virtual void           GetPosition(unsigned short& x, unsigned short& y) = 0;
    virtual int            GetLevel()const = 0;
    virtual int            GetHp()const = 0;
    virtual int            GetExp()const = 0;
    virtual bool           GetUsed()const = 0;

    virtual void           SetPosition(const unsigned short x, const unsigned short y) = 0;
    virtual void           SetLevel(const int level) = 0;
    virtual void           SetHp(const int hp) = 0;
    virtual void           SetExp(const int exp) = 0;
    virtual void           SetRenderCharacterDirection(unsigned char renderCharacterDirection) = 0;
    virtual void           SetVisibility(VISIBILITY_OPTION option) = 0;
    virtual void           SetCharacterTexture(const wchar_t* fileName, const int width, const int height) = 0;
    virtual void           SetCharacterTexture(void* texture, const int cycle) = 0;
    virtual void           SetUsed(const bool used);

    virtual void           Update(CWorld& word) = 0;
    virtual void           InitGameObject(void* data) = 0;
};
