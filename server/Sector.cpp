#pragma warning(disable: 4996) //JSON 오류 무시
#include<algorithm>
#include<fstream>
#include"Sector.h"
#include"Player.h"
#include"Network.h"
#include"Monster.h"
#include"..\JSON\json\json.h"
#include"PlayerInputComponent.h"
#include"MonsterInputComponent.h"
#include"TimerQueue.h"

CSector::CSector() {

    //8방향 탐색용.. 초기화가 너무 구리다.
    searchDirection_[0] = SectorDir{ -1,-1 };
    searchDirection_[1] = SectorDir{ 0,-1 };
    searchDirection_[2] = SectorDir{ 1,-1 };
    searchDirection_[3] = SectorDir{ -1,0 };
    searchDirection_[4] = SectorDir{ 0,0 };
    searchDirection_[5] = SectorDir{ 1,0 };
    searchDirection_[6] = SectorDir{ -1,1 };
    searchDirection_[7] = SectorDir{ 0,1 };
    searchDirection_[8] = SectorDir{ 1,1 };


    //일단 여기서 동적할당을 받고 나중에 풀에서 받자
    for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        players_[i] = new CPlayer(PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y,100,1,0,1,new CPlayerInputComponent);
    }
    InitMonsterForJson();
}

CSector::~CSector() {}

