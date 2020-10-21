#pragma once
#include<unordered_set>
#include<array>
#include"Protocol.h"
#include"DataType.h"




struct GameObject;
class CPlayer;
class CMonster;
class CNpc;

/*
Server의 Object들을 관리하는 Sector Class
*/
class CSector {
private:
public:
    std::unordered_set<GameObject*>cells_[MAP_DEFINDS::MAP_SIZE][MAP_DEFINDS::MAP_SIZE];
    std::array<CPlayer*, OBJECT_DEFINDS::MAX_USER> players_;
    std::array<CMonster*, OBJECT_DEFINDS::MAX_MONSER> monsters_;
    std::array<CNpc*, OBJECT_DEFINDS::MAX_NPC> npcs_;

    void        GetCellObject(PositionType x, PositionType y);
    void        SetCellObject(PositionType x, PositionType y);

    CSector();
    ~CSector();
};


