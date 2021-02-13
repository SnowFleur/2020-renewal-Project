#pragma warning(disable: 4996) //JSON 오류 무시
#include<algorithm>
#include<fstream>
#include"GameObject.h"
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
    //유저 생성
    for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        gameobjects_[i] = new CPlayer(PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y, 100, 1, 0, 1, new CPlayerInputComponent);
    }

    //NPC 생성
    for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        //gameobjects_[i] = new CNPC(PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y, 100, 1, 0, 1, new CPlayerInputComponent);
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
    for (ObjectIDType i = OBJECT_DEFINDS::MAX_USER; i < OBJECT_DEFINDS::MAX_USER + OBJECT_DEFINDS::MAX_MONSER; ++i) {
        HpType hp{}; LevelType level{}; ExpType exp{}; AttackPowerType attackPower{};

        if (i < OBJECT_DEFINDS::MAX_USER + 100) {
            hp = root["Orc"]["INFOR"].get("HP", -1).asInt();
            level = root["Orc"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Orc"]["INFOR"].get("EXP", -1).asInt();
            attackPower = root["Orc"]["INFOR"].get("DAMAGE", -1).asInt();
            gameobjects_[i] = new CMonster(MonsterType::ORC, 0, 0, hp, level, exp, attackPower, new CMonsterInputComponent);
        }
        else if (i < OBJECT_DEFINDS::MAX_USER + 200) {
            gameobjects_[i] = new CMonster(MonsterType::ZOMBIE, 0, 0, hp, level, exp, attackPower, new CMonsterInputComponent);
        }
        else if (i < OBJECT_DEFINDS::MAX_USER + 300) {
            gameobjects_[i] = new CMonster(MonsterType::MUMMY, 0, 0, hp, level, exp, attackPower, new CMonsterInputComponent);

        }
        else if (i < OBJECT_DEFINDS::MAX_USER + 400) {
            hp = root["Bat"]["INFOR"].get("HP", -1).asInt();
            level = root["Bat"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Bat"]["INFOR"].get("EXP", -1).asInt();
            attackPower = root["Bat"]["INFOR"].get("DAMAGE", -1).asInt();
            gameobjects_[i] = new CMonster(MonsterType::BAT, 0, 0, hp, level, exp, attackPower, new CMonsterInputComponent);
        }
    }
    openjsonFile.close();
    CLogCollector::GetInstance()->PrintLog("Monster Init And Add In Sector");

    //테스트를 위해 하나만 True 킴;
    gameobjects_[OBJECT_DEFINDS::MAX_USER + 300]->SetUsed(true);
    //21.02.02 현재는 하나의 몬스터만 사용할 것이기 때문에 하나만 등록 나중에는 위로 올려서 다 등록
    AddObject(OBJECT_DEFINDS::MAX_USER + 300, PRIMARY_MONSTER_X, PRIMARY_MONSTER_Y);


    //테스트를 위해 하나만 True 킴;
    gameobjects_[OBJECT_DEFINDS::MAX_USER + 301]->SetUsed(true);
    //21.02.02 현재는 하나의 몬스터만 사용할 것이기 때문에 하나만 등록 나중에는 위로 올려서 다 등록
    AddObject(OBJECT_DEFINDS::MAX_USER + 301, PRIMARY_MONSTER_X + 1, PRIMARY_MONSTER_Y);


    ////테스트를 위해 하나만 True 킴;
    //gameobjects_[OBJECT_DEFINDS::MAX_USER + 302]->SetUsed(true);
    ////21.02.02 현재는 하나의 몬스터만 사용할 것이기 때문에 하나만 등록 나중에는 위로 올려서 다 등록
    //AddObject(OBJECT_DEFINDS::MAX_USER + 302, PRIMARY_MONSTER_X + 1, PRIMARY_MONSTER_Y+5);


    ////테스트를 위해 하나만 True 킴;
    //gameobjects_[OBJECT_DEFINDS::MAX_USER + 303]->SetUsed(true);
    ////21.02.02 현재는 하나의 몬스터만 사용할 것이기 때문에 하나만 등록 나중에는 위로 올려서 다 등록
    //AddObject(OBJECT_DEFINDS::MAX_USER + 303, PRIMARY_MONSTER_X, PRIMARY_MONSTER_Y+5);

}

void CSector::AddObject(const ObjectIDType id, const PositionType x, const PositionType y) {
    PositionType cx = x / MAP_DEFINDS::CELL_SIZE;
    PositionType cy = y / MAP_DEFINDS::CELL_SIZE;

    gameobjects_[id]->SetPosition(x, y);
    cells_[cx][cy].emplace(id);
}

/*
이것도 PlayerInputComponet로 옮기기
*/
void CSector::MoveObject(const ObjectIDType id, const PositionType newX, const PositionType newY,
    const TextureDirection textureDirection) {

    PositionType newCellX = newX / MAP_DEFINDS::CELL_SIZE;
    PositionType newCellY = newY / MAP_DEFINDS::CELL_SIZE;


    gameobjects_[id]->srwLock_.Readlock();
    std::unordered_set<ObjectIDType>oldView = gameobjects_[id]->GetViewList();
    gameobjects_[id]->srwLock_.Readunlock();


    //새로 이동한 좌표에 해당 ID가 없다면 추가
    //20.11.17: 다른 스레드가 들어와서 Cells을 건들 수 있으니 나중에 처리
    if (cells_[newCellX][newCellY].count(id) == false) {
        cells_[newCellX][newCellY].emplace(id);

        PositionType oldX = gameobjects_[id]->GetPositionX();
        PositionType oldY = gameobjects_[id]->GetPositionY();

        //이전 Sector에서 값을 지운다.
        PositionType oldCellX = oldX / MAP_DEFINDS::CELL_SIZE;
        PositionType oldCellY = oldY / MAP_DEFINDS::CELL_SIZE;
        cells_[oldCellX][oldCellY].erase(id);
    }

    gameobjects_[id]->SetPosition(newX, newY);

    //해당 Sector에 있는 Object 저장
    std::unordered_set <ObjectIDType> nearList;

    //해당 Sector 주변에 있는 8방향의 Sector 정보를 NearList에 저장
    for (PositionType i = 0; i < NUMBER_OF_SEARCH; ++i) {
        short x = newCellX + searchDirection_[i].first;
        short y = newCellY + searchDirection_[i].second;
        if (x < 0 || y < 0)continue;

        for (auto i : cells_[x][y]) {
            nearList.emplace(i);
        }
    }
    //내 정보삭제
    nearList.erase(id);

    for (auto iter : nearList) {

        //이동한 본인 oldView에 NearList의 정보가 있다면
        if (oldView.find(iter) != oldView.end()) {

            //깨어 있는 몬스터인지 확인 후 이동 시작
            WakeUpNearMonster(iter, id);

            //Object가 사람인 경우인경우 밑에 내용 진행
            if (IsMonster(iter) == true)continue;

            //상대 viewList에 내가 있으면
            gameobjects_[iter]->srwLock_.Readlock();
            if (gameobjects_[iter]->GetViewList().find(id) != gameobjects_[iter]->GetViewList().end()) {
                //이동한 플레이어의 정보를 시야안에 있는 플레이어에게 전송
                gameobjects_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(gameobjects_[iter]->GetSocket(),
                    gameobjects_[id]->GetPositionX(), gameobjects_[id]->GetPositionY(), id, textureDirection);
            }
            //상대 viewList에 내가 없으면
            else {
                //read UnLock
                gameobjects_[iter]->srwLock_.Readunlock();

                //View List에 등록
                gameobjects_[iter]->srwLock_.Writelock();
                gameobjects_[iter]->GetViewList().insert(id);
                gameobjects_[iter]->srwLock_.Writeunlock();

                //이동한 플레이어의 정보를 시야안에 있는 플레이어에게 전송
                NETWORK::SendAddObject(gameobjects_[iter]->GetSocket(),
                    gameobjects_[id]->GetPositionX(), gameobjects_[id]->GetPositionY(), id);
            }
        }
        //이동한 본인 oldview에 NearList의 정보가 없다면 
        else {
            //나의 ViewLIst에 상대정보 등록
            gameobjects_[id]->srwLock_.Writelock();
            gameobjects_[id]->GetViewList().insert(iter);
            gameobjects_[id]->srwLock_.Writeunlock();

            //AddObject Packet 전송
            if (IsMonster(iter) == true) {
                NETWORK::SendAddObject(gameobjects_[id]->GetSocket(),
                    gameobjects_[iter]->GetPositionX(), gameobjects_[iter]->GetPositionY(), iter);
            }
            else {
                NETWORK::SendAddObject(gameobjects_[id]->GetSocket(),
                    gameobjects_[iter]->GetPositionX(), gameobjects_[iter]->GetPositionY(), iter);
            }

            //가까이 있는(시야에 있는) Monster 깨움
            WakeUpNearMonster(iter, id);

            //Object가 사람인 경우인경우 밑에 내용 진행
            if (IsMonster(iter) == true)continue;

            gameobjects_[iter]->srwLock_.Readlock();
            if (gameobjects_[iter]->GetViewList().find(id) != gameobjects_[iter]->GetViewList().end()) {
                gameobjects_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(gameobjects_[iter]->GetSocket(),
                    gameobjects_[id]->GetPositionX(), gameobjects_[id]->GetPositionY(), id, textureDirection);
            }
            //기존에 상대의 viewList에 내가 없으면 내 정보 전송
            else {
                gameobjects_[iter]->srwLock_.Readunlock();

                gameobjects_[iter]->srwLock_.Writelock();
                gameobjects_[iter]->GetViewList().insert(id);
                gameobjects_[iter]->srwLock_.Writeunlock();

                //이동한 플레이어의 정보를 시야안에 있는 플레이어에게 전송
                NETWORK::SendAddObject(gameobjects_[iter]->GetSocket(),
                    gameobjects_[id]->GetPositionX(), gameobjects_[id]->GetPositionY(), id);
            }
        }
    } //End For NearList

     //oldView에 있는 모든 객체에 대해
    for (auto iter : oldView) {

        //NearList에 없다면 시야에서 사라진 object
        if (nearList.find(iter) != nearList.end())continue;

        gameobjects_[id]->srwLock_.Writelock();
        gameobjects_[id]->GetViewList().erase(iter);
        gameobjects_[id]->srwLock_.Writeunlock();

        // 상태를 SLEEP 상태로 변경
        gameobjects_[iter]->SetObjectState(ObjectState::SLEEP);
        //Remove Packet 전송
        NETWORK::SendRemoveObject(gameobjects_[id]->GetSocket(), iter);

        //Object가 사람인 경우인경우 밑에 내용 진행
        if (IsMonster(iter) == true)continue;

        gameobjects_[iter]->srwLock_.Readlock();
        if (gameobjects_[iter]->GetViewList().find(id) != gameobjects_[iter]->GetViewList().end()) {
            gameobjects_[iter]->srwLock_.Readunlock();

            gameobjects_[iter]->srwLock_.Writelock();
            gameobjects_[iter]->GetViewList().erase(id);
            gameobjects_[iter]->srwLock_.Writeunlock();
            NETWORK::SendRemoveObject(gameobjects_[iter]->GetSocket(), id);
        }
        else {
            gameobjects_[iter]->srwLock_.Readunlock();
        }
    }
}

bool CSector::WakeUpNearMonster(const ObjectIDType montserID, const ObjectIDType playerID) {

    //SLEEP 상태가 아니라면 PASS(어떠한 행동을 하고 있는 중)
    if (gameobjects_[montserID]->GetObjectState() != ObjectState::SLEEP) return false;

    //가까우면서 몬스터라면 깨운다.
    if (IsMonster(montserID) && IsNearObject(montserID, playerID)) {

        // 상태를 IDEL 상태로 변경
        gameobjects_[montserID]->SetObjectState(ObjectState::IDEL);

        //TimerQueue에 몬스터 추가
        CTimerQueueHandle::GetInstance()->queue_.Emplace(
            EVENT_ST{ montserID,playerID,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });

        return true;
    }
    return false;
}

//void CSector::StartMovedMonster(const ObjectIDType montserID, const ObjectIDType playerID) {
//    //몬스터가 아니라면 return
//    if (IsMonster(montserID) == false)return;
//
//    //IDEL 상태가 아니라면 PASS(어떠한 행동을 하고 있으면 추가 X)
//    if (gameobjects_[montserID]->GetObjectState() != ObjectState::IDEL)return;
//
//    // 상태를 MOVE상태로 변경
//    gameobjects_[montserID]->SetObjectState(ObjectState::MOVE);
//
//    //TimerQueue에 몬스터 추가
//    CTimerQueueHandle::GetInstance()->queue_.Emplace(
//        EVENT_ST{ montserID,playerID,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
//}

void CSector::ProcessEvent(EVENT_ST& ev) {
    gameobjects_[ev.obj_id]->ProcessInputComponent(*this, ev);
}

bool CSector::IsNearObject(const ObjectIDType lhs, const ObjectIDType rhs) {

    PositionType lhsX{}, lhsY{}, rhsX{}, rhsY{};

    gameobjects_[lhs]->GetPosition(lhsX, lhsY);
    gameobjects_[rhs]->GetPosition(rhsX, rhsY);

    if (VIEW_SIZE < abs(lhsX - rhsX) || VIEW_SIZE < abs(lhsY - rhsY))
        return false;
    return true;
}

bool CSector::IsMonster(const ObjectIDType id)const {
    return id >= OBJECT_DEFINDS::MAX_USER ? true : false;
}

void  CSector::GetViewListByCell(std::unordered_set<ObjectIDType>& view, PositionType x, PositionType y) {
    PositionType CellX = x / MAP_DEFINDS::CELL_SIZE;
    PositionType CellY = y / MAP_DEFINDS::CELL_SIZE;
    for (PositionType i = 0; i < NUMBER_OF_SEARCH; ++i) {
        short x = CellX + searchDirection_[i].first;
        short y = CellY + searchDirection_[i].second;
        if (x < 0 || y < 0)continue;

        for (auto i : cells_[x][y]) {
            if (i < OBJECT_DEFINDS::MAX_USER) {
                view.emplace(i);
            }
        }
    }
}
