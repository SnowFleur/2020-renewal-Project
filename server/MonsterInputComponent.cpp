#include "MonsterInputComponent.h"
#include"Monster.h"
#include"Protocol.h"
#include"Navigation.h"
#include"AStar.h"

CMonsterInputComponent::CMonsterInputComponent() :state_{ MonsterState::MOVE } {
    astarHandle_ = new CAstar();

}

void CMonsterInputComponent::State(CMonster& monster) {

    switch (state_) {
    case MonsterState::IDEL:

        /*AI�� �����ٴ� �Ҹ��� ������ ������ �ִٴ� �Ҹ�*/



        break;
    case MonsterState::ATTACK:
        /*
        �ڱ� ���ݽþ߿� ������ ���� ���ٸ� �ٽ� �̵�
        */

        break;
    case MonsterState::MOVE:
        monster.x_ += 1;
        monster.diretion_ = OBJECT_DEFINDS::CHARACTER_RIGHT;
        break;
    case MonsterState::RETURN_MOVE:

        break;
    default:
        break;
    }
}

void CMonsterInputComponent::StartPathFind(CNavigation& navigation) {



}