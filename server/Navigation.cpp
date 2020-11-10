#include "Navigation.h"
#include"Protocol.h"

CNavigation::CNavigation() {
    cells_.assign(MAP_DEFINDS::WORLD_WIDTH, std::vector<Cell>(MAP_DEFINDS::WORLD_HEIGHT,
        Cell{ CELL_TYPE::GROUND,MAX_WEIGHT }));




}

bool CNavigation::SafetyCheck(const int x, const int y)const {
    if ((0 <= x && x <= MAP_DEFINDS::WORLD_WIDTH) && (0 <= y && y <= MAP_DEFINDS::WORLD_HEIGHT))
        return true;
    else
        return false;
}

void CNavigation::SetCellType(const int x, const int y, CELL_TYPE type) {
    if (SafetyCheck(x, y) == true) {
        cells_[x][y].cellType_ = type;
    }
}

CELL_TYPE CNavigation::GetCellType(const int x, const int y)const {
    if (SafetyCheck(x, y) == true) {
        return cells_[x][y].cellType_;
    }
}

void CNavigation::SetWeight(const int x, const int y, const int weight) {
    if ((0 <= x && x <= MAP_DEFINDS::WORLD_WIDTH) && (0 <= y && y <= MAP_DEFINDS::WORLD_HEIGHT)) {
        cells_[x][y].weight_ = weight;
    }
}

int CNavigation::GetWeight(const int x, const int y)const {
    if ((0 <= x && x <= MAP_DEFINDS::WORLD_WIDTH) && (0 <= y && y <= MAP_DEFINDS::WORLD_HEIGHT)) {
        return cells_[x][y].weight_;
    }
}

void CNavigation::ResetData() {
    for (int x = 0; x < MAP_DEFINDS::WORLD_WIDTH; ++x) {
        for (int y = 0; y < MAP_DEFINDS::WORLD_HEIGHT; ++y) {
            cells_[x][y].cellType_ = CELL_TYPE::GROUND;
            cells_[x][y].weight_ = MAX_WEIGHT;
        }
    }
   // cheeses_.clear();
}

#pragma region Get/Set Cheese
//void CNavigation::SetCheese(Position&& cheese) {
//    cheeses_.emplace_back(cheese);
//}
//
//Cheeses CNavigation::GetCheeses()const {
//    return cheeses_;
//}
#pragma endregion