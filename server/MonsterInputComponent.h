#pragma once
#include<stack>
#include"InputComponent.h"
#include"DataType.h"
#include"AStar.h"
class CMonster;
class CAstar;
class CNavigation;
class CPlayer;
class GameObject;

enum class MonsterState {
    IDEL=1,             //�⺻����
    ATTACK=2,           //���ݻ���
    MOVE=3,             //�̵�����
    RETURN_MOVE=4,      //�÷��̾ �þ߿��� ����� �ڱ� �ڸ��� ���ư��� ����
};

using Position=std::pair<PositionType, PositionType>;

class CMonsterInputComponent : public CInputComponent {
private:
    MonsterState            state_;
    std::stack<Position>    returnMoveStack_;
    CAstar*                 astarHandle_;
public:
    CMonsterInputComponent();
    ~CMonsterInputComponent()override;

    void Update(GameObject& gameobject)override {};
    void State(CMonster& monster, CPlayer& player);
    void StartPathFind(Astar::PairPositionType monster, Astar::PairPositionType player,CNavigation& navigation);
};
