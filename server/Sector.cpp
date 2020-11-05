#include"Sector.h"
#include"Player.h"
#include"Network.h"
#include"Monster.h"

CSector::CSector() {
    //�ϴ� ���⼭ �����Ҵ��� �ް� ���߿� Ǯ���� ����
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
            
            //�׽�Ʈ��
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

    //���� �̵��� ��ǥ�� �ش� ID�� ���ٸ� �߰�
    if (cells_[newCellX][newCellY].count(id) == false) {
        cells_[newCellX][newCellY].emplace(id);

        PositionType oldX = players_[id]->x_;
        PositionType oldY = players_[id]->y_;

        //���� Sector���� ���� �����.
        PositionType oldCellX = oldX / MAP_DEFINDS::CELL_SIZE;
        PositionType oldCellY = oldY / MAP_DEFINDS::CELL_SIZE;
        cells_[oldCellX][oldCellY].erase(id);
    }


    //�̵��� Ŭ���̾�Ʈ�� ���� ���Ϳ� �ִ� Ŭ��鿡�� Move ����
    //����� �̵� üũ�� �� �����̱� ������ �׳� ��ü ���� (20.11.01)
    for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        //���� ���� �� ������� Ŭ���̾�Ʈ��
        if (i == id || players_[i]->isUsed_ == false)continue;
        //socket, �̵�x, �̵�y, �̵���id, �̵���id�� �ؽ�������
        NETWORK::SendMoveObject(players_[i]->socket_, newX, newY, id, textureDirection);
    }

}

