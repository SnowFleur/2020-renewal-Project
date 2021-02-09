#include "Navigation.h"
#include"Protocol.h"

CNavigation::CNavigation() {
    /*cells_.assign(MAP_DEFINDS::WORLD_WIDTH, std::vector<Cell>(MAP_DEFINDS::WORLD_HEIGHT,
        Cell{ CELL_TYPE::GROUND,MAX_WEIGHT }));*/
}

bool CNavigation::SafetyCheck(const int x, const int y)const {
    if ((0 <= x && x < MAP_DEFINDS::SECTOR_X) && (0 <= y && y < MAP_DEFINDS::SECTOR_Y))
        return true;
    else
        return false;
}

void CNavigation::SetTileType(const int x, const int y, TILE_TYPE type) {
    if (SafetyCheck(x, y) == true) {
        tiles_[x][y].tileType_ = type;
    }
}

TILE_TYPE CNavigation::GetTileType(const int x, const int y)const {
    if (SafetyCheck(x, y) == true) {
        return tiles_[x][y].tileType_;
    }
}

void CNavigation::SetWeight(const int x, const int y, const int weight) {
    if ((0 <= x && x <= MAP_DEFINDS::SECTOR_X) && (0 <= y && y <= MAP_DEFINDS::SECTOR_Y)) {
        tiles_[x][y].weight_ = weight;
    }
}

int CNavigation::GetWeight(const int x, const int y)const {
    if ((0 <= x && x <= MAP_DEFINDS::SECTOR_X) && (0 <= y && y <= MAP_DEFINDS::SECTOR_Y)) {
        return tiles_[x][y].weight_;
    }
}

void CNavigation::ResetData() {
    for (int x = 0; x < MAP_DEFINDS::SECTOR_X; ++x) {
        for (int y = 0; y < MAP_DEFINDS::SECTOR_Y; ++y) {
            tiles_[x][y].tileType_ = TILE_TYPE::GROUND;
            tiles_[x][y].weight_ = MAX_WEIGHT;
        }
    }
}

void CNavigation::SetMapData(const int NumberOfSector, const int weight, const int height) {

    // 나중에 JSON으로 변경
    //0: 성당, 1: 숲
    switch (NumberOfSector) {
    case 0: {
        tiles_.assign(weight, std::vector<Tile>(height, Tile{ TILE_TYPE::GROUND,MAX_WEIGHT }));

        //테이블, 성물, 테이블
        tiles_[3][2] = Tile{TILE_TYPE::WALL,MAX_WEIGHT };
        tiles_[4][2] = Tile{TILE_TYPE::WALL,MAX_WEIGHT };
        tiles_[5][2] = Tile{TILE_TYPE::WALL,MAX_WEIGHT };


        //타워
        for (int i = 2; i < 6; ++i) {
            tiles_[2][i] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };
            tiles_[6][i] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };
        }

        //벽
        for (int x = 0; x < 15; ++x) {
            for (int y = 0; y < 2; ++y) {
                tiles_[x][y] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };
            }
        }

        //스태인글라스
        for (int x = 2; x < 7; ++x) {
            tiles_[x][0] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };
        }

        break;
    }
    case 1: {
        tiles_.assign(weight, std::vector<Tile>(height, Tile{ TILE_TYPE::GROUND,MAX_WEIGHT }));

        //15개씩 컷

        //0==15

        // 백그라운드 숲
        for (int x = 0; x < weight; ++x) {
            for (int y = 0; y < 2; ++y) {
                tiles_[x][y] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };
            }
        }

        //나무 세로줄
        for (int y = 3; y < 15; ++y) {
            tiles_[1][y] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };
        }

        //나무 가로줄 (1)
        for (int x = 1; x < weight; ++x) {
            if (x == 5 || x == 6)continue;
            tiles_[x][3] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };
        }

        //나무 가로줄 (2)
        for (int x = 1; x < weight; ++x) {
            if (x == 5 || x == 6 || x == 7)continue;
            tiles_[x][4] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };
        }
        //텐트 및 화돗불
        tiles_[7][6] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };
        tiles_[7][7] = Tile{ TILE_TYPE::WALL,MAX_WEIGHT };

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