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
    IDEL=1,             //기본상태
    ATTACK=2,           //공격상태
    MOVE=3,             //이동상태
    RETURN_MOVE=4,      //플레이어가 시야에서 사라져 자기 자리로 돌아가는 상태
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
