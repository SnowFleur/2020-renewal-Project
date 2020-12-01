#pragma once
#include<vector>
#include<unordered_set>
#include"../server/Protocol.h"
__interface IGameObject;


enum class COLLISION_TYPE {
    ATTACK, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN
};



class CSector {
private:
    std::unordered_set<IGameObject*>cells_[MAP_DEFINDS::SECTOR_SIZE][MAP_DEFINDS::SECTOR_SIZE];
    COLLISION_TYPE collisionType_;

    bool CompareDistanceObject(unsigned short lhsX, unsigned short lhsY,
        unsigned short rhsX, unsigned short rhsY, unsigned short length)const;

    void CheckSector(IGameObject* gameobject, unsigned short cellX, unsigned short cellY,
        bool& isCollision)const;

    inline bool CheckAttack(unsigned short lhsX, unsigned short lhsY,
        unsigned short rhsX, unsigned short rhsY)const;

    inline bool CheckMove(unsigned short lhsX, unsigned short lhsY,
        unsigned short rhsX, unsigned short rhsY)const;

public:
    CSector() = default;
    ~CSector();
    CSector(const CSector&) = delete;
    CSector(const CSector&&) = delete;
    CSector& operator=(const CSector&) = delete;
    CSector& operator=(const CSector&&) = delete;

    void Add(IGameObject* gameobject);
    void Remove(IGameObject* gameobject);
    bool Collision(IGameObject* gameobject, COLLISION_TYPE type);
    void ChangeSector(IGameObject* gameobject,
        const unsigned short newX, const unsigned short newY);
};