void CSector::InitMonsterForJson() {

    std::ifstream openjsonFile("../JSON/Data/MonsterData.json");
    Json::Reader reader;
    Json::Value root;

    if (reader.parse(openjsonFile, root) == false)
        CLogCollector::GetInstance()->PrintLog("Json File Open is Fail");

    //ORC 0~100, ZOMBLE 101~200, MUMMY 201~300, BAT 301~400
    for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_MONSER; ++i) {
        HpType hp{}; LevelType level{}; ExpType exp{}; DamageType damage{};

        if (i < 100) {
            hp = root["Orc"]["INFOR"].get("HP", -1).asInt();
            level = root["Orc"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Orc"]["INFOR"].get("EXP", -1).asInt();
            damage = root["Orc"]["INFOR"].get("DAMAGE", -1).asInt();
            monsters_[i] = new CMonster(MonsterType::ORC, 0, 0, hp, level, exp, damage, new CMonsterInputComponent);

        }
        else if (i < 200) {
            monsters_[i] = new CMonster(MonsterType::ZOMBIE, 0, 0, hp, level, exp, damage, new CMonsterInputComponent);
        }
        else if (i < 300) {
            monsters_[i] = new CMonster(MonsterType::MUMMY, 0, 0, hp, level, exp, damage, new CMonsterInputComponent);

        }
        else if (i < 400) {
            hp = root["Bat"]["INFOR"].get("HP", -1).asInt();
            level = root["Bat"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Bat"]["INFOR"].get("EXP", -1).asInt();
            damage = root["Bat"]["INFOR"].get("DAMAGE", -1).asInt();
            monsters_[i] = new CMonster(MonsterType::BAT, 0, 0, hp, level, exp, damage, new CMonsterInputComponent);
        }
        /*
        2020.11.19
        여기서는 몬스터인것만 알면 되기 때문에(박쥐인지 오크인지 몰라두 됨)
        OBJECT_DEFINEDS 의 상수값을 넣어준다.
        */
        AddObject(i, OBJECT_DEFINDS::MONSTER, PRIMARY_MONSTER_X, PRIMARY_MONSTER_Y);
    }
    openjsonFile.close();
    CLogCollector::GetInstance()->PrintLog("Monster Init And Add In Sector");
    monsters_[0]->isUsed_ = true;
}

/*
2020.11.19 이 함수에서 Monster와 player의 Index를 구별해서 넣게 함으로 써 다른쪽에서 신경을 쓰지않도록 하자
*/
void CSector::AddObject(const ObjectIDType id, const ObjectClass type,const PositionType x, const PositionType y) {
    PositionType cx = x / MAP_DEFINDS::CELL_SIZE;
    PositionType cy = y / MAP_DEFINDS::CELL_SIZE;

    switch (type) {
    case OBJECT_DEFINDS::OTHER_PLAYER: {
        players_[id]->x_ = x;
        players_[id]->y_ = y;
        cells_[cx][cy].emplace(id);
        break;
    }
    case OBJECT_DEFINDS::MONSTER: {
        monsters_[id]->x_ = x;
        monsters_[id]->y_ = y;
        cells_[cx][cy].emplace(id + OBJECT_DEFINDS::MAX_USER);
        break;
    }
    default:
        CLogCollector::GetInstance()->PrintLog("Not Defined Type");
        break;
    }
}

void CSector::MoveObject(const ObjectIDType id, const PositionType newX, const PositionType newY,
    const TextureDirection textureDirection) {

    PositionType newCellX = newX / MAP_DEFINDS::CELL_SIZE;
    PositionType newCellY = newY / MAP_DEFINDS::CELL_SIZE;

    players_[id]->srwLock_.Readlock();
    std::unordered_set<ObjectIDType>oldView = players_[id]->viewLIst_;
    players_[id]->srwLock_.Readunlock();

    //새로 이동한 좌표에 해당 ID가 없다면 추가
    //20.11.17: 다른 스레드가 들어와서 Cells을 건들 수 있으니 나중에 처리
    if (cells_[newCellX][newCellY].count(id) == false) {
        cells_[newCellX][newCellY].emplace(id);

        PositionType oldX = players_[id]->x_;
        PositionType oldY = players_[id]->y_;

        players_[id]->x_ = newX;
        players_[id]->y_ = newY;

        //이전 Sector에서 값을 지운다.
        PositionType oldCellX = oldX / MAP_DEFINDS::CELL_SIZE;
        PositionType oldCellY = oldY / MAP_DEFINDS::CELL_SIZE;
        cells_[oldCellX][oldCellY].erase(id);
    }
    //이미 Sector안에 있는 ID라면 그냥 이동값만 변경
    else {
        players_[id]->x_ = newX;
        players_[id]->y_ = newY;
    }

    //해당 Sector에 있는 Object 저장
    std::unordered_set <ObjectIDType> nearList;

    //해당 Sector 주변에 있는 8방향의 Sector 정보를 NearList에 저장
    for (PositionType i = 0; i < NUMBER_OF_SEARCH; ++i) {
        short x = newCellX + searchDirection_[i].first;
        short y = newCellY + searchDirection_[i].second;
        if (x < 0 || y < 0)continue;

        for (auto i : cells_[x][y]) {
            /*
            2020.11.19 이게 최선인가...
            */
            if (IsMonster(i) == true) {
                if (SafeCheckUsedInArray(OBJECT_DEFINDS::MONSTER, i) == false)continue;
                if (IsNearMonsterAndPlayer(i, id) == false)continue;
            }
            else {
                if (SafeCheckUsedInArray(OBJECT_DEFINDS::OTHER_PLAYER, i) == false)continue;
                if (IsNearPlayerAndPlayer(i, id) == false)continue;
            }
            nearList.emplace(i);
        }
    }
    //내 정보삭제
    nearList.erase(id);

    for (auto iter : nearList) {

        //이동한 본인 oldView에 NearList의 정보가 있다면
        if (oldView.find(iter) != oldView.end()) {

            //깨어 있는 몬스터인지 확인 후 이동 시작
            StartMovedMonster(iter, id);

            //Object가 사람인 경우인경우 밑에 내용 진행
            if (IsMonster(iter) == true)continue;

            //상대 viewList에 내가 있으면
            players_[iter]->srwLock_.Readlock();
            if (players_[iter]->viewLIst_.find(id) != players_[iter]->viewLIst_.end()) {
                //이동한 플레이어의 정보를 시야안에 있는 플레이어에게 전송
                players_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER,
                    textureDirection);
            }
            //상대 viewList에 내가 없으면
            else {
                //read UnLock
                players_[iter]->srwLock_.Readunlock();

                //View List에 등록
                players_[iter]->srwLock_.Writelock();
                players_[iter]->viewLIst_.insert(id);
                players_[iter]->srwLock_.Writeunlock();

                //이동한 플레이어의 정보를 시야안에 있는 플레이어에게 전송
                NETWORK::SendAddObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER);
            }
        }
        //이동한 본인 oldview에 NearList의 정보가 없다면 
        else {
            //나의 ViewLIst에 상대정보 등록
            players_[id]->srwLock_.Writelock();
            players_[id]->viewLIst_.insert(iter);
            players_[id]->srwLock_.Writeunlock();

            //AddObject Packet 전송
            if (IsMonster(iter) == true) {
                NETWORK::SendAddObject(players_[id]->socket_,
                    monsters_[iter - OBJECT_DEFINDS::MAX_USER]->x_, 
                    monsters_[iter - OBJECT_DEFINDS::MAX_USER]->y_, iter, OBJECT_DEFINDS::MONSTER);
            }
            else {
                NETWORK::SendAddObject(players_[id]->socket_,
                    players_[iter]->x_, players_[iter]->y_, iter, OBJECT_DEFINDS::OTHER_PLAYER);
            }

            //가까이 있는(시야에 있는) Monster 깨움
            WakeUpNearMonster(iter,id);

            //Object가 사람인 경우인경우 밑에 내용 진행
            if (IsMonster(iter) == true)continue;

            players_[iter]->srwLock_.Readlock();
            if (players_[iter]->viewLIst_.find(id) != players_[iter]->viewLIst_.end()) {
                players_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER,
                    textureDirection);
            }
            //기존에 상대의 viewList에 내가 없으면 내 정보 전송
            else {
                players_[iter]->srwLock_.Readunlock();

                players_[iter]->srwLock_.Writelock();
                players_[iter]->viewLIst_.insert(id);
                players_[iter]->srwLock_.Writeunlock();

                //이동한 플레이어의 정보를 시야안에 있는 플레이어에게 전송
                NETWORK::SendAddObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER);
            }
        }
    } //End For NearList

     //oldView에 있는 모든 객체에 대해
    for (auto iter : oldView) {      //시야에서 사라짐

        if (nearList.find(iter) != nearList.end())continue;

        players_[id]->srwLock_.Writelock();
        players_[id]->viewLIst_.erase(iter);
        players_[id]->srwLock_.Writeunlock();

        //Remove Packet 전송
        if (IsMonster(iter) == true) {
            // 상태를 Sleep 상태로 변경
            monsters_[iter]->SetMonsterState(MonsterState::SLEEP);
            NETWORK::SendRemoveObject(players_[id]->socket_, iter, OBJECT_DEFINDS::MONSTER);
        }
        else {
            NETWORK::SendRemoveObject(players_[id]->socket_, iter, OBJECT_DEFINDS::OTHER_PLAYER);
        }

        //Object가 사람인 경우인경우 밑에 내용 진행
        if (IsMonster(iter) == true)continue;

        players_[iter]->srwLock_.Readlock();
        if (players_[iter]->viewLIst_.find(id) != players_[iter]->viewLIst_.end()) {
            players_[iter]->srwLock_.Readunlock();

            players_[iter]->srwLock_.Writelock();
            players_[iter]->viewLIst_.erase(id);
            players_[iter]->srwLock_.Writeunlock();
            NETWORK::SendRemoveObject(players_[iter]->socket_, id, OBJECT_DEFINDS::OTHER_PLAYER);
        }
        else {
            players_[iter]->srwLock_.Readunlock();
        }
    }
}

