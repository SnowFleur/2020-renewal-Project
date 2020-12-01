#include<algorithm>
#include<iostream>
#include "Sector.h"
#include"GameObject.h"
/*
규칙: 모든 cell 계산은 함수안에서 하고 밖에서 가져오는 좌표는
오브젝트 고유의 좌표를 입력한다.
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
    //Celle에 Object Add
    cells_[cellX][cellY].emplace(gameobject);
}

void CSector::Remove(IGameObject* gameobject) {
    unsigned short objectX{}, objectY{};
    gameobject->GetPosition(objectX, objectY);

    unsigned short cellX = objectX / MAP_DEFINDS::CELL_SIZE;
    unsigned short cellY = objectY / MAP_DEFINDS::CELL_SIZE;
    //Celle에 Object erase
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
    Object가 있는 Sector와 주위의 4개 Sector를 Check
    */
    CheckSector(gameobject, cellX, cellY, isCollision);
    //왼쪽
    if (cellX - 1 >= 0)
        CheckSector(gameobject, cellX - 1, cellY, isCollision);
    //오른쪽
    if (cellX + 1 <= MAP_DEFINDS::SECTOR_SIZE)
        CheckSector(gameobject, cellX + 1, cellY, isCollision);
    //위쪽
    if (cellY - 1 >= 0)
        CheckSector(gameobject, cellX, cellY - 1, isCollision);
    //아래쪽
    if (cellY + 1 <= MAP_DEFINDS::SECTOR_SIZE)
        CheckSector(gameobject, cellX, cellY + 1, isCollision);


    // 데미지 줄이고
    // 캐릭터 겹치기 X
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
    //상
    if (lhsX == rhsX && lhsY - 1 == rhsY) {
        std::cout << "Collision Attack 상\n";
    }
    //하
    else if (lhsX == rhsX && lhsY + 1 == rhsY) {
        std::cout << "Collision Attack 하\n";
    }
    //좌
    else if (lhsY == rhsY && lhsX - 1 == rhsX) {
        std::cout << "Collision Attack 좌\n";
    }
    //우
    else if (lhsY == rhsY && lhsX + 1 == rhsX) {
        std::cout << "Collision Attack 우\n";
    }
    return true;
}

inline bool CSector::CheckMove(unsigned short lhsX, unsigned short lhsY,
    unsigned short rhsX, unsigned short rhsY)const {

    switch (collisionType_) {
    case COLLISION_TYPE::MOVE_LEFT:
        if (lhsY == rhsY && lhsX - 1 == rhsX) {
            std::cout << "Collision Move 좌\n";
            return true;
        }
        break;
    case COLLISION_TYPE::MOVE_RIGHT:
        if (lhsY == rhsY && lhsX + 1 == rhsX) {
            std::cout << "Collision Move 우\n";
            return true;
        }
        break;
    case COLLISION_TYPE::MOVE_UP:
        //상
        if (lhsX == rhsX && lhsY - 1 == rhsY) {
            std::cout << "Collision Move 상\n";
            return true;
        }

        break;
    case COLLISION_TYPE::MOVE_DOWN:
        //하
        if (lhsX == rhsX && lhsY + 1 == rhsY) {
            std::cout << "Collision Move 하\n";
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

    //새로 이동한 좌표에 해당 오브젝트가 없다면 추가
    if (cells_[newCellX][newCellY].count(gameobject) == false) {
        cells_[newCellX][newCellY].emplace(gameobject);

        unsigned short oldX{}, oldY{};
        gameobject->GetPosition(oldX, oldY);

        //이전 Sector에서 값을 지운다.
        unsigned short oldCellX = oldX / MAP_DEFINDS::CELL_SIZE;
        unsigned short oldCellY = oldY / MAP_DEFINDS::CELL_SIZE;
        cells_[oldCellX][oldCellY].erase(gameobject);
    }
}
