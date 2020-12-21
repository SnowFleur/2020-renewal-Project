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

//Monstet Attack Range
namespace MAR {
    //Noraml(Basic) Attack Range
    constexpr int NORMAL_ATTACK     = 1;
}

enum class MonsterState {
    IDEL=1,             //기본상태
    SLEEP,              //잠들어 있는 상태
    ATTACK,             //공격상태
    MOVE,               //이동상태
    RETURN_MOVE,        //플레이어가 시야에서 사라져 자기 자리로 돌아가는 상태
};

using Position=std::pair<PositionType, PositionType>;

class CMonsterInputComponent : public CInputComponent {
private:
    MonsterState            state_;
    std::stack<Position>    returnMoveStack_;
    CAstar*                 astarHandle_;
    AtomicBool              astarFlag_;

    bool CheckNearPlayer(CMonster& monster, CPlayer& player);
    void StartPathFind(Astar::PairPosition monster, Astar::PairPosition player,CNavigation& navigation);
public:
    CMonsterInputComponent();
    ~CMonsterInputComponent()override;

    void Update(GameObject& gameobject)override;
    void SetMonsterState(const MonsterState state);
    MonsterState GetMonsterState()const;
    void State(CMonster& monster, CPlayer& player);
};
