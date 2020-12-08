#pragma once
#include<unordered_set>
#include<array>
#include"Protocol.h"
#include"DataType.h"
#include"EventType.h"

#define VIEW_SIZE		  7
constexpr int NUMBER_OF_SEARCH = 9;


struct GameObject;
class  CPlayer;
class  CMonster;
class  CNpc;

using SectorDir      = std::pair<short, short>;
using Players        = std::array<CPlayer*, OBJECT_DEFINDS::MAX_USER>;
using Monsters       = std::array<CMonster*, OBJECT_DEFINDS::MAX_MONSER>;
using Npcs           = std::array<CNpc*, OBJECT_DEFINDS::MAX_NPC>;
using Cells          = std::unordered_set<ObjectIDType>;
/*
Server의 Object들을 관리하는 Sector Class
*/
class CSector {
private:
    void         InitMonsterForJson();
public:
    Cells        cells_[MAP_DEFINDS::SECTOR_SIZE][MAP_DEFINDS::SECTOR_SIZE];
    Players      players_;
    Monsters     monsters_;
    Npcs         npcs_;
    SectorDir    searchDirection_[NUMBER_OF_SEARCH]; //8방향 탐색용 Pair변수

    void        AddObject(const ObjectIDType id, const ObjectClass type, const PositionType x, const PositionType y);
    void        MoveObject(const ObjectIDType id, const PositionType x, const PositionType y,
                 const TextureDirection textureDirection);
    bool        IsNearMonsterAndPlayer(const ObjectIDType montserID, const ObjectIDType playerID);
    bool        IsNearPlayerAndPlayer(const ObjectIDType lhs, const ObjectIDType rhs);
    bool        IsMonster(const ObjectIDType id);
    bool        SafeCheckUsedInArray(const ObjectClass type,const ObjectIDType id);

    bool        TestFunction(const ObjectIDType montserID, const ObjectIDType playerID);
#pragma region Monster Functions
    bool        WakeUpNearMonster(const ObjectIDType montserID,const ObjectIDType playerID);
    void        ProcessEvent(EVENT_ST& ev);
#pragma endregion

    CSector();
    ~CSector();
};


