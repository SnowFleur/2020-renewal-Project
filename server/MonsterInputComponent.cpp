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
MyObject:       Monster(본인)
targetObject:   targetObject(상대, 주로 Player 예상)
*/
void CMonsterInputComponent::ExcuteEvent(CGameObject& myObject, CGameObject& targetObject) {

    switch (myObject.GetObjectState()) {
    case ObjectState::IDEL:
        /*AI를 깨웠다는 소리는 주위에 유저가 있다는 소리*/
        break;
    case ObjectState::ATTACK:
        /*자기 공격시야에 있으면 공격 없다면 다시 이동*/

        //옆에 있는지 Check
        if (CheckNearPlayer(myObject, targetObject)) {
            //tarGet 체력감소 (atomic 하게 감소)
            targetObject.SetHp(targetObject.GetHp() - 1);
        }
        //없으면 다시 Move
        else {
            myObject.SetObjectState(ObjectState::MOVE);
        }
        break;
    case ObjectState::MOVE: {
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
            returnMoveStack_.push(Position{ mx,my });
            /*
            2021.01.16
            %15는 Map Data와 맞추기 위해서 좌표값을 임의로 줄인것 나중에 최적화 해야하며
            했을 경우 해당 주석 삭제
            */

            if (StartPathFind(Astar::PairPosition{ mx,my }, Astar::PairPosition{ px,py },
                CNavigationHandle::GetInstance()->navigation[0]) == true) {

                auto iter = astarHandle_->shortPath_.rbegin();

                //플레이어와 겹침 방지
                if (px != std::get<0>(*iter) || py != std::get<1>(*iter)) {
                    mx = std::get<0>(*iter);
                    my = std::get<1>(*iter);
                }
                //플레이어 근처에 도착했다.
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
        //이동할 때 마다 변경되는 지형데이터 꼭 바꿀것
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