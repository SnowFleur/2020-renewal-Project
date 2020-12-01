#include<algorithm>
#include<iostream>
#include "Sector.h"
#include"GameObject.h"
/*
��Ģ: ��� cell ����� �Լ��ȿ��� �ϰ� �ۿ��� �������� ��ǥ��
������Ʈ ������ ��ǥ�� �Է��Ѵ�.
*/

CSector::~CSector() {
    for (int i = 0; i < MAP_DEFINDS::CELL_SIZE; ++i) {
        if (cells_[i] != nullptr) {
            delete[] cells_[i];
        }
    }
}

void CSector::Add(IGameObject* gameobject) {
    unsigned short objectX{}, objectY{};
    gameobject->GetPosition(objectX, objectY);

    unsigned short cellX = objectX / MAP_DEFINDS::CELL_SIZE;
    unsigned short cellY = objectY / MAP_DEFINDS::CELL_SIZE;
    //Celle�� Object Add
    cells_[cellX][cellY].emplace(gameobject);
}

void CSector::Remove(IGameObject* gameobject) {
    unsigned short objectX{}, objectY{};
    gameobject->GetPosition(objectX, objectY);

    unsigned short cellX = objectX / MAP_DEFINDS::CELL_SIZE;
    unsigned short cellY = objectY / MAP_DEFINDS::CELL_SIZE;
    //Celle�� Object erase
    cells_[cellX][cellY].erase(gameobject);
}

#pragma region Collision Functions

bool CSector::Collision(IGameObject* gameobject, COLLISION_TYPE type) {
    collisionType_ = type;
    bool isCollision{ false };
    unsigned short objectX{}, objectY{};
    gameobject->GetPosition(objectX, objectY);

    unsigned short cellX = objectX / MAP_DEFINDS::CELL_SIZE;
    unsigned short cellY = objectY / MAP_DEFINDS::CELL_SIZE;
    /*
    Object�� �ִ� Sector�� ������ 4�� Sector�� Check
    */
    CheckSector(gameobject, cellX, cellY, isCollision);
    //����
    if (cellX - 1 >= 0)
        CheckSector(gameobject, cellX - 1, cellY, isCollision);
    //������
    if (cellX + 1 <= MAP_DEFINDS::SECTOR_SIZE)
        CheckSector(gameobject, cellX + 1, cellY, isCollision);
    //����
    if (cellY - 1 >= 0)
        CheckSector(gameobject, cellX, cellY - 1, isCollision);
    //�Ʒ���
    if (cellY + 1 <= MAP_DEFINDS::SECTOR_SIZE)
        CheckSector(gameobject, cellX, cellY + 1, isCollision);


    // ������ ���̰�
    // ĳ���� ��ġ�� X
    return isCollision;
}

void CSector::CheckSector(IGameObject* gameobject, unsigned short cellX, unsigned short cellY,
    bool& isCollision)const {

    unsigned short myX{}, myY{};
    gameobject->GetPosition(myX, myY);

    unsigned short otherX{}, otherY{};

    for (const auto Othergameobjet : cells_[cellX][cellY]) {

        if (gameobject == Othergameobjet)continue;
        Othergameobjet->GetPosition(otherX, otherY);

        if (CompareDistanceObject(myX, myY, otherX, otherY, 1) == false)continue;

        switch (collisionType_) {
        case COLLISION_TYPE::ATTACK:
            isCollision = CheckAttack(myX, myY, otherX, otherY);
            break;
        case COLLISION_TYPE::MOVE_LEFT:
        case COLLISION_TYPE::MOVE_RIGHT:
        case COLLISION_TYPE::MOVE_UP:
        case COLLISION_TYPE::MOVE_DOWN:
            isCollision = CheckMove(myX, myY, otherX, otherY);
            break;
        default:
            break;
        }

    }

}

bool CSector::CompareDistanceObject(unsigned short lhsX, unsigned short lhsY,
    unsigned short rhsX, unsigned short rhsY, unsigned short length)const {

    // (x2-x1)^2 + (y2-y1)^2
    auto x = (lhsX - rhsX) * (lhsX - rhsX);
    auto y = (lhsY - rhsY) * (lhsY - rhsY);

    if (sqrt(x + y) <= length) {
        return true;
    }
    return false;
}

inline bool CSector::CheckAttack(unsigned short lhsX, unsigned short lhsY,
    unsigned short rhsX, unsigned short rhsY)const {
    //��
    if (lhsX == rhsX && lhsY - 1 == rhsY) {
        std::cout << "Collision Attack ��\n";
    }
    //��
    else if (lhsX == rhsX && lhsY + 1 == rhsY) {
        std::cout << "Collision Attack ��\n";
    }
    //��
    else if (lhsY == rhsY && lhsX - 1 == rhsX) {
        std::cout << "Collision Attack ��\n";
    }
    //��
    else if (lhsY == rhsY && lhsX + 1 == rhsX) {
        std::cout << "Collision Attack ��\n";
    }
    return true;
}

inline bool CSector::CheckMove(unsigned short lhsX, unsigned short lhsY,
    unsigned short rhsX, unsigned short rhsY)const {

    switch (collisionType_) {
    case COLLISION_TYPE::MOVE_LEFT:
        if (lhsY == rhsY && lhsX - 1 == rhsX) {
            std::cout << "Collision Move ��\n";
            return true;
        }
        break;
    case COLLISION_TYPE::MOVE_RIGHT:
        if (lhsY == rhsY && lhsX + 1 == rhsX) {
            std::cout << "Collision Move ��\n";
            return true;
        }
        break;
    case COLLISION_TYPE::MOVE_UP:
        //��
        if (lhsX == rhsX && lhsY - 1 == rhsY) {
            std::cout << "Collision Move ��\n";
            return true;
        }

        break;
    case COLLISION_TYPE::MOVE_DOWN:
        //��
        if (lhsX == rhsX && lhsY + 1 == rhsY) {
            std::cout << "Collision Move ��\n";
            return true;
        }
        break;
    default:
        break;
    }


    return false;
}


#pragma endregion

void CSector::ChangeSector(IGameObject* gameobject,
    const unsigned short newX, const unsigned short newY) {

    unsigned short newCellX = newX / MAP_DEFINDS::CELL_SIZE;
    unsigned short newCellY = newY / MAP_DEFINDS::CELL_SIZE;

    //���� �̵��� ��ǥ�� �ش� ������Ʈ�� ���ٸ� �߰�
    if (cells_[newCellX][newCellY].count(gameobject) == false) {
        cells_[newCellX][newCellY].emplace(gameobject);

        unsigned short oldX{}, oldY{};
        gameobject->GetPosition(oldX, oldY);

        //���� Sector���� ���� �����.
        unsigned short oldCellX = oldX / MAP_DEFINDS::CELL_SIZE;
        unsigned short oldCellY = oldY / MAP_DEFINDS::CELL_SIZE;
        cells_[oldCellX][oldCellY].erase(gameobject);
    }
}
