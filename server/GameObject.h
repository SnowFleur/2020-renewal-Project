#pragma once
#include <unordered_set>
#include"DataType.h"
#include"OverEx.h"
#include"SRWLock.h"

class CInputComponent;
class CLockComponent;
class CSector;
struct EVENT_ST;

using ViewList = std::unordered_set<ObjectIDType>;

enum class ObjectState {
    IDEL = 1,				//기본상태
    SLEEP,					//잠들어 있는 상태
    ATTACK,					//공격상태
    MOVE,					//이동상태
    RETURN_MOVE,			//플레이어가 시야에서 사라져 자기 자리로 돌아가는 상태
};

/*
21.02.26: Aligned Data 구조체 패딩에 맞춰서 변수 위치 변경
*/
class CGameObject {
private:
    OverEx                  overEx_;
    PositionType            x_;                     // 2Byte
    PositionType            y_;                     // 2Byte
    AtomicHpType            hp_;                    // 1Byte
    AttackPowerType         attackPower_;           // 1Byte
    LevelType               level_;                 // 1Byte
    ExpType                 exp_;                   // 1Byte
    BoolType                isUsed_;                // 1Byte
    TextureDirection        characterDirection_;    // 1Byte+2Byte
    ObjectState             objectState_;           // 4Byte
    CInputComponent*        inputComponent_;        // 4Byte
public:
    CGameObject() = delete;
    CGameObject(const PositionType x, const PositionType y, const HpType hp, const LevelType level,
        const ExpType exp, const AttackPowerType attackPower,  CInputComponent* inputComponent);
    virtual ~CGameObject();
    CSRWLock                srwLock_;
public:
    void              GetPosition(PositionType& x, PositionType& y);
    LevelType         GetLevel()const;
    HpType            GetHp()const;
    ExpType           GetExp()const;
    bool              GetUsed()const;
    ObjectState       GetObjectState()const;
    PositionType      GetPositionX()const;
    PositionType      GetPositionY()const;
    OverEx&           GetOverEx();
    TextureDirection  GetObjectDirection()const;
    void              SetPosition(const PositionType x, const PositionType y);
    void              SetLevel(const LevelType level);
    void              SetHp(const HpType hp);
    void              SetExp(const ExpType exp);
    void              SetObjectDirection(const TextureDirection characterDirection);
    void              SetUsed(const bool used);
    void              SetObjectState(const ObjectState state);
    void              ProcessInputComponent(CSector& sector, EVENT_ST& ev);
#pragma region Only Used Player Function 
    virtual SOCKET       GetSocket()const;
    virtual char*        GetPacketBuffer();
    virtual ViewList&    GetViewList();
    virtual uInt32Type   GetPrevSize()const;
    virtual void         SetSocket(const SOCKET);
    virtual void         SetPrevSize(const uInt32Type);
#pragma endregion
};
