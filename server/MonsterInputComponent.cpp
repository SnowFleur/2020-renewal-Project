#include "MonsterInputComponent.h"
#include"Monster.h"
#include"Protocol.h"
#include"Navigation.h"
#include"Player.h"
#include"LogCollector.h"
#include"NavigationHandle.h"


CMonsterInputComponent::CMonsterInputComponent() :state_{ MonsterState::MOVE },
astarHandle_{ nullptr }, astarFlag_{ false }{
    astarHandle_ = new CAstar();
    CNavigationHandle::GetInstance();
}

CMonsterInputComponent::~CMonsterInputComponent() {
    if (astarHandle_ != nullptr) {
        delete astarHandle_;
    }
}

void CMonsterInputComponent::State(CMonster& monster, CPlayer& player) {

    switch (state_) {
    case MonsterState::IDEL:

        /*AI를 깨웠다는 소리는 주위에 유저가 있다는 소리*/


        break;
    case MonsterState::ATTACK:
        /*
        자기 공격시야에 있으면 공격 없다면 다시 이동
        */
        break;
    case MonsterState::MOVE: {

        /*
        2020.11.14
        Index 몬스터에는 한번만 Event가 들어갈줄 알았는데 Event가 계속 들어옴(Wakeup)에 따라
        해당 코드에 스레드가 두 개이상 접근하게 됐으며 그로 인해 Data Race가 발생하기 때문에
        AStar가 진행중이라면 무시한다. 
        */
        if (astarFlag_ == false) {

            astarFlag_.store(true);

            StartPathFind(Astar::PairPositionType{ monster.x_ % 15,monster.y_ % 15 },
                Astar::PairPositionType{ player.x_ % 15,player.y_ % 15 },
                CNavigationHandle::GetInstance()->navigation[0]);

            auto iter = astarHandle_->shortPath_.rbegin();
            if (player.x_ != iter->first || player.y_ != iter->second) {
                monster.x_ = iter->first;
                monster.y_ = iter->second;
            }
            astarFlag_.store(false);
        }

        //플레이어 근처에 도착했다.
        else {
          //  std::cout << "플레이어 근처에 도착\n";
         //   state_ == MonsterState::ATTACK;
        }
        break;
    }
    case MonsterState::RETURN_MOVE:

        break;
    default:
        break;
    }
}

void CMonsterInputComponent::StartPathFind(Astar::PairPositionType monster, Astar::PairPositionType player, CNavigation& navigation) {
    astarHandle_->StartFindPath(monster, player, navigation);
}