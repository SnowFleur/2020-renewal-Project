#pragma once
#include"Terrian.h"

/*
Wolrd�� Sector�� ������ ������ ��
*/

class CSector;

class CWorld {
private:
    CTerrian*         tiles_[MAP_DEFINDS::SECTOR_SIZE][MAP_DEFINDS::SECTOR_SIZE];
    CTerrian          groundTerrain_;
    CTerrian          waterTerrain_;
    CTerrian          churchTerrain_[6];
    CTerrian          forestTerrain_[6];
    CSector*          sectorHandle_ = nullptr;

private:
    void              GenerateChurch();
    void              GenerateForest();
public:
    CWorld(CSector* sector);
    ~CWorld();

    CWorld() = delete;
    CWorld(const CWorld&) = delete;
    CWorld(const CWorld&&) = delete;
    CWorld& operator=(const CWorld&) = delete;
    CWorld& operator=(const CWorld&&) = delete;

    void                GenerateTerrain();
    void                Draw();
    const CTerrian*     GetTile(int x, int y)const noexcept;
    CSector*            GetSectorHandle()const noexcept;
};

