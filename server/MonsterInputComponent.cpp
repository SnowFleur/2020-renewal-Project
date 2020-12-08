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

        /*AI�� �����ٴ� �Ҹ��� ������ ������ �ִٴ� �Ҹ�*/


        break;
    case MonsterState::ATTACK:
        /*
        �ڱ� ���ݽþ߿� ������ ���� ���ٸ� �ٽ� �̵�
        */
        break;
    case MonsterState::MOVE: {

        /*
        2020.11.14
        Index ���Ϳ��� �ѹ��� Event�� ���� �˾Ҵµ� Event�� ��� ����(Wakeup)�� ����
        �ش� �ڵ忡 �����尡 �� ���̻� �����ϰ� ������ �׷� ���� Data Race�� �߻��ϱ� ������
        AStar�� �������̶�� �����Ѵ�. 
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

        //�÷��̾� ��ó�� �����ߴ�.
        else {
          //  std::cout << "�÷��̾� ��ó�� ����\n";
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