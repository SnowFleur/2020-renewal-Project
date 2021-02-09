#pragma once
#include<vector>
#include"SRWLock.h"


constexpr int MAX_WEIGHT = 50000;

enum class TILE_TYPE {
    PLAYER = 1,
    GROUND = 2,
    WALL = 3,
};

class Tile{
private:
public:
    TILE_TYPE     tileType_;
    int           weight_;
};
using Tiles = std::vector<std::vector<Tile>>;

class CNavigation {

private:
    CSRWLock        srwLock;
    Tiles           tiles_;
    inline bool     SafetyCheck(const int x, const int y)const;
public:
    CNavigation();
    ~CNavigation() = default;
    CNavigation(const CNavigation&) = default;
    CNavigation(CNavigation&&) = default;
    CNavigation& operator=(const CNavigation&) = default;
    CNavigation& operator=(CNavigation&&) = default;

    void        SetTileType(const int x, const int y, TILE_TYPE type);
    TILE_TYPE   GetTileType(const int x, const int y)const;
    void        SetWeight(const int x, const int y, const int weight);
    int         GetWeight(const int x, const int y)const;
    void        ResetData();
    void        SetMapData(const int NumberOfSector, const int weight, const int height);


};


