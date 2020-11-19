#pragma once
#include<unordered_set>
#include<array>
#include"Protocol.h"
#include"DataType.h"
#include"EventType.h"

struct GameObject;
class  CPlayer;
class  CMonster;
class  CNpc;
class  CTimerThread;

using SectorDir = std::pair<short, short>;
constexpr int NUMBER_OF_SEARCH = 9;

/*
Server의 Object들을 관리하는 Sector Class
*/
class CSector {
private:
public:
    std::unordered_set<ObjectIDType>cells_[MAP_DEFINDS::SECTOR_SIZE][MAP_DEFINDS::SECTOR_SIZE];

    std::array<CPlayer*,OBJECT_DEFINDS::MAX_USER>       players_;
    std::array<CMonster*,OBJECT_DEFINDS::MAX_MONSER>    monsters_;
    std::array<CNpc*, OBJECT_DEFINDS::MAX_NPC>          npcs_;
    SectorDir                                           searchDirection_[]; //8방향 탐색용 Pair변수

    void        GetCellObject(const PositionType x, const PositionType y);
    void        SetCellObject(const PositionType x, const PositionType y);
    void        AddObject(const ObjectIDType id, const ObjectClass type, const PositionType x, const PositionType y);
    void        MoveObject(const ObjectIDType id, const PositionType x, const PositionType y,
                 const TextureDirection textureDirection);
    bool        IsNearMonsterAndPlayer(const ObjectIDType montserID, const ObjectIDType playerID);
    bool        IsNearPlayerAndPlayer(const ObjectIDType lhs, const ObjectIDType rhs);
    bool        IsMonster(const ObjectIDType id);
    bool        TestFunction(const ObjectIDType montserID, const ObjectIDType playerID);

#pragma region Monster Functions
    bool        WakeUpNearMonster(const ObjectIDType montserID,const ObjectIDType playerID);
    void        ProcessEvent(EVENT_ST& ev);
#pragma endregion

    CSector();
    ~CSector();
};


