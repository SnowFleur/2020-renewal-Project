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

enum class MonsterState;

//Monstet Attack Range
namespace MAR {
    //Noraml(Basic) Attack Range
    constexpr int NORMAL_ATTACK     = 1;
}

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


