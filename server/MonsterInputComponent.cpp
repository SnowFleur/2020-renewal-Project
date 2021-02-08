#include "MonsterInputComponent.h"
#include"GameObject.h"
#include"Protocol.h"
#include"Navigation.h"
#include"LogCollector.h"
#include"NavigationHandle.h"
#include"Network.h"

CMonsterInputComponent::CMonsterInputComponent() :astarHandle_{ nullptr }, astarFlag_{ false }{
    astarHandle_ = new CAstar();
    CNavigationHandle::GetInstance();
}

CMonsterInputComponent::~CMonsterInputComponent() {
    if (astarHandle_ != nullptr) {
        delete astarHandle_;
    }
}

/*
MyObject:       Monster(����)
targetObject:   targetObject(���, �ַ� Player ����)
*/
void CMonsterInputComponent::ExcuteEvent(CGameObject& myObject, CGameObject& targetObject) {

    switch (myObject.GetObjectState()) {
    case ObjectState::IDEL:
        /*AI�� �����ٴ� �Ҹ��� ������ ������ �ִٴ� �Ҹ�*/
        break;
    case ObjectState::ATTACK:
        /*�ڱ� ���ݽþ߿� ������ ���� ���ٸ� �ٽ� �̵�*/

        //���� �ִ��� Check
        if (CheckNearPlayer(myObject, targetObject)) {
            //tarGet ü�°��� (atomic �ϰ� ����)
            targetObject.SetHp(targetObject.GetHp() - 1);
        }
        //������ �ٽ� Move
        else {
            myObject.SetObjectState(ObjectState::MOVE);
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

            myObject.GetPosition(mx, my);
            targetObject.GetPosition(px, py);
            //�ǵ��ư� ��Ʈ ���� ����
            returnMoveStack_.push(Position{ mx,my });
            /*
            2021.01.16
            %15�� Map Data�� ���߱� ���ؼ� ��ǥ���� ���Ƿ� ���ΰ� ���߿� ����ȭ �ؾ��ϸ�
            ���� ��� �ش� �ּ� ����
            */

            if (StartPathFind(Astar::PairPosition{ mx,my }, Astar::PairPosition{ px,py },
                CNavigationHandle::GetInstance()->navigation[0]) == true) {

                auto iter = astarHandle_->shortPath_.rbegin();

                //�÷��̾�� ��ħ ����
                if (px != std::get<0>(*iter) || py != std::get<1>(*iter)) {
                    mx = std::get<0>(*iter);
                    my = std::get<1>(*iter);
                }
                //�÷��̾� ��ó�� �����ߴ�.
                else {
                    myObject.SetObjectState(ObjectState::ATTACK);
                }
                myObject.SetObjectDirection(std::get<2>(*iter));
                myObject.SetPosition(mx, my);
            }
            else {
                std::cout << "Not Found Target\n";
            }

            astarFlag_.store(false);
        }
        break;
    }
    case ObjectState::RETURN_MOVE:
        //�̵��� �� ���� ����Ǵ� ���������� �� �ٲܰ�
        std::cout << "Return Move\n";

        break;
    default:
        break;
    }

}

bool CMonsterInputComponent::CheckNearPlayer(CGameObject& myObject, CGameObject& targetObject) {
    PositionType mx{}, my{};
    PositionType px{}, py{};

    myObject.GetPosition(mx, my);
    targetObject.GetPosition(px, py);

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
    switch (myObject.GetObjectDirection()) {
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

bool CMonsterInputComponent::StartPathFind(Astar::PairPosition monster, Astar::PairPosition player, CNavigation& navigation) {
    return astarHandle_->StartFindPath(monster, player, navigation);
}