#include "MonsterInputComponent.h"
#include"Monster.h"
#include"Protocol.h"
#include"Navigation.h"
#include"Player.h"
#include"LogCollector.h"
#include"NavigationHandle.h"
#include"Network.h"

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

void CMonsterInputComponent::Update(GameObject& gameobject) {}

void CMonsterInputComponent::SetMonsterState(const MonsterState state) {
    state_ = state;
}

MonsterState CMonsterInputComponent::GetMonsterState()const {
    return state_;
}

bool CMonsterInputComponent::CheckNearPlayer(CMonster& monster, CPlayer& player) {

    //��,��
    if (monster.y_ == player.y_) {
        if (monster.x_ == player.x_)return true;
    }
    //��,��
    else if (monster.x_ == player.x_) {
        if (monster.y_ == player.y_)return true;
    }
    else {
        return false;
    }
}

void CMonsterInputComponent::State(CMonster& monster, CPlayer& player) {

    switch (state_) {
    case MonsterState::IDEL:
        /*AI�� �����ٴ� �Ҹ��� ������ ������ �ִٴ� �Ҹ�*/
        break;
    case MonsterState::ATTACK:
        /*�ڱ� ���ݽþ߿� ������ ���� ���ٸ� �ٽ� �̵�*/

        //���� �ִ��� Check
        if (CheckNearPlayer(monster,player)) {

            //Player ü�°��� (atomic �ϰ� ����)
            player.hp_--;


            //Packet ���� �ֺ��� �ִ� ��� �÷��̾�����
            for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                // GetNear �Լ��� ����...
                // All �� ����� ����

                NETWORK::SendHitObject(player.socket_, player.hp_, i, OBJECT_DEFINDS::OTHER_PLAYER);
            }
        }
        //������ �ٽ� Move
        else {
            state_ = MonsterState::MOVE;
        }
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

            StartPathFind(Astar::PairPosition{ monster.x_ % 15,monster.y_ % 15 },
                Astar::PairPosition{ player.x_ % 15,player.y_ % 15 },
                CNavigationHandle::GetInstance()->navigation[0]);

            auto iter = astarHandle_->shortPath_.rbegin();

            //�÷��̾�� ��ħ ����
            if (player.x_ != std::get<0>(*iter) || player.y_ != std::get<1>(*iter)) {
                monster.x_ = std::get<0>(*iter);
                monster.y_ = std::get<1>(*iter);
            }
            //�÷��̾� ��ó�� �����ߴ�.
            else {
                state_ = MonsterState::ATTACK;
            }
            monster.diretion_ = std::get<2>(*iter);
            astarFlag_.store(false);
        }
     
        break;
    }
    case MonsterState::RETURN_MOVE:

        break;
    default:
        break;
    }
}

void CMonsterInputComponent::StartPathFind(Astar::PairPosition monster, Astar::PairPosition player, CNavigation& navigation) {
    astarHandle_->StartFindPath(monster, player, navigation);
}