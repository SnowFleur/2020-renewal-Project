#pragma warning(disable: 4996) //JSON 오류 무시
#include<algorithm>
#include<fstream>
#include"Sector.h"
#include"Player.h"
#include"Network.h"
#include"Monster.h"
#include"TimerThread.h"
#include"..\JSON\json\json.h"


CSector::CSector() {
    //일단 여기서 동적할당을 받고 나중에 풀에서 받자
    for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        players_[i] = new CPlayer();
    }

    std::ifstream openjsonFile("../JSON/Data/MonsterData.json");
    Json::Reader reader;
    Json::Value root;

    if (reader.parse(openjsonFile, root) == false)
        CLogCollector::GetInstance()->PrintLog("Json File Open is Fail");

    //ORC 0~100, ZOMBLE 101~200, MUMMY 201~300, BAT 301~400
    for (int i = 0; i < OBJECT_DEFINDS::MAX_MONSER; ++i) {
        HpType hp{}; LevelType level{}; ExpType exp{}; DamageType damage{};

        switch (i / 100) {
        case 0: {
            hp = root["Orc"]["INFOR"].get("HP", -1).asInt();
            level = root["Orc"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Orc"]["INFOR"].get("EXP", -1).asInt();
            damage = root["Orc"]["INFOR"].get("DAMAGE", -1).asInt();
            monsters_[i] = new CMonster(MonsterType::ORC, 0, 0, hp, level, exp, damage);

            break;
        }
        case 1: {
            //monsters_[i] = new CMonster(MonsterType::ZOMBIE);
            break;
        }
        case 2: {
            //monsters_[i] = new CMonster(MonsterType::MUMMY);
            break;
        }
        case 3: {
            hp = root["Bat"]["INFOR"].get("HP", -1).asInt();
            level = root["Bat"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Bat"]["INFOR"].get("EXP", -1).asInt();
            damage = root["Bat"]["INFOR"].get("DAMAGE", -1).asInt();
            monsters_[i] = new CMonster(MonsterType::BAT, 0, 0, hp, level, exp, damage);
            break;
        }
        default:
            break;
        }
    }

    openjsonFile.close();

}

CSector::~CSector() {}

void CSector::GetCellObject(const PositionType x, const PositionType y) {}

/*
2020.11.16: 현재 Object의 ID 구별이 Monster인지 Player인지 어려운 상황이기 때문에
Monster는 넣을 때 최대 플레이어 개수인 2500개 이상부터 +해서 넣어야 할것으로 보임
*/
void CSector::SetCellObject(const PositionType x, const PositionType y) {}

void CSector::AddObject(const ObjectIDType id, const PositionType x, const PositionType y) {
    PositionType cx = x / MAP_DEFINDS::CELL_SIZE;
    PositionType cy = y / MAP_DEFINDS::CELL_SIZE;
    cells_[cx][cy].emplace(id);

    players_[id]->x_ = x;
    players_[id]->y_ = y;
}

void CSector::MoveObject(const ObjectIDType id, const PositionType newX, const PositionType newY,
    const TextureDirection textureDirection) {


    PositionType newCellX = newX / MAP_DEFINDS::CELL_SIZE;
    PositionType newCellY = newY / MAP_DEFINDS::CELL_SIZE;

    players_[id]->srwLock_.Writelock();
    std::unordered_set<ObjectIDType>oldView = players_[id]->viewLIst_;
    players_[id]->srwLock_.Writeunlock();


    //해당 Sector에 있는 Object 저장
    std::unordered_set <ObjectIDType> nearList = cells_[newCellX][newCellY];


    /*
    Near의 모든 객체에 대해

    1. viewList에 없으면
    2. ViewList에 있으면



    */

    for (auto iter : nearList) {

        //이동한 본인 oldView에 NearList의 정보가 있다면
        if (oldView.find(iter) != oldView.end()) {

            //가까이 있는 Monster 깨움
            WakeUpNearMonster(iter, id);

            //Object가 사람인 경우 실행
            if (IsMonster(iter) == false)continue;

            players_[iter]->srwLock_.Readlock();
            //상대 viewList에 내가 있으면
            if (players_[iter]->viewLIst_.count(id)) {
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

            //상대정보를 전송
            NETWORK::SendAddObject(players_[id]->socket_,
                players_[iter]->x_, players_[iter]->y_, iter, OBJECT_DEFINDS::OTHER_PLAYER);

            //가까이 있는 Monster 깨움
            WakeUpNearMonster(id, iter);

            //Object가 사람인 경우 실행
            if (IsMonster(iter) == false)continue;

            players_[iter]->srwLock_.Readlock();
            if (players_[iter]->viewLIst_.count(id)) {
                players_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER,
                    textureDirection);
            }

            //기존에 상대의 viewList에 내가 없으면
            //내 정보 전송
            else {
                players_[iter]->srwLock_.Writelock();
                players_[iter]->viewLIst_.insert(id);
                players_[iter]->srwLock_.Writeunlock();

                //이동한 플레이어의 정보를 시야안에 있는 플레이어에게 전송
                NETWORK::SendAddObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER);
            }

        }

    } //End For NearList

     //oldView에 있는 모든 객채에 대해
    for (auto iter : oldView) {      //시야에서 사라짐

        if (0 != nearList.count(iter))
            continue;

        players_[id]->srwLock_.Writelock();
        players_[id]->viewLIst_.erase(iter);
        players_[id]->srwLock_.Writeunlock();

        NETWORK::SendRemoveObject(players_[id]->socket_, iter, OBJECT_DEFINDS::MONSTER);

        //Object가 사람인 경우 실행
        if (IsMonster(iter) == false)continue;

        players_[iter]->srwLock_.Readlock();
        if (0 != players_[iter]->viewLIst_.count(id)) {
            players_[iter]->srwLock_.Readunlock();

            players_[iter]->srwLock_.Writelock();
            players_[iter]->viewLIst_.erase(id);
            players_[id]->srwLock_.Writeunlock();

            NETWORK::SendRemoveObject(players_[iter]->socket_, ), OBJECT_DEFINDS::MONSTER);
        }
        else {
            players_[iter]->srwLock_.Readunlock();
        }
    }


    //새로 이동한 좌표에 해당 ID가 없다면 추가
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

    //이동한 클라이언트와 같은 섹터에 있는 클라들에게 Move 전송
    //현재는 이동 체크만 할 에정이기 때문에 그냥 전체 전송 (20.11.01)
    for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        //본인 제외 및 사용중인 클라이언트만
        if (i == id || players_[i]->isUsed_ == false)continue;
        //socket, 이동x, 이동y, 이동한id, 클래스(플레이어),이동한id의 텍스쳐정보
        NETWORK::SendMoveObject(players_[i]->socket_, newX, newY, id,
            OBJECT_DEFINDS::OTHER_PLAYER, textureDirection);
    }
}

