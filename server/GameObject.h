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

class CGameObject {
private:
    OverEx                  overEx_;
    PositionType            x_;
    PositionType            y_;
    AtomicHpType            hp_;
    DamageType              damage_;
    LevelType               level_;
    ExpType                 exp_;
    BoolType                isUsed_;
    ObjectState             objectState_;
    CInputComponent*        inputComponent_;
public:
    virtual ~CGameObject() = 0;
    CSRWLock                srwLock_;
public:
    void                  GetPosition(PositionType& x, PositionType& y);
    inline LevelType      GetLevel()const;
    inline HpType         GetHp()const;
    inline ExpType        GetExp()const;
    inline bool           GetUsed()const;
    inline ObjectState    GetObjectState()const;
    inline PositionType   GetPositionX()const;
    inline PositionType   GetPositionY()const;
    inline OverEx&        GetOverEx();

    inline void           SetPosition(const PositionType x, const PositionType y);
    inline void           SetLevel(const LevelType level);
    inline void           SetHp(const HpType hp);
    inline void           SetExp(const ExpType exp);
    inline void           SetRenderCharacterDirection(const TextureDirection renderCharacterDirection);
    inline void           SetUsed(const bool used);
    inline void           SetObjectState(const ObjectState state);

#pragma region Only Used Player Function 
    inline virtual SOCKET    GetSocket()const {};
    inline virtual void      SetSocket(const SOCKET) {};
    inline virtual char*     GetPacketBuffer() {};
    inline virtual ViewList& GetViewList(){};
    inline virtual uIntType  GetPrevSize()const {};
#pragma endregion
};
