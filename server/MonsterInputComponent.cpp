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
        /*AI를 깨웠다는 소리는 주위에 유저가 있다는 소리*/
        break;
    case ObjectState::ATTACK:
        /*자기 공격시야에 있으면 공격 없다면 다시 이동*/


        //옆에 있는지 Check
        if (CheckNearPlayer(monster,player)) {

            //Player 체력감소 (atomic 하게 감소)
            player.SetHp(player.GetHp() - 1);

        }
        //없으면 다시 Move
        else {
            state_ = ObjectState::MOVE;
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

            monster.GetPosition(mx, my);
            player.GetPosition(px, py);

            /*
            2021.01.16
            %15는 Map Data와 맞추기 위해서 좌표값을 임의로 줄인것 나중에 최적화 해야하며
            했을 경우 해당 주석 삭제
            */
            StartPathFind(Astar::PairPosition{ mx% 15,my % 15 },
                Astar::PairPosition{ px % 15, py% 15 },
                CNavigationHandle::GetInstance()->navigation[0]);

            auto iter = astarHandle_->shortPath_.rbegin();

            //플레이어와 겹침 방지
            if (px != std::get<0>(*iter) || py != std::get<1>(*iter)) {
                mx = std::get<0>(*iter);
                my = std::get<1>(*iter);
            }
            //플레이어 근처에 도착했다.
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