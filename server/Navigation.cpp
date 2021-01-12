#include "Navigation.h"
#include"Protocol.h"

CNavigation::CNavigation() {
    /*cells_.assign(MAP_DEFINDS::WORLD_WIDTH, std::vector<Cell>(MAP_DEFINDS::WORLD_HEIGHT,
        Cell{ CELL_TYPE::GROUND,MAX_WEIGHT }));*/
}

bool CNavigation::SafetyCheck(const int x, const int y)const {
    if ((0 <= x && x <= MAP_DEFINDS::SECTOR_X) && (0 <= y && y <= MAP_DEFINDS::SECTOR_Y))
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
    if ((0 <= x && x <= MAP_DEFINDS::SECTOR_X) && (0 <= y && y <= MAP_DEFINDS::SECTOR_Y)) {
        cells_[x][y].weight_ = weight;
    }
}

int CNavigation::GetWeight(const int x, const int y)const {
    if ((0 <= x && x <= MAP_DEFINDS::SECTOR_X) && (0 <= y && y <= MAP_DEFINDS::SECTOR_Y)) {
        return cells_[x][y].weight_;
    }
}

void CNavigation::ResetData() {
    for (int x = 0; x < MAP_DEFINDS::SECTOR_X; ++x) {
        for (int y = 0; y < MAP_DEFINDS::SECTOR_Y; ++y) {
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


void CNavigation::SetMapData(const int NumberOfSector,const int weight,const int height) {

    //0: 성당, 1: 숲
    switch (NumberOfSector) {
    case 0: {
        cells_.assign(weight, std::vector<Cell>(height,Cell{ CELL_TYPE::GROUND,MAX_WEIGHT }));

        //테이블, 성물, 테이블
        cells_[3][2] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
        cells_[4][2] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
        cells_[5][2] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };


        //타워
        for (int i = 2; i < 6; ++i) {
            cells_[2][i] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
            cells_[6][i] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
        }





        //벽
        for (int x = 0; x < 15; ++x) {
            for (int y = 0; y < 2; ++y) {
                cells_[x][y] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
            }
        }

        //스태인글라스
        for (int x = 2; x < 7; ++x) {
            cells_[x][0] =Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
        }



        break;
    }
    case 1: {
        cells_.assign(weight, std::vector<Cell>(height, Cell{ CELL_TYPE::GROUND,MAX_WEIGHT }));

        //15개씩 컷

        //0==15

        // 백그라운드 숲
        for (int x = 0; x < weight; ++x) {
            for (int y = 0; y < 2; ++y) {
                cells_[x][y] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
            }
        }

        //나무 세로줄
        for (int y = 3; y < 15; ++y) {
            cells_[1][y] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
        }

        //나무 가로줄 (1)
        for (int x = 1; x < weight; ++x) {
            if (x == 5 || x == 6)continue;
            cells_[x][3] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
        }

        //나무 가로줄 (2)
        for (int x = 1; x < weight; ++x) {
            if (x == 5 || x == 6 || x == 7)continue;
            cells_[x][4] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
        }
        //텐트 및 화돗불
        cells_[7][6] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };
        cells_[7][7] = Cell{ CELL_TYPE::WALL,MAX_WEIGHT };

        break;
    }
    case 2: {
        break;
    }
    case 3: {
        break;
    }
    default:
        break;
    }

}