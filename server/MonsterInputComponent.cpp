#include "MonsterInputComponent.h"
#include"Sector.h"
#include"GameObject.h"
#include"Protocol.h"
#include"Navigation.h"
#include"LogCollector.h"
#include"NavigationHandle.h"
#include"Network.h"
#include"TimerQueue.h"

CMonsterInputComponent::CMonsterInputComponent() :astarHandle_{ nullptr }, astarFlag_{ false }{
    astarHandle_ = new CAstar();
    CNavigationHandle::GetInstance();
}

CMonsterInputComponent::~CMonsterInputComponent() {
    if (astarHandle_ != nullptr) {
        delete astarHandle_;
    }
}

void CMonsterInputComponent::ExcuteEvent(CSector& sector, EVENT_ST& ev) {

    ObjectIDType npc_id = ev.obj_id;
    ObjectIDType player_id = ev.target_id;

    CGameObject& myObject = *sector.gameobjects_[npc_id];
    CGameObject& targetObject = *sector.gameobjects_[player_id];

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

            //���� ����, ���ݹ��� Ŭ���� HP, ���ݹ��� ID,  ������ ID
            for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                //������� Ŭ���̾�Ʈ��
                if (sector.gameobjects_[i]->GetUsed() == false)continue;
                NETWORK::SendHitObject(sector.gameobjects_[i]->GetSocket(),
                    sector.gameobjects_[player_id]->GetHp(), player_id, sector.gameobjects_[npc_id]->GetObjectDirection(), npc_id);
            }
        }
        //������ �ٽ� Move
        else {
            myObject.SetObjectState(ObjectState::MOVE);
        }

        break;
    case ObjectState::MOVE: {

        //�̵��ϱ� �� �� ����
        std::unordered_set<ObjectIDType>oldView;
        PositionType x, y;
        myObject.GetPosition(x, y);

        //���� x,y�� ������� cell�� �ִ� Object�� ID ���� view�� �־��ش�.
        sector.GetViewListByCell(oldView, x, y);

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
            returnMoveStack_.push(Astar::TuplePosition{ mx,my,myObject.GetObjectDirection() });
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
                myObject.SetObjectState(ObjectState::RETURN_MOVE);
            }
            astarFlag_.store(false);
        }

        //�̵� �� ó��
        std::unordered_set<ObjectIDType>newView;

        myObject.GetPosition(x, y);

        //���� x,y�� ������� cell�� �ִ� Object�� ID ���� view�� �־��ش�.
        sector.GetViewListByCell(newView, x, y);

        //�̵� �� ViewList
        for (auto iter : oldView) {

            //User�� �̵��� ����Ʈ�� �̵��� ����Ʈ�� �����Ѵ�..
            if (newView.count(iter) != 0) {

                //���� User�� viewlist�� npc_id�� �ִ��� Ȯ��
                //������ npc������ �ش� User���� ������.
                sector.gameobjects_[iter]->srwLock_.Readlock();
                if (sector.gameobjects_[iter]->GetViewList().find(npc_id) != sector.gameobjects_[iter]->GetViewList().end()) {
                    sector.gameobjects_[iter]->srwLock_.Readunlock();

                    NETWORK::SendMoveObject(sector.gameobjects_[iter]->GetSocket(),
                        sector.gameobjects_[npc_id]->GetPositionX(), sector.gameobjects_[npc_id]->GetPositionY(),
                        npc_id, sector.gameobjects_[npc_id]->GetObjectDirection());
                }

                //���� User�� viewIst�� npc_id�� ����.
                //���� ������ npc�� ������ User���� ������.
                else {
                    sector.gameobjects_[iter]->srwLock_.Readunlock();
                    sector.gameobjects_[iter]->srwLock_.Writelock();
                    sector.gameobjects_[iter]->GetViewList().insert(npc_id);
                    sector.gameobjects_[iter]->srwLock_.Writeunlock();

                    NETWORK::SendAddObject(sector.gameobjects_[iter]->GetSocket(),
                        sector.gameobjects_[npc_id]->GetPositionX(), sector.gameobjects_[npc_id]->GetPositionY(), npc_id);
                }

            }

            //User�� �̵� �� ����Ʈ�� ����.
            else {

                //�ش� user�� ���� npc_id�� ������
                //�����. 
                //������ �ִ°� �ƴ϶� �־������̱� ����
                sector.gameobjects_[iter]->srwLock_.Readlock();
                if (sector.gameobjects_[iter]->GetViewList().find(npc_id) != sector.gameobjects_[iter]->GetViewList().end()) {
                    sector.gameobjects_[iter]->srwLock_.Readunlock();

                    sector.gameobjects_[iter]->srwLock_.Writelock();
                    sector.gameobjects_[iter]->GetViewList().erase(npc_id);
                    sector.gameobjects_[iter]->srwLock_.Writeunlock();

                    NETWORK::SendRemoveObject(sector.gameobjects_[iter]->GetSocket(), npc_id);
                }
                else {
                    sector.gameobjects_[iter]->srwLock_.Readunlock();
                }

            }
        }

        //�̵� �� ����Ʈ�� �ִ� ������ NPC
        for (auto iter : newView) {
            //�̵� ���� ����Ʈ �������� �ش� npc_id�� ����.
            if (oldView.find(npc_id) == oldView.end()) {

                //�̵� �� ����Ʈ���� ���� NPC�� ������ ����.
                sector.gameobjects_[iter]->srwLock_.Readlock();
                if (sector.gameobjects_[iter]->GetViewList().find(npc_id) == sector.gameobjects_[iter]->GetViewList().end()) {
                    sector.gameobjects_[iter]->srwLock_.Readunlock();

                    sector.gameobjects_[iter]->srwLock_.Writelock();
                    sector.gameobjects_[iter]->GetViewList().insert(npc_id);
                    sector.gameobjects_[iter]->srwLock_.Writeunlock();

                    NETWORK::SendAddObject(sector.gameobjects_[iter]->GetSocket(),
                        sector.gameobjects_[npc_id]->GetPositionX(), sector.gameobjects_[npc_id]->GetPositionY(), npc_id);
                }

                else {
                    sector.gameobjects_[iter]->srwLock_.Readunlock();
                    NETWORK::SendMoveObject(sector.gameobjects_[iter]->GetSocket(),
                        sector.gameobjects_[npc_id]->GetPositionX(), sector.gameobjects_[npc_id]->GetPositionY(),
                        npc_id, sector.gameobjects_[npc_id]->GetObjectDirection());
                }
            }
        }
        break;
    }
    case ObjectState::RETURN_MOVE: {
        //�̵��� �� ���� ����Ǵ� ���������� �� �ٲܰ�

        //���� �ڸ��� ���� ��� �ٽ� IDEL ���·� ����
        if (returnMoveStack_.empty())
            myObject.SetObjectState(ObjectState::IDEL);
        else {
            std::cout << "Return Move\n";

            auto topValue = returnMoveStack_.top();
            returnMoveStack_.pop();

            myObject.SetObjectDirection(ReverseTexutre(std::get<2>(topValue)));
            myObject.SetPosition(std::get<0>(topValue), std::get<1>(topValue));

            NETWORK::SendMoveObject(targetObject.GetSocket(), myObject.GetPositionX(), myObject.GetPositionY(),
                npc_id, myObject.GetObjectDirection());
        }
        break;
    }
    default:
        break;
    }

    //�þ߿� �ִٸ� �ٽ� �ൿ 
    if (sector.IsNearObject(npc_id, player_id) == true) {
        CTimerQueueHandle::GetInstance()->queue_.Emplace(
            EVENT_ST{ npc_id,player_id,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
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

int CMonsterInputComponent::ReverseTexutre(int texture) {
    switch (texture) {
    case CHARACTER_DOWN: {return CHARACTER_UP; }
    case CHARACTER_LEFT: {return CHARACTER_RIGHT; }
    case CHARACTER_RIGHT: {return CHARACTER_LEFT; }
    case CHARACTER_UP: {return CHARACTER_DOWN; }
    }
}

