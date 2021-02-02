#pragma once
#include <unordered_set>
#include"DataType.h"
#include"OverEx.h"
#include"SRWLock.h"

/*
���Ͱ� ������ ��ü�� ����°� �ƴ϶� ��Ȱ���ؼ� ����� ���̱� ������ 
�������, �̵������� �� �����.
*/
class CInputComponent;
class CLockComponent;

using ViewList = std::unordered_set<ObjectIDType>;

enum class ObjectState {
    IDEL = 1,				//�⺻����
    SLEEP,					//���� �ִ� ����
    ATTACK,					//���ݻ���
    MOVE,					//�̵�����
    RETURN_MOVE,			//�÷��̾ �þ߿��� ����� �ڱ� �ڸ��� ���ư��� ����
};

/*
21.01.27 �ν��ٽ��� �� �� �ִ� ������ �ִٸ� ����
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
    TextureDirection        characterDirection_;    // ���߿� �ٸ������� �� �� �ִ��� ����
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
