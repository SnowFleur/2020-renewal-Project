#include "MonsterInputComponent.h"
#include"Monster.h"
#include"Protocol.h"

CMonsterInputComponent::CMonsterInputComponent() :state_{ MonsterState::MOVE } {}

void CMonsterInputComponent::State(CMonster& monster) {

    switch (state_) {
    case MonsterState::IDEL:

        /*AI를 깨웠다는 소리는 주위에 유저가 있다는 소리*/



        break;
    case MonsterState::ATTACK:
        /*
        자기 공격시야에 있으면 공격 없다면 다시 이동
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