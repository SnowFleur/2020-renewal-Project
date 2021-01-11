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

    //좌,우
    if (monster.y_ == player.y_) {
        if (monster.x_ == player.x_)return true;
    }
    //상,하
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
        /*AI를 깨웠다는 소리는 주위에 유저가 있다는 소리*/
        break;
    case MonsterState::ATTACK:
        /*자기 공격시야에 있으면 공격 없다면 다시 이동*/

        //옆에 있는지 Check
        if (CheckNearPlayer(monster,player)) {

            //Player 체력감소 (atomic 하게 감소)
            player.hp_--;


            //Packet 전송 주변에 있는 모든 플레이어한테
            for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
                // GetNear 함수가 없다...
                // All 할 방법도 없고

                NETWORK::SendHitObject(player.socket_, player.hp_, i, OBJECT_DEFINDS::OTHER_PLAYER);
            }
        }
        //없으면 다시 Move
        else {
            state_ = MonsterState::MOVE;
        }
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

            StartPathFind(Astar::PairPosition{ monster.x_ % 15,monster.y_ % 15 },
                Astar::PairPosition{ player.x_ % 15,player.y_ % 15 },
                CNavigationHandle::GetInstance()->navigation[0]);

            auto iter = astarHandle_->shortPath_.rbegin();

            //플레이어와 겹침 방지
            if (player.x_ != std::get<0>(*iter) || player.y_ != std::get<1>(*iter)) {
                monster.x_ = std::get<0>(*iter);
                monster.y_ = std::get<1>(*iter);
            }
            //플레이어 근처에 도착했다.
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