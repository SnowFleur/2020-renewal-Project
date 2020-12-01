#pragma once
#include"gpdumb1.h"

constexpr int MAX_CHAT_SIZE = 50;
class IGameObject;

class CPlayerUI {
private:
    static wchar_t  chatArray_[MAX_CHAT_SIZE];
    BITMAP_IMAGE    chatbar_[2];
    
public:
    CPlayerUI();
    static void InputChat(WPARAM);
    void Update(IGameObject& gameobject);
};








