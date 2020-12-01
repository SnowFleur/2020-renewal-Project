#include "World.h"
#include"global.h"
#include<iostream>

/*
�������� ������ ������ ������ Ÿ������ �׸��°� �ƴ϶�
Ÿ���� ����� �ǹ��� �׸��� ������ �������� ������ ���̿���

�ذ����� �� ������ �ִµ� Ÿ�ϵ��� �ǹ����� �׸���
Ȥ�� �׳� ó������ �ǹ��� Ÿ�ϱ�Ƴ���

���ڴ� ������������ ���߿� �������� ����
���ڴ� �ܼ������� �ش� Ÿ�Ͽ��� �ǹ��� ���� �� �ִٴ� ������ ����(���ڷ� ����)
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
        // ��ȸ 0: ��, 1: ���, 2: ����, 3: ���̺�, 4: ��, 5:�����α۶�
        bool move_able[6] = { true,false,false,false,false,false };
        for (int i = 0; i < 6; ++i) {
            churchTerrain_[i].InitData(1, move_able[i], L"resource/Church.PNG", i);
        }
    }

#pragma endregion 

#pragma region InitForest
    {
        //��  0: �� 1: ���� 2: ȭ���� 3: ��Ʈ 4: �� 5:������
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
    // ��ȸ 0: �� 1: ��� 2: ���� 3: ���̺� 4: �� 5:�����α۶�
    */


    for (int x = 0; x < 15; ++x) {
        for (int y = 2; y < 15; ++y) {
            tiles_[x][y] = &churchTerrain_[0];
        }
    }

    //���̺�, ����, ���̺�
    tiles_[3][2] = &churchTerrain_[2];
    tiles_[4][2] = &churchTerrain_[3];
    tiles_[5][2] = &churchTerrain_[2];


    //Ÿ��
    for (int i = 2; i < 6; ++i) {
        tiles_[2][i] = &churchTerrain_[1];
        tiles_[6][i] = &churchTerrain_[1];
    }

    //��
    for (int x = 0; x < 15; ++x) {
        for (int y = 0; y < 2; ++y) {
            tiles_[x][y] = &churchTerrain_[4];
        }
    }

    //�����α۶�
    for (int x = 2; x < 7; ++x) {
        tiles_[x][0] = &churchTerrain_[5];

    }
#pragma endregion
}

void CWorld::GenerateForest() {
    /*
    15~30
    // �� 0: �� 1: ���� 2: ȭ���� 3: ��Ʈ 4: �� 5:������
    */
    for (int x = 15; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        for (int y = 0; y < 15; ++y) {
            tiles_[x][y] = &forestTerrain_[0];
        }
    }
    // ��׶��� ��
    for (int x = 15; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        for (int y = 0; y < 2; ++y) {
            tiles_[x][y] = &forestTerrain_[5];
        }
    }
    //���� ������
    for (int y = 3; y < 15; ++y) {
        tiles_[16][y] = &forestTerrain_[1];
    }

    //���� ������ (1)
    for (int x = 16; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        if (x == 20 || x == 21)continue;
        tiles_[x][3] = &forestTerrain_[1];
    }

    //���� ������ (2)
    for (int x = 16; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        if (x == 20 || x == 21 || x == 22)continue;
        tiles_[x][4] = &forestTerrain_[1];
    }
    //��Ʈ �� ȭ����
    tiles_[22][6] = &forestTerrain_[3];
    tiles_[22][7] = &forestTerrain_[2];

    //��
    tiles_[17][6] = &forestTerrain_[4];
    tiles_[18][6] = &forestTerrain_[4];
    tiles_[18][7] = &forestTerrain_[4];
    tiles_[20][8] = &forestTerrain_[4];
    tiles_[20][5] = &forestTerrain_[4];
    tiles_[21][5] = &forestTerrain_[4];
}


void CWorld::Draw() {
    //�Ź� Draw �Ҷ����� ��ġ���� ������ �ʿ�� ����. 
    //�ϴ� ���⿡ �׽�Ʈ�� �ϰ� ���߿� �ű���

    for (int x = 0; x < MAP_DEFINDS::SECTOR_SIZE; ++x) {
        for (int y = 0; y < MAP_DEFINDS::SECTOR_SIZE; ++y) {

            if (tiles_[x][y] == nullptr)continue;
            tiles_[x][y]->SetTexturePosition(x, y);
            /*
            20.11.17 VS2019 ����(19.8.1)������Ʈ�ϰ� ���� ���� C2102 
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