bool CSector::WakeUpNearMonster(const ObjectIDType montserID, const ObjectIDType playerID) {

    //MonsterID인지 확인
    if (IsMonster(montserID) == false)return false;

    //플레이어와 가까이 있는 몬스터인지 Check
    if (IsNearPlayer(montserID, playerID)) {
        // 이미 깨어있는 Monster는 pass
        monsters_[montserID]->isUsed_ = true;
        return true;
    }
    return false;
}

void CSector::ProcessEvent(EVENT_ST& ev) {

    switch (ev.type) {
    case EV_MONSTER_MOVE: {
        //자기를 깨운 플레이어 정보를 넣는다.
        monsters_[ev.obj_id]->MoveMonster(*players_[ev.target_id]);

        //if(cells_.)

        break;
    }
    default:
        break;
    }

}

bool CSector::IsNearPlayer(const ObjectIDType montserID, const ObjectIDType playerID) {
    PositionType mx = monsters_[montserID]->x_;
    PositionType my = monsters_[montserID]->y_;
    PositionType px = players_[playerID]->x_;
    PositionType py = players_[playerID]->y_;

    if (VIEW_SIZE < abs(mx - px) || VIEW_SIZE < abs(my - py))
        return false;

    return true;
}

bool CSector::TestFunction(const ObjectIDType montserID, const ObjectIDType playerID) {


    PositionType mx = monsters_[montserID]->x_;
    PositionType my = monsters_[montserID]->y_;
    PositionType px = players_[playerID]->x_;
    PositionType py = players_[playerID]->y_;

    PositionType distance = abs(mx - px) + abs(my - py);

    /*
    멀어지면 ShortDistance보다 멀어지기 때문에 A 스타를 중지하는 문제가 발생

    2개이상의 플레이어가 시야에 잡힐 때 어느 플레이어를 상대로 A스타를 진행할지
    판단 시켜줘야함

    */

    /*
    거리 비교 -> 더 짧으면 ID 교체 아니면 FALSE
    */

    if (monsters_[montserID]->shortDistanceByPlayer_ > distance) {
        monsters_[montserID]->shortDistanceByPlayer_ = distance;

        std::cout << "Short Player ID: " << (int)playerID << "\n";
        return true;
    }

    return false;
}

bool CSector::IsMonster(const ObjectIDType id) {
    return id >= OBJECT_DEFINDS::MAX_USER ? true : false;
}