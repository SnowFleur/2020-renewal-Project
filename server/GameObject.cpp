#include"GameObject.h"
#include"LogCollector.h"

CGameObject::~CGameObject() {

}

void CGameObject::GetPosition(PositionType& x, PositionType& y) {
    x = x_;
    y = y_;
}

LevelType CGameObject::GetLevel()const {
    return level_;
}

HpType CGameObject::GetHp()const {
    return hp_;
}

ExpType CGameObject::GetExp()const {
    return exp_;
}

bool CGameObject::GetUsed()const {
    return isUsed_;
}

ObjectState CGameObject::GetObjectState()const {
    return objectState_;
}

PositionType CGameObject::GetPositionX()const {
    return x_;
}

PositionType CGameObject::GetPositionY()const {
    return y_;
}

OverEx& CGameObject::GetOverEx() {
    return overEx_;
}

TextureDirection CGameObject::GetRenderCharacterDirection()const {
    return characterDirection_;
}
//*******************
//Set Functions
//*******************

void CGameObject::SetPosition(const PositionType x, const PositionType y) {
    x_ = x;
    y_ = y;
}

void CGameObject::SetLevel(const LevelType level) {
    level_ = level;
}

void CGameObject::SetHp(const HpType hp) {
    hp_ = hp;
}

void CGameObject::SetExp(const ExpType exp) {
    exp_ = exp;
}

void CGameObject::SetRenderCharacterDirection(const TextureDirection characterDirection) {
    characterDirection_ = characterDirection;
}

void CGameObject::SetUsed(bool used) {
    isUsed_ = used;
}

void CGameObject::SetObjectState(const ObjectState state) {
    objectState_ = state;
}


#pragma region Only Used Player Function 
SOCKET CGameObject::GetSocket()const {
    CLogCollector::GetInstance()->PrintLog("Error Not Overriding GetSocket Function");
    return 0;
}

char* CGameObject::GetPacketBuffer() {
    CLogCollector::GetInstance()->PrintLog("Error Not Overriding GetPacketBuffer Function");
    return nullptr;
}
ViewList& CGameObject::GetViewList() {
    CLogCollector::GetInstance()->PrintLog("Error Not Overriding GetViewList Function");
    ViewList v{};
    return v;
}

uInt32Type CGameObject::GetPrevSize()const {
    CLogCollector::GetInstance()->PrintLog("Error Not Overriding GetPrevSize Function");
    return 0;
}

void CGameObject::SetSocket(const SOCKET) {}
void CGameObject::SetPrevSize(const uInt32Type) {}
#pragma endregion