#include "NpcObject.h"
#include"InputComponent.h"
#include"GraphicsComponent.h"
#include"..\server\Protocol.h"


CNpcObject::CNpcObject(CInputComponent* input, CGraphicsComponent* graphics)
    :x_{ 0 }, y_{ 0 }, id_{ 0 },
    inputHandle_{ input }, graphicsHandle_{ graphics } {}

CNpcObject::~CNpcObject() {
    if (inputHandle_ != nullptr) {
        delete inputHandle_;
    }

    if (graphicsHandle_ != nullptr) {
        delete graphicsHandle_;
    }
}
void CNpcObject::Update(CWorld& world) {
    inputHandle_->Update(*this, world);
    graphicsHandle_->Update(*this);
}

void CNpcObject::InitGameObject(void* packet) {

    sc_packet_login_ok* data =
        reinterpret_cast<sc_packet_login_ok*>(packet);

    if (data != nullptr) {
        id_ = data->id;
        x_ = data->x;
        y_ = data->y;
    }
}

#pragma region GetFunctions
ObjectIDType CNpcObject::Getid()const {
    return id_;
}

void CNpcObject::GetPosition(unsigned short& x, unsigned short& y) {
    x = x_;
    y = y_;
}
/*확장성을 위해 납뒀지만 좋은 코드는 아니다*/
int CNpcObject::GetLevel()const { return 0; }
int CNpcObject::GetHp()const { return 0; }
int CNpcObject::GetExp()const { return 0; }
#pragma endregion

#pragma region Set Functions

void CNpcObject::SetRenderCharacterDirection(unsigned char renderCharacterDirection) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetRenderCharacterDirection(renderCharacterDirection);
}


void CNpcObject::SetVisibility(VISIBILITY_OPTION option) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetVisibility(option);
}

void CNpcObject::SetCharacterTexture(const wchar_t* fileName, const int width, const int height) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetCharacterTexture(fileName, width, height);
}

void CNpcObject::SetCharacterTexture(void* texture, const int cycle) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetCharacterTexture(texture, cycle);
}



void CNpcObject::SetPosition(const unsigned short x, const unsigned short y) {
    if (x >= 0 || y >= 0) {
        x_ = x;
        y_ = y;
    }
}

void  CNpcObject::SetNpcType(NPC_TYPE type) {
    npcType_ = type;
}

void CNpcObject::SetLevel(const int level) {}
void CNpcObject::SetHp(const int hp) {}
void CNpcObject::SetExp(const int exp) {}

#pragma endregion

