#include"MonsterObject.h"
#include"InputComponent.h"
#include"GraphicsComponent.h"
#include"..\server\Protocol.h"

CMonsterObject::CMonsterObject(CInputComponent* input, CGraphicsComponent* graphics)
    :x_{ 0 }, y_{ 0 }, level_{ 0 }, hp_{ 0 }, exp_{ 0 }, id_{ 0 },
    inputHandle_{ input }, graphicsHandle_{ graphics }, isused_{ false } {}

CMonsterObject::~CMonsterObject() {
    if (inputHandle_ != nullptr) {
        delete inputHandle_;
    }

    if (graphicsHandle_ != nullptr) {
        delete graphicsHandle_;
    }
}

void CMonsterObject::Update(CWorld& world) {
    inputHandle_->Update(*this, world);
    graphicsHandle_->Update(*this);
}

void CMonsterObject::InitGameObject(void* packet) {

    sc_packet_login_ok* data =
        reinterpret_cast<sc_packet_login_ok*>(packet);

    if (data != nullptr) {
        id_ = data->id;
        x_ = data->x;
        y_ = data->y;
       // level_ = data->LEVEL;
       // hp_ = data->HP;
       /// exp_ = data->EXP;
    }
}




#pragma region GetFunctions
ObjectIDType CMonsterObject::Getid()const {
    return id_;
}

void CMonsterObject::GetPosition(unsigned short& x, unsigned short& y) {
    x = x_;
    y = y_;
}
int CMonsterObject::GetLevel()const {
    return level_;
}

int CMonsterObject::GetHp()const {
    return hp_;
}

int CMonsterObject::GetExp()const {
    return exp_;
}

bool CMonsterObject::GetUsed()const {
    return isused_;
}

#pragma endregion

#pragma region Set Functions

void CMonsterObject::SetRenderCharacterDirection(unsigned char renderCharacterDirection) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetRenderCharacterDirection(renderCharacterDirection);
}

void CMonsterObject::SetVisibility(VISIBILITY_OPTION option) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetVisibility(option);
}

void CMonsterObject::SetCharacterTexture(const wchar_t* fileName, const int width, const int height) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetCharacterTexture(fileName, width, height);
}

void CMonsterObject::SetCharacterTexture(void* texture, const int cycle) {
    //Grpahics Componect에 넣기
    graphicsHandle_->SetCharacterTexture(texture, cycle);
}

void CMonsterObject::SetPosition(const unsigned short x, const unsigned short y) {
    if (x >= 0 || y >= 0) {
        x_ = x;
        y_ = y;
    }
}

void CMonsterObject::SetLevel(const int level) {
    level_ = level;
}

void CMonsterObject::SetHp(const int hp) {
    hp_ = hp;
}

void CMonsterObject::SetExp(const int exp) {
    exp_ = exp;
}

void CMonsterObject::SetUsed(const bool used) {
    isused_ = used;
}
#pragma endregion


