#pragma once
#include<vector>
#include"SRWLock.h"

constexpr int MAX_WEIGHT = 50000;

enum class CELL_TYPE {
    PLAYER=1,
    GROUND=2,
    WALL=3,
};

class Cell {
private:
public:
    CELL_TYPE   cellType_;
    int         weight_;
};

using Cells = std::vector<std::vector<Cell>>;

class CNavigation{
private:
    CSRWLock        srwLock;
    Cells           cells_;
    //Cheeses         cheeses_;
    inline bool     SafetyCheck(const int x, const int y)const;
public:
    CNavigation();
    ~CNavigation() = default;
    CNavigation(const CNavigation&) = default;
    CNavigation(CNavigation&&) = default;
    CNavigation& operator=(const CNavigation&) = default;
    CNavigation& operator=(CNavigation&&) = default;

    void        SetCellType(const int x, const int y, CELL_TYPE type);
    CELL_TYPE   GetCellType(const int x, const int y)const;
    void        SetWeight(const int x, const int y, const int weight);
    int         GetWeight(const int x, const int y)const;
    void        ResetData();
    void        SetMapData(const int NumberOfSector, const int weight, const int height);

    //void        SetCheese(Position && cheese);
    //Cheeses     GetCheeses()const;
};


