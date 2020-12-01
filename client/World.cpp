#include "World.h"
#include"global.h"
#include<iostream>

/*
검은색이 나오는 이유가 기존의 타일위에 그리는게 아니라
타일을 지우고 건물을 그리기 때문에 검은색이 나오는 것이였음

해결방법은 두 가지가 있는데 타일따로 건물따로 그리기
혹은 그냥 처음부터 건물에 타일깔아놓기

전자는 복잡해지지만 나중에 유동성이 있음
후자는 단순하지만 해당 타일에만 건물이 있을 수 있다는 단점이 있음(후자로 진행)
*/

CWorld::CWorld(CSector* sector) :sectorHandle_(sector), tiles_{},
groundTerrain_{}, waterTerrain_{}, churchTerrain_{}{}

CWorld::~CWorld() {
    if (sectorHandle_ != nullptr) {
        delete sectorHandle_;
    }
}

void CWorld::GenerateTerrain() {

#pragma region Init Church 
    {
        // 교회 0: 땅, 1: 기둥, 2: 석상, 3: 테이블, 4: 벽, 5:스태인글라스
        bool move_able[6] = { true,false,false,false,false,false };
        for (int i = 0; i < 6; ++i) {
            churchTerrain_[i].InitData(1, move_able[i], L"resource/Church.PNG", i);
        }
    }

#pragma endregion 

#pragma region InitForest
    {
        //숲  0: 땅 1: 나무 2: 화돗불 3: 텐트 4: 꽃 5:나무들
        bool move_able[6] = { true,false,false,false,true,false };
        for (int i = 0; i < 6; ++i) {
            forestTerrain_[i].InitData(1, move_able[i], L"resource/Forest.PNG", i);
        }
    }
#pragma endregion
    GenerateChurch();
    GenerateForest();
}

void CWorld::GenerateChurch() {
#pragma region GenerateChurch
    /*
    0~15
    // 교회 0: 땅 1: 기둥 2: 석상 3: 테이블 4: 벽 5:스태인글라스
    */


    for (int x = 0; x < 15; ++x) {
        for (int y = 2; y < 15; ++y) {
            tiles_[x][y] = &churchTerrain_[0];
        }
    }

    //테이블, 성물, 테이블
    tiles_[3][2] = &churchTerrain_[2];
    tiles_[4][2] = &churchTerrain_[3];
    tiles_[5][2] = &churchTerrain_[2];


    //타워
    for (int i = 2; i < 6; ++i) {
        tiles_[2][i] = &churchTerrain_[1];
        tiles_[6][i] = &churchTerrain_[1];
    }

    //벽
    for (int x = 0; x < 15; ++x) {
        for (int y = 0; y < 2; ++y) {
            tiles_[x][y] = &churchTerrain_[4];
        }
    }

    //스태인글라스
    for (int x = 2; x < 7; ++x) {
        tiles_[x][0] = &churchTerrain_[5];

    }
#pragma endregion
}

void CWorld::GenerateForest() {
    /*
    15~30
    // 숲 0: 땅 1: 나무 2: 화돗불 3: 텐트 4: 꽃 5:나무들
    */
    for (int x = 15; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        for (int y = 0; y < 15; ++y) {
            tiles_[x][y] = &forestTerrain_[0];
        }
    }
    // 백그라운드 숲
    for (int x = 15; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        for (int y = 0; y < 2; ++y) {
            tiles_[x][y] = &forestTerrain_[5];
        }
    }
    //나무 세로줄
    for (int y = 3; y < 15; ++y) {
        tiles_[16][y] = &forestTerrain_[1];
    }

    //나무 가로줄 (1)
    for (int x = 16; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        if (x == 20 || x == 21)continue;
        tiles_[x][3] = &forestTerrain_[1];
    }

    //나무 가로줄 (2)
    for (int x = 16; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        if (x == 20 || x == 21 || x == 22)continue;
        tiles_[x][4] = &forestTerrain_[1];
    }
    //텐트 및 화돗불
    tiles_[22][6] = &forestTerrain_[3];
    tiles_[22][7] = &forestTerrain_[2];

    //꽃
    tiles_[17][6] = &forestTerrain_[4];
    tiles_[18][6] = &forestTerrain_[4];
    tiles_[18][7] = &forestTerrain_[4];
    tiles_[20][8] = &forestTerrain_[4];
    tiles_[20][5] = &forestTerrain_[4];
    tiles_[21][5] = &forestTerrain_[4];
}


void CWorld::Draw() {
    //매번 Draw 할때마다 위치값을 변경할 필요는 없다. 
    //일단 여기에 테스트만 하고 나중에 옮기자

    for (int x = 0; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        for (int y = 0; y < MAP_DEFINDS::SECTOR_SIZE; ++y) {

            if (tiles_[x][y] == nullptr)continue;
            tiles_[x][y]->SetTexturePosition(x, y);
            /*
            20.11.17 VS2019 버전(19.8.1)업데이트하고 생긴 오류 C2102 
            Draw_BOB32(&tiles_[x][y]->GetTexture());
            */
            auto texture=tiles_[x][y]->GetTexture();
            Draw_BOB32(&texture);
        }
    }
}


const CTerrian* CWorld::GetTile(int x, int y)const noexcept {
    if (x >= 0 && y >= 0) {
        return tiles_[x][y];
    }
}

CSector* CWorld::GetSectorHandle()const noexcept {
    if (sectorHandle_ != nullptr) {
        return sectorHandle_;
    }
}
