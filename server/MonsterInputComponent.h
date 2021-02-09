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

class CMonsterInputComponent : public CInputComponent {
private:
    std::stack<Astar::TuplePosition>    returnMoveStack_;
    CAstar*                             astarHandle_;
    AtomicBool                          astarFlag_;

    bool CheckNearPlayer(CGameObject& myObject, CGameObject& targetObject);
    bool StartPathFind(Astar::PairPosition monster, Astar::PairPosition player,CNavigation& navigation);
    int  ReverseTexutre(int texture);
public:
    CMonsterInputComponent();
    ~CMonsterInputComponent()override;
    void ExcuteEvent(CSector& sector, EVENT_ST& ev)override;
};


