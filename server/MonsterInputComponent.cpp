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
    case ObjectState::IDEL: {
        /*
        플레이어가 현재 사정거리 안에 있다면 Attack으로 변경
        그렇지 않고 시야안에 있다면 move로 변경
        둘다 아니라면 다시 Sleep
        */
        if (CheckNearPlayer(myObject, targetObject)) {
            myObject.SetObjectState(ObjectState::ATTACK);
            CTimerQueueHandle::GetInstance()->queue_.Emplace(
                EVENT_ST{ npc_id,player_id,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
        }
        else if (sector.IsNearObject(npc_id, player_id) == true) {
            myObject.SetObjectState(ObjectState::MOVE);
            CTimerQueueHandle::GetInstance()->queue_.Emplace(
                EVENT_ST{ npc_id,player_id,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
        }
        else {
#ifdef _DEBUG
            std::cout << npc_id << ": IDEL To SLEEP " << "\n";
#endif // _DEBUG
            myObject.SetObjectState(ObjectState::SLEEP);
        }
        break;
    }
    case ObjectState::ATTACK: {
        /*
        플레이어가 현재 사정거리 안에 있다면 데미지
        그렇지 않고 시야안에 있다면 move로 변경
        둘다 아니라면 다시 Sleep
        */

     /*자기 공격시야에 있으면 공격 없다면 다시 이동*/

        if (CheckNearPlayer(myObject, targetObject)) {
            //tarGet 체력감소 (atomic 하게 감소)
            targetObject.SetHp(targetObject.GetHp() - 1);

            //보낼 소켓, 공격받은 클라의 HP, 공격받은 ID,  공격한 ID
            for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                //사용중인 클라이언트만
                if (sector.gameobjects_[i]->GetUsed() == false)continue;
                NETWORK::SendHitObject(sector.gameobjects_[i]->GetSocket(),
                    sector.gameobjects_[player_id]->GetHp(), player_id, sector.gameobjects_[npc_id]->GetObjectDirection(), npc_id);
            }
        }
        else {
            myObject.SetObjectState(ObjectState::MOVE);
        }

        //시야에 있다면 다시 행동 
        if (sector.IsNearObject(npc_id, player_id) == true) {
            CTimerQueueHandle::GetInstance()->queue_.Emplace(
                EVENT_ST{ npc_id,player_id,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
        }
        //시야에 없다면 슬립
        else {
            myObject.SetObjectState(ObjectState::SLEEP);
        }

        break;
    }
    case ObjectState::MOVE: {

        /*
        플레이어가 현재 시야안에 있다면 move
        플레이어를 찾을 수 없다면 returnmove으로 변경
        플레이어 근처에 도착했다면 attack으로 변경
        둘다 아니라면 다시 Sleep
        */

        //이동하기 전 값 저장
        std::unordered_set<ObjectIDType>oldView;
        PositionType x, y;
        myObject.GetPosition(x, y);

        //현재 x,y를 기반으로 cell에 있는 Object의 ID 값을 view에 넣어준다.
        sector.GetViewListByCell(oldView, x, y);

        /*
        2020.11.14
        Index 몬스터에는 한번만 Event가 들어갈줄 알았는데 Event가 계속 들어옴(Wakeup)에 따라
        해당 코드에 스레드가 두 개이상 접근하게 됐으며 그로 인해 Data Race가 발생하기 때문에
        AStar가 진행중이라면 무시한다.
        */
        if (astarFlag_ == false) {
            astarFlag_.store(true);
            PositionType mx{}, my{};
            PositionType px{}, py{};

            myObject.GetPosition(mx, my);
            targetObject.GetPosition(px, py);
            //되돌아갈 루트 스택 저장
            returnMoveStack_.push(Astar::TuplePosition{ mx,my,myObject.GetObjectDirection() });

            //21.02.10 현재는 첫 번째 방에서만 정상 A*작동
            if (StartPathFind(Astar::PairPosition{ mx,my }, Astar::PairPosition{ px,py },
                CNavigationHandle::GetInstance()->navigation[0]) == true) {

                auto iter = astarHandle_->shortPath_.rbegin();

                //플레이어와 겹침 방지
                if (px != std::get<0>(*iter) || py != std::get<1>(*iter)) {

                    //내가 갈려는 위치가 현재 이동이 가능하다 그러면 바꾸고 바꾼다.
                    //나중에 CAS InterLock Change로 Acquired 느낌으로 해야하지 않을까?
                    if (CNavigationHandle::GetInstance()->navigation[0].GetTileType(
                        std::get<0>(*iter), std::get<1>(*iter)) == TILE_TYPE::GROUND) {

                        CNavigationHandle::GetInstance()->navigation[0].ChangeTileType(mx, my,
                            std::get<0>(*iter), std::get<1>(*iter));

                        mx = std::get<0>(*iter);
                        my = std::get<1>(*iter);
                    }
                }

                //플레이어 근처에 도착했다.
                else {
                    myObject.SetObjectState(ObjectState::ATTACK);
                }
                myObject.SetObjectDirection(std::get<2>(*iter));
                myObject.SetPosition(mx, my);
            }
            else {
#ifdef _DEBUG
                std::cout << "Not Find Target\n";
#endif // _DEBUG
                myObject.SetObjectState(ObjectState::RETURN_MOVE);
            }
            astarFlag_.store(false);
        }

        //이동 후 처리
        std::unordered_set<ObjectIDType>newView;

        myObject.GetPosition(x, y);

        //현재 x,y를 기반으로 cell에 있는 Object의 ID 값을 view에 넣어준다.
        sector.GetViewListByCell(newView, x, y);

        //이동 전 ViewList
        for (auto iter : oldView) {

            //User가 이동전 리스트와 이동후 리스트에 존재한다..
            if (newView.count(iter) != 0) {

                //현재 User의 viewlist에 npc_id가 있는지 확인
                //있으면 npc정보를 해당 User에게 보낸다.
                sector.gameobjects_[iter]->srwLock_.Readlock();
                if (sector.gameobjects_[iter]->GetViewList().find(npc_id) != sector.gameobjects_[iter]->GetViewList().end()) {
                    sector.gameobjects_[iter]->srwLock_.Readunlock();

                    NETWORK::SendMoveObject(sector.gameobjects_[iter]->GetSocket(),
                        sector.gameobjects_[npc_id]->GetPositionX(), sector.gameobjects_[npc_id]->GetPositionY(),
                        npc_id, sector.gameobjects_[npc_id]->GetObjectDirection());
                }

                //현재 User의 viewIst에 npc_id가 없다.
                //없기 때문에 npc의 정보를 User에게 보낸다.
                else {
                    sector.gameobjects_[iter]->srwLock_.Readunlock();
                    sector.gameobjects_[iter]->srwLock_.Writelock();
                    sector.gameobjects_[iter]->GetViewList().insert(npc_id);
                    sector.gameobjects_[iter]->srwLock_.Writeunlock();

                    NETWORK::SendAddObject(sector.gameobjects_[iter]->GetSocket(),
                        sector.gameobjects_[npc_id]->GetPositionX(), sector.gameobjects_[npc_id]->GetPositionY(), npc_id);
                }

            }

            //User가 이동 후 리스트에 없다.
            else {

                //해당 user에 지금 npc_id가 있으면
                //지운다. 
                //가까이 있는게 아니라 멀어진거이기 때문
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
        //이동 후 리스트에 있는 유저와 NPC
        for (auto iter : newView) {
            //이동 전에 리스트 정보에는 해당 npc_id가 없다.
            if (oldView.find(npc_id) == oldView.end()) {

                //이동 후 리스트에도 현재 NPC의 정보가 없다.
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
        //시야에 있다면 다시 행동 
        if (sector.IsNearObject(npc_id, player_id) == true) {
            CTimerQueueHandle::GetInstance()->queue_.Emplace(
                EVENT_ST{ npc_id,player_id,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
        }
        else {
#ifdef _DEBUG
            std::cout << npc_id << ": Move To SLEEP " << "\n";
#endif // _DEBUG
            myObject.SetObjectState(ObjectState::SLEEP);
        }
        break;
    }
    case ObjectState::RETURN_MOVE: {
        /*
        원래 자리로 왔을 경우 IDEL로 변경
        이동 도중에 시야에서 사라지면 IDEL로 변경
        */

        //원래 자리로 왔을 경우 다시 IDEL 상태로 변경
        if (returnMoveStack_.empty()) {
            myObject.SetObjectState(ObjectState::IDEL);
        }

        else {
            //이동하기 전 값 저장
            std::unordered_set<ObjectIDType>oldView;
            PositionType x, y;
            myObject.GetPosition(x, y);

            //현재 x,y를 기반으로 cell에 있는 Object의 ID 값을 view에 넣어준다.
            sector.GetViewListByCell(oldView, x, y);


            auto topValue = returnMoveStack_.top();
            returnMoveStack_.pop();

            //Navigation 값 변경
            CNavigationHandle::GetInstance()->navigation[0].ChangeTileType(x, y,
                std::get<0>(topValue), std::get<1>(topValue));

            myObject.SetObjectDirection(ReverseTexutre(std::get<2>(topValue)));
            myObject.SetPosition(std::get<0>(topValue), std::get<1>(topValue));


            //이동 후 처리
            std::unordered_set<ObjectIDType>newView;

            myObject.GetPosition(x, y);

            //현재 x,y를 기반으로 cell에 있는 Object의 ID 값을 view에 넣어준다.
            sector.GetViewListByCell(newView, x, y);

            //이동 전 ViewList
            for (auto iter : oldView) {

                //User가 이동전 리스트와 이동후 리스트에 존재한다..
                if (newView.count(iter) != 0) {

                    //현재 User의 viewlist에 npc_id가 있는지 확인
                    //있으면 npc정보를 해당 User에게 보낸다.
                    sector.gameobjects_[iter]->srwLock_.Readlock();
                    if (sector.gameobjects_[iter]->GetViewList().find(npc_id) != sector.gameobjects_[iter]->GetViewList().end()) {
                        sector.gameobjects_[iter]->srwLock_.Readunlock();

                        NETWORK::SendMoveObject(sector.gameobjects_[iter]->GetSocket(),
                            sector.gameobjects_[npc_id]->GetPositionX(), sector.gameobjects_[npc_id]->GetPositionY(),
                            npc_id, sector.gameobjects_[npc_id]->GetObjectDirection());
                    }

                    //현재 User의 viewIst에 npc_id가 없다.
                    //없기 때문에 npc의 정보를 User에게 보낸다.
                    else {
                        sector.gameobjects_[iter]->srwLock_.Readunlock();
                        sector.gameobjects_[iter]->srwLock_.Writelock();
                        sector.gameobjects_[iter]->GetViewList().insert(npc_id);
                        sector.gameobjects_[iter]->srwLock_.Writeunlock();

                        NETWORK::SendAddObject(sector.gameobjects_[iter]->GetSocket(),
                            sector.gameobjects_[npc_id]->GetPositionX(), sector.gameobjects_[npc_id]->GetPositionY(), npc_id);
                    }

                }

                //User가 이동 후 리스트에 없다.
                else {

                    //해당 user에 지금 npc_id가 있으면
                    //지운다. 
                    //가까이 있는게 아니라 멀어진거이기 때문
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
            //이동 후 리스트에 있는 유저와 NPC
            for (auto iter : newView) {
                //이동 전에 리스트 정보에는 해당 npc_id가 없다.
                if (oldView.find(npc_id) == oldView.end()) {

                    //이동 후 리스트에도 현재 NPC의 정보가 없다.
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
        }
        //시야에 없다면 IDEL 상태로 변경다시 행동 
        if (sector.IsNearObject(npc_id, player_id) == false) {
            myObject.SetObjectState(ObjectState::IDEL);
#ifdef _DEBUG
            std::cout << npc_id << ": Return Move To IDEL " << "\n";
#endif // _DEBUG
        }
        CTimerQueueHandle::GetInstance()->queue_.Emplace(
            EVENT_ST{ npc_id,player_id,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
        break;
    }
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
    몬스터의 현재 기본 공격은 좌, 우, 상, 하 만 가능 대각선 X
    P: Player, M: Monster
    000  000  0P0  000
    PM0  0MP  0M0  0M0
    000  000  000  0P0
    */

    /*
    현재 몬스터의 Direction 방향에 따른 방향만 공격가능 여부 체크
    불 필요하게 4번 반복문 도는거 방지
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

