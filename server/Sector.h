#pragma once
#include<unordered_set>
#include<array>
#include"Protocol.h"
#include"DataType.h"
#include"EventType.h"

#define VIEW_SIZE		  7
constexpr int NUMBER_OF_SEARCH = 9;

class CGameObject;
using SectorDir      = std::pair<short, short>;
using Cells          = std::unordered_set<ObjectIDType>;
using GameObjects    = std::array<CGameObject*, OBJECT_DEFINDS::MAX_GAMEOBJECT>;

/*
Server의 Object들을 관리하는 Sector Class
*/
class CSector {
private:
    void         InitMonsterForJson();
public:
    CSector();
    ~CSector();
public:
    Cells        cells_[MAP_DEFINDS::SECTOR_SIZE][MAP_DEFINDS::SECTOR_SIZE];
    GameObjects  gameobjects_;
    SectorDir    searchDirection_[NUMBER_OF_SEARCH]; //8방향 탐색용 Pair변수

    void        AddObject(const ObjectIDType id, const PositionType x, const PositionType y);
    void        MoveObject(const ObjectIDType id, const PositionType x, const PositionType y,
                const TextureDirection textureDirection);
    bool        IsNearObject(const ObjectIDType lhs, const ObjectIDType rhs);
    bool        IsMonster(const ObjectIDType id)const;
    bool        WakeUpNearMonster(const ObjectIDType montserID, const ObjectIDType playerID);
    void        StartMovedMonster(const ObjectIDType montserID, const ObjectIDType playerID);
    void        ProcessEvent(EVENT_ST& ev);
    void        GetViewListByCell(std::unordered_set<ObjectIDType>& view, PositionType x, PositionType y);
};


