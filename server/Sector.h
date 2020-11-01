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
    std::unordered_set<ObjectIDType>cells_[MAP_DEFINDS::MAP_SIZE][MAP_DEFINDS::MAP_SIZE];
    std::array<CPlayer*,OBJECT_DEFINDS::MAX_USER>       players_;
    std::array<CMonster*,OBJECT_DEFINDS::MAX_MONSER>    monsters_;
    std::array<CNpc*, OBJECT_DEFINDS::MAX_NPC>          npcs_;

    void        GetCellObject(const PositionType x, const PositionType y);
    void        SetCellObject(const PositionType x, const PositionType y);
    void        AddObject(const ObjectIDType id, const PositionType x, const PositionType y);
    void        MoveObject(const ObjectIDType id, const PositionType x, const PositionType y,
                const TextureDirection textureDirection);

    CSector();
    ~CSector();
};