bool CSector::WakeUpNearMonster(const ObjectIDType montserID,const ObjectIDType playerID) {
    //MonsterID이면서 가까운 몬스터라면 깨운다.
    if (IsMonster(montserID) && IsNearMonsterAndPlayer(montserID, playerID)) {
        ObjectIDType IndexingMonster = montserID - OBJECT_DEFINDS::MAX_USER;

        // 상태를 IDEL 상태로 변경
        monsters_[IndexingMonster]->SetMonsterState(MonsterState::IDEL);
        return true;
    }
    return false;
}

void CSector::StartMovedMonster(const ObjectIDType montserID, const ObjectIDType playerID) {
    //몬스터가 아니라면 return
    if (IsMonster(montserID) == false)return;

    //이거 어떻게든 해야겠음 개 빡침
    ObjectIDType IndexingMonster = montserID - OBJECT_DEFINDS::MAX_USER;

    //IDEL 상태가 아니라면 PASS(어떠한 행동을 하고 있으면 추가 X)
    if (monsters_[IndexingMonster]->GetMonsterState() != MonsterState::IDEL)return;

    // 상태를 MOVE상태로 변경
    monsters_[IndexingMonster]->SetMonsterState(MonsterState::MOVE);

    //TimerQueue에 몬스터 추가
    CTimerQueueHandle::GetInstance()->queue_.Emplace(
        EVENT_ST{ IndexingMonster,playerID,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
}

void CSector::ProcessEvent(EVENT_ST& ev) {
    //몬스터의 행동 실행
    monsters_[ev.obj_id]->ExcuteMonster(*players_[ev.target_id]);
}

bool CSector::IsNearMonsterAndPlayer(const ObjectIDType montserID, const ObjectIDType playerID) {
    ObjectIDType IndexingMonster = montserID - OBJECT_DEFINDS::MAX_USER;

    PositionType mx = monsters_[IndexingMonster]->x_;
    PositionType my = monsters_[IndexingMonster]->y_;
    PositionType px = players_[playerID]->x_;
    PositionType py = players_[playerID]->y_;

    if (VIEW_SIZE < abs(mx - px) || VIEW_SIZE < abs(my - py))
        return false;
    return true;
}

bool CSector::IsNearPlayerAndPlayer(const ObjectIDType lhs, const ObjectIDType rhs) {
    PositionType mx = players_[lhs]->x_;
    PositionType my = players_[lhs]->y_;
    PositionType px = players_[rhs]->x_;
    PositionType py = players_[rhs]->y_;

    if (VIEW_SIZE < abs(mx - px) || VIEW_SIZE < abs(my - py))
        return false;
    return true;
}

bool CSector::SafeCheckUsedInArray(const ObjectClass type, const ObjectIDType id) {

    switch (type) {
    case OBJECT_DEFINDS::OTHER_PLAYER: {
        return players_[id]->isUsed_ ? true : false;
        break;
    }
    case OBJECT_DEFINDS::MONSTER: {
        return monsters_[id-OBJECT_DEFINDS::MAX_USER ]->isUsed_ ? true : false;
        break;
    }
    default:
        CLogCollector::GetInstance()->PrintLog("Not Defined Type In SafeCheckUsedArray Function");
        break;
    }
}

bool CSector::TestFunction(const ObjectIDType montserID, const ObjectIDType playerID) {

    //일치하면 그냥 갱신하고 끝
    if (monsters_[montserID]->followPlayerId_ == playerID) {
        return true;
    }
    //더 가까운거로 비교후 ID Change
    else {
        ObjectIDType followId = monsters_[montserID]->followPlayerId_;

        PositionType mx = monsters_[montserID]->x_;
        PositionType my = monsters_[montserID]->y_;
        PositionType px = players_[playerID]->x_;
        PositionType py = players_[playerID]->y_;
        PositionType fx = players_[followId]->x_;
        PositionType fy = players_[followId]->y_;

        PositionType newDistance = abs(mx - px) + abs(my - py);
        PositionType oldDistance = abs(mx - fx) + abs(my - fy);

        if (oldDistance < newDistance) {
            std::cout << "새로운 플레이어가 더 멀리 있음 FALSE\n";
            return false;
        }
        else {
            std::cout << "새로운 플레이어가 더 가까이 있음 TRUE\n";
            monsters_[montserID]->followPlayerId_ = playerID;
            return true;
        }
    }
}

bool CSector::IsMonster(const ObjectIDType id)const {
    return id >= OBJECT_DEFINDS::MAX_USER ? true : false;
}

