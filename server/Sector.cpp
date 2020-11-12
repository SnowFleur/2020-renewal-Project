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
            hp      = root["Orc"]["INFOR"].get("HP", -1).asInt();
            level   = root["Orc"]["INFOR"].get("LEVEL", -1).asInt();
            exp     = root["Orc"]["INFOR"].get("EXP", -1).asInt();
            damage  = root["Orc"]["INFOR"].get("DAMAGE", -1).asInt();
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
            hp      = root["Bat"]["INFOR"].get("HP", -1).asInt();
            level   = root["Bat"]["INFOR"].get("LEVEL", -1).asInt();
            exp     = root["Bat"]["INFOR"].get("EXP", -1).asInt();
            damage  = root["Bat"]["INFOR"].get("DAMAGE", -1).asInt();
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

    // 가까이 있는 오브젝트인지 Check
    // 이미 깨어있는 Monster는 pass

    return true;
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