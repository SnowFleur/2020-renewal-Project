#pragma once
#include<stack>
#include"DataType.h"
#include"AStar.h"

class CMonster;
class CAstar;
class CNavigation;
class CPlayer;

enum class MonsterState {
    IDEL=1,             //�⺻����
    ATTACK=2,           //���ݻ���
    MOVE=3,             //�̵�����
    RETURN_MOVE=4,      //�÷��̾ �þ߿��� ����� �ڱ� �ڸ��� ���ư��� ����
};

using Position=std::pair<PositionType, PositionType>;

class CMonsterInputComponent{
private:
    MonsterState            state_;
    std::stack<Position>    returnMoveStack_;
    CAstar*                 astarHandle_;
public:
    CMonsterInputComponent();
    ~CMonsterInputComponent();

    void State(CMonster& monster, CPlayer& player);
    void StartPathFind(Astar::PairPositionType monster, Astar::PairPositionType player,CNavigation& navigation);
};
