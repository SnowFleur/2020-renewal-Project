#pragma once
#include<stack>
#include"InputComponent.h"
#include"DataType.h"
#include"AStar.h"

class CAstar;
class CNavigation;
class CGameObject;

enum class ObjectState;

//Monstet Attack Range
namespace MAR {
    //Noraml(Basic) Attack Range
    constexpr int NORMAL_ATTACK     = 1;
}

using Position=std::pair<PositionType, PositionType>;

class CMonsterInputComponent : public CInputComponent {
private:
    std::stack<Position>    returnMoveStack_;
    CAstar*                 astarHandle_;
    AtomicBool              astarFlag_;

    bool CheckNearPlayer(CGameObject& myObject, CGameObject& targetObject);
    void StartPathFind(Astar::PairPosition monster, Astar::PairPosition player,CNavigation& navigation);
public:
    CMonsterInputComponent();
    ~CMonsterInputComponent()override;
    void ExcuteEvent(CGameObject& myObject,CGameObject& targetObject)override;
};


