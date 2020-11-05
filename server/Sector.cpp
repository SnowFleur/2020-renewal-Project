#include"Sector.h"
#include"Player.h"
#include"Network.h"
#include"Monster.h"

CSector::CSector() {
    //일단 여기서 동적할당을 받고 나중에 풀에서 받자
    for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        players_[i] = new CPlayer();
    }

    //ORC 0~100, ZOMBLE 101~200, MUMMY 201~300, BAT 301~400
    for (int i = 0; i < OBJECT_DEFINDS::MAX_MONSER; ++i) {


        switch (i / 1) {
        case 0: {
            monsters_[i] = new CMonster(MonsterType::ORC);
            break;
        }
        case 1: {
            monsters_[i] = new CMonster(MonsterType::ZOMBIE);
            break;
        }
        case 2: {
            monsters_[i] = new CMonster(MonsterType::MUMMY);
            
            //테스트용
            monsters_[i]->x_ = 15;
            monsters_[i]->y_ = 15;
            
            break;
        }
        case 3: {
            monsters_[i] = new CMonster(MonsterType::BAT);
            break;
        }
        default:
            break;
        }
    }
}

CSector::~CSector() {}

void CSector::GetCellObject(const PositionType x, const PositionType y) {


}

void CSector::SetCellObject(const PositionType x, const PositionType y) {

}

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

        //이전 Sector에서 값을 지운다.
        PositionType oldCellX = oldX / MAP_DEFINDS::CELL_SIZE;
        PositionType oldCellY = oldY / MAP_DEFINDS::CELL_SIZE;
        cells_[oldCellX][oldCellY].erase(id);
    }


    //이동한 클라이언트와 같은 섹터에 있는 클라들에게 Move 전송
    //현재는 이동 체크만 할 에정이기 때문에 그냥 전체 전송 (20.11.01)
    for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        //본인 제외 및 사용중인 클라이언트만
        if (i == id || players_[i]->isUsed_ == false)continue;
        //socket, 이동x, 이동y, 이동한id, 이동한id의 텍스쳐정보
        NETWORK::SendMoveObject(players_[i]->socket_, newX, newY, id, textureDirection);
    }

}

