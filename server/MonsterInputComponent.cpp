#include "MonsterInputComponent.h"
#include"GameObject.h"
#include"Protocol.h"
#include"Navigation.h"
#include"LogCollector.h"
#include"NavigationHandle.h"
#include"Network.h"

CMonsterInputComponent::CMonsterInputComponent() :state_{ ObjectState::MOVE },
astarHandle_{ nullptr }, astarFlag_{ false }{
    astarHandle_ = new CAstar();
    CNavigationHandle::GetInstance();
}

CMonsterInputComponent::~CMonsterInputComponent() {
    if (astarHandle_ != nullptr) {
        delete astarHandle_;
    }
}

void CMonsterInputComponent::Update(CGameObject& gameobject) {}

void CMonsterInputComponent::SetMonsterState(const ObjectState state) {
    state_ = state;
}

ObjectState CMonsterInputComponent::GetMonsterState()const {
    return state_;
}

bool CMonsterInputComponent::CheckNearPlayer(CGameObject& monster, CGameObject& player) {
    PositionType mx{}, my{};
    PositionType px{}, py{};

    monster.GetPosition(mx, my);
    player.GetPosition(px, py);

    /*
    21.01.16
    ������ ���� �⺻ ������ ��, ��, ��, �� �� ���� �밢�� X
    P: Player, M: Monster
    000  000  0P0  000
    PM0  0MP  0M0  0M0
    000  000  000  0P0
    */
    
    /*
    ���� ������ Direction ���⿡ ���� ���⸸ ���ݰ��� ���� üũ
    �� �ʿ��ϰ� 4�� �ݺ��� ���°� ����
    */
    switch (monster.GetRenderCharacterDirection()) {
    case CHARACTER_DOWN: {
        if (mx == px && my + MAR::NORMAL_ATTACK == py) {
            return true;
        }
        return false;
    }
    case CHARACTER_LEFT: {
        if (my == py && mx - MAR::NORMAL_ATTACK == px) {
            return true;
        }
        return false;
    }
    case CHARACTER_RIGHT: {
        if (my == py && mx + MAR::NORMAL_ATTACK == px) {
            return true;
        }
        return false;
    }
    case CHARACTER_UP: {
        if (mx == px && my - MAR::NORMAL_ATTACK == py) {
            return true;
        }
        return false;
    }
    default:
        return false;
        break;
    }
}

void CMonsterInputComponent::State(CGameObject& monster, CGameObject& player) {

    switch (state_) {
    case ObjectState::IDEL:
        /*AI�� �����ٴ� �Ҹ��� ������ ������ �ִٴ� �Ҹ�*/
        break;
    case ObjectState::ATTACK:
        /*�ڱ� ���ݽþ߿� ������ ���� ���ٸ� �ٽ� �̵�*/


        //���� �ִ��� Check
        if (CheckNearPlayer(monster,player)) {

            //Player ü�°��� (atomic �ϰ� ����)
            player.SetHp(player.GetHp() - 1);

        }
        //������ �ٽ� Move
        else {
            state_ = ObjectState::MOVE;
        }
        break;
    case ObjectState::MOVE: {
        /*
        2020.11.14
        Index ���Ϳ��� �ѹ��� Event�� ���� �˾Ҵµ� Event�� ��� ����(Wakeup)�� ����
        �ش� �ڵ忡 �����尡 �� ���̻� �����ϰ� ������ �׷� ���� Data Race�� �߻��ϱ� ������
        AStar�� �������̶�� �����Ѵ�. 
        */
        if (astarFlag_ == false) {

            astarFlag_.store(true);

            PositionType mx{}, my{};
            PositionType px{}, py{};

            monster.GetPosition(mx, my);
            player.GetPosition(px, py);

            /*
            2021.01.16
            %15�� Map Data�� ���߱� ���ؼ� ��ǥ���� ���Ƿ� ���ΰ� ���߿� ����ȭ �ؾ��ϸ�
            ���� ��� �ش� �ּ� ����
            */
            StartPathFind(Astar::PairPosition{ mx% 15,my % 15 },
                Astar::PairPosition{ px % 15, py% 15 },
                CNavigationHandle::GetInstance()->navigation[0]);

            auto iter = astarHandle_->shortPath_.rbegin();

            //�÷��̾�� ��ħ ����
            if (px != std::get<0>(*iter) || py != std::get<1>(*iter)) {
                mx = std::get<0>(*iter);
                my = std::get<1>(*iter);
            }
            //�÷��̾� ��ó�� �����ߴ�.
            else {
                state_ = ObjectState::ATTACK;
            }
            monster.SetRenderCharacterDirection(std::get<2>(*iter));
            astarFlag_.store(false);
        }

        break;
    }
    case ObjectState::RETURN_MOVE:
        break;
    default:
        break;
    }
}

void CMonsterInputComponent::StartPathFind(Astar::PairPosition monster, Astar::PairPosition player, CNavigation& navigation) {
    astarHandle_->StartFindPath(monster, player, navigation);
}