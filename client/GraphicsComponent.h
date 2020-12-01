#pragma once


constexpr int MAX_CHARACTER_SEPRITE = 4;

enum class VISIBILITY_OPTION {
    INVISIABLE, VISIABLE
};


__interface IGameObject;

class CGraphicsComponent{
private:
public:
    virtual void SetRenderCharacterDirection(unsigned char renderCharacterDirection) = 0;
    virtual void SetVisibility(VISIBILITY_OPTION option)=0;
    virtual void GenerateSprite(const wchar_t* fileName,const int width, const int height) = 0;
    virtual void SetCharacterTexture(const wchar_t* fileName, const int width, const int height) = 0;
    virtual void SetCharacterTexture(void* texture, const int cycle) = 0;

    virtual void Update(IGameObject&) = 0;

};

