#include "PlayerObject.h"
#include"Sector.h"
#include"InputComponent.h"
#include"GraphicsComponent.h"
#include"..\server\Protocol.h"

CPlayerObject::CPlayerObject(CInputComponent* input, CGraphicsComponent* graphics)
    :x_{ 5 }, y_{ 5 }, level_{ 1 }, hp_{ 100 }, exp_{ 0 }, id_{OBJECT_DEFINDS::NOT_USED_OBJID},
    inputHandle_{ input }, graphicsHandle_{ graphics }, isused_{ false } {}

CPlayerObject::~CPlayerObject() {
    if (inputHandle_ != nullptr) {
        delete inputHandle_;
    }

    if (graphicsHandle_ != nullptr) {
        delete graphicsHandle_;
    }
}

/*
상속의 단점을 알 수 있는 코드 20.11.01
*/
void CPlayerObject::InitGameObject(void* packet) {

    sc_packet_login_ok* data =
        reinterpret_cast<sc_packet_login_ok*>(packet);

    if (data != nullptr) {
        id_ = data->id;
        x_ = data->x;
        y_ = data->y;
       // level_ = data->LEVEL;
       // hp_ = data->HP;
       // exp_ = data->EXP;
    }
}

void CPlayerObject::Update(CWorld& world) {
    inputHandle_->Update(*this, world);
    graphicsHandle_->Update(*this);

}

#pragma region GetFunctions
ObjectIDType CPlayerObject::Getid()const {
    return id_;
}

void CPlayerObject::GetPosition(unsigned short& x, unsigned short& y) {
    x = x_;
    y = y_;
}
int CPlayerObject::GetLevel()const {
    return level_;
}

int CPlayerObject::GetHp()const {
    return hp_;
}

int CPlayerObject::GetExp()const {
    return exp_;
}

bool CPlayerObject::GetUsed()const {
    return isused_;
}

#pragma endregion

#pragma region Set Functions

void CPlayerObject::SetRenderCharacterDirection(unsigned char renderCharacterDirection) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetRenderCharacterDirection(renderCharacterDirection);
}

void CPlayerObject::SetVisibility(VISIBILITY_OPTION option) {
    graphicsHandle_->SetVisibility(option);
}

void  CPlayerObject::SetCharacterTexture(const wchar_t* fileName, const int width, const int height) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetCharacterTexture(fileName, width, height);
}

void CPlayerObject::SetCharacterTexture(void* texture, int const cycle) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetCharacterTexture(texture, cycle);
}

void CPlayerObject::SetPosition(const unsigned short x, const unsigned short y) {
    if (x >= 0 || y >= 0) {
        x_ = x;
        y_ = y;
    }
}

void CPlayerObject::SetLevel(const int level) {
    level_ = level;
}

void CPlayerObject::SetHp(const int hp) {
    hp_ = hp;
}

void CPlayerObject::SetExp(const int exp) {
    exp_ = exp;
}

void CPlayerObject::SetUsed(const bool used) {
    isused_ = used;
}
#pragma endregion


