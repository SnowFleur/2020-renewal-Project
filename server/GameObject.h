#pragma once
#include <unordered_set>
#include"DataType.h"
#include"OverEx.h"
#include"SRWLock.h"

/*
몬스터가 죽으면 객체를 지우는게 아니라 재활용해서 사용할 것이기 때문에 
복사생성, 이동생성을 다 지운다.
*/
class CInputComponent;
class CLockComponent;

using ViewList = std::unordered_set<ObjectIDType>;

enum class ObjectState {
    IDEL = 1,				//기본상태
    SLEEP,					//잠들어 있는 상태
    ATTACK,					//공격상태
    MOVE,					//이동상태
    RETURN_MOVE,			//플레이어가 시야에서 사라져 자기 자리로 돌아가는 상태
};

/*
21.01.27 인스텐스로 뺄 수 있는 변수가 있다면 빼기
*/
class CGameObject {
private:
    OverEx                  overEx_;
    PositionType            x_;
    PositionType            y_;
    AtomicHpType            hp_;
    AttackPowerType         attackPower_;
    LevelType               level_;
    ExpType                 exp_;
    BoolType                isUsed_;
    ObjectState             objectState_;
    TextureDirection        characterDirection_;    // 나중에 다른곳으로 뺄 수 있는지 볼것
    CInputComponent*        inputComponent_;
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
    TextureDirection  GetRenderCharacterDirection()const;
    void              SetPosition(const PositionType x, const PositionType y);
    void              SetLevel(const LevelType level);
    void              SetHp(const HpType hp);
    void              SetExp(const ExpType exp);
    void              SetRenderCharacterDirection(const TextureDirection characterDirection);
    void              SetUsed(const bool used);
    void              SetObjectState(const ObjectState state);
    void              ProcessInputComponent(CGameObject& gameobjects);
#pragma region Only Used Player Function 
    virtual SOCKET       GetSocket()const;
    virtual char*        GetPacketBuffer();
    virtual ViewList&    GetViewList();
    virtual uInt32Type   GetPrevSize()const;
    virtual void         SetSocket(const SOCKET);
    virtual void         SetPrevSize(const uInt32Type);
#pragma endregion
};
