#include"ObjectTextureCreater.h"


BOB* CObjectTextureCreater::SetGameObjectTextre(const wchar_t* fileName,
    const int width, const int height, const int cycle) {

    BOB* texture = new BOB[cycle];

    Load_Texture(fileName, UNIT_TEXTURE, width, height);

    for (int i = 0; i < cycle; ++i) {
        if (!Create_BOB32(&texture[i], 0, 0, 70, height, 0, BOB_ATTR_SINGLE_FRAME)) {
            //std::cout << "Fail Create_BOB32" << std::endl;
        }
        //Frame은 출력할 이미지 칸
        Load_Frame_BOB32(&texture[i], UNIT_TEXTURE, 0, i, 0, BITMAP_EXTRACT_MODE_CELL);
        texture[i].attr |= BOB_ATTR_VISIBLE;
    }
    return texture;
}