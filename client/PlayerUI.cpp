#include"global.h"
#include"GameObject.h"
#include "PlayerUI.h"

#include<iostream>
wchar_t CPlayerUI::chatArray_[MAX_CHAT_SIZE];

CPlayerUI::CPlayerUI() {
  /*  Create_Bitmap32(&chatbar_[0], 0, 0, 770, 30);
    Load_Image_Bitmap32(&chatbar_[0], L"resource/Chat_bar_1.PNG", 770, 30, BITMAP_EXTRACT_MODE_ABS);
    chatbar_[0].x = 0;
    chatbar_[0].y = screen_height - 64;
    chatbar_[0].height = 770;
    chatbar_[0].width = 30;*/

   /* Create_Bitmap32(&chatbar_[1], 0, 0, 770, 30);
    Load_Image_Bitmap32(&chatbar_[1], L"resource/Chat_bar_2.PNG", 770, 30, BITMAP_EXTRACT_MODE_ABS);
    chatbar_[1].x = 0;
    chatbar_[1].y = screen_height - 64;
    chatbar_[1].height = 770;
    chatbar_[1].width = 30;*/

}

/*
UI는 잠시 STOP~
*/
void CPlayerUI::InputChat(WPARAM wparam) {
    static int arraySize{};
    if (wparam == VK_RETURN) {
        arraySize = 0;
        memset(&chatArray_, 0, MAX_CHAT_SIZE);
    }
    else {
        if ((arraySize+1) < MAX_CHAT_SIZE) {
            chatArray_[arraySize++] = wparam;
            chatArray_[arraySize] = '\0';
        }
    }
}

void CPlayerUI::Update(IGameObject& gameobject) {
    
    wchar_t text[300]{};

    wsprintf(text, L"Player HP: %3d", gameobject.GetHp());
    Draw_Text_D3D(text, 10, 30, D3DCOLOR_ARGB(255, 255, 255, 255));

    wsprintf(text, L"Player Level:%3d", gameobject.GetLevel());
    Draw_Text_D3D(text, 10, 55, D3DCOLOR_ARGB(255, 255, 255, 255));

    wsprintf(text, L"Player Exp:%3d", gameobject.GetHp());
    Draw_Text_D3D(text, 10, 80, D3DCOLOR_ARGB(255, 255, 255, 255));

    //두개의 클라이언트를키면 문제가 생길 수 있음
    if (KEY_DOWN(VK_RETURN)) {
      
    }
    unsigned short x, y;
    gameobject.GetPosition(x, y);
    D3DXVECTOR3 pos = D3DXVECTOR3((x - g_left_x) * 65.0f + 8,
        (y - g_top_y) * 65.0f + 8, 0.0);
    Draw_Text_D3D(chatArray_, static_cast<int>(pos.x - 10), static_cast<int>(pos.y - 10), D3DCOLOR_ARGB(255, 0, 0, 0));


    Draw_Text_D3D(chatArray_, 0, screen_height - 60, D3DCOLOR_ARGB(255, 0, 0, 255));
}






