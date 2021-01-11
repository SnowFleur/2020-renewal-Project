#pragma warning(disable: 4996) //JSON ���� ����
#include<algorithm>
#include<fstream>
#include"Sector.h"
#include"Player.h"
#include"Network.h"
#include"Monster.h"
#include"..\JSON\json\json.h"
#include"PlayerInputComponent.h"
#include"MonsterInputComponent.h"
#include"TimerQueue.h"

CSector::CSector() {

    //8���� Ž����.. �ʱ�ȭ�� �ʹ� ������.
    searchDirection_[0] = SectorDir{ -1,-1 };
    searchDirection_[1] = SectorDir{ 0,-1 };
    searchDirection_[2] = SectorDir{ 1,-1 };
    searchDirection_[3] = SectorDir{ -1,0 };
    searchDirection_[4] = SectorDir{ 0,0 };
    searchDirection_[5] = SectorDir{ 1,0 };
    searchDirection_[6] = SectorDir{ -1,1 };
    searchDirection_[7] = SectorDir{ 0,1 };
    searchDirection_[8] = SectorDir{ 1,1 };


    //�ϴ� ���⼭ �����Ҵ��� �ް� ���߿� Ǯ���� ����
    for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        players_[i] = new CPlayer(PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y,100,1,0,1,new CPlayerInputComponent);
    }
    InitMonsterForJson();
}

CSector::~CSector() {}

void CSector::InitMonsterForJson() {

    std::ifstream openjsonFile("../JSON/Data/MonsterData.json");
    Json::Reader reader;
    Json::Value root;

    if (reader.parse(openjsonFile, root) == false)
        CLogCollector::GetInstance()->PrintLog("Json File Open is Fail");

    //ORC 0~100, ZOMBLE 101~200, MUMMY 201~300, BAT 301~400
    for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_MONSER; ++i) {
        HpType hp{}; LevelType level{}; ExpType exp{}; DamageType damage{};

        if (i < 100) {
            hp = root["Orc"]["INFOR"].get("HP", -1).asInt();
            level = root["Orc"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Orc"]["INFOR"].get("EXP", -1).asInt();
            damage = root["Orc"]["INFOR"].get("DAMAGE", -1).asInt();
            monsters_[i] = new CMonster(MonsterType::ORC, 0, 0, hp, level, exp, damage, new CMonsterInputComponent);

        }
        else if (i < 200) {
            monsters_[i] = new CMonster(MonsterType::ZOMBIE, 0, 0, hp, level, exp, damage, new CMonsterInputComponent);
        }
        else if (i < 300) {
            monsters_[i] = new CMonster(MonsterType::MUMMY, 0, 0, hp, level, exp, damage, new CMonsterInputComponent);

        }
        else if (i < 400) {
            hp = root["Bat"]["INFOR"].get("HP", -1).asInt();
            level = root["Bat"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Bat"]["INFOR"].get("EXP", -1).asInt();
            damage = root["Bat"]["INFOR"].get("DAMAGE", -1).asInt();
            monsters_[i] = new CMonster(MonsterType::BAT, 0, 0, hp, level, exp, damage, new CMonsterInputComponent);
        }
        /*
        2020.11.19
        ���⼭�� �����ΰ͸� �˸� �Ǳ� ������(�������� ��ũ���� ����� ��)
        OBJECT_DEFINEDS �� ������� �־��ش�.
        */
        AddObject(i, OBJECT_DEFINDS::MONSTER, PRIMARY_MONSTER_X, PRIMARY_MONSTER_Y);
    }
    openjsonFile.close();
    CLogCollector::GetInstance()->PrintLog("Monster Init And Add In Sector");
    monsters_[0]->isUsed_ = true;
}

/*
2020.11.19 �� �Լ����� Monster�� player�� Index�� �����ؼ� �ְ� ������ �� �ٸ��ʿ��� �Ű��� �����ʵ��� ����
*/
void CSector::AddObject(const ObjectIDType id, const ObjectClass type,const PositionType x, const PositionType y) {
    PositionType cx = x / MAP_DEFINDS::CELL_SIZE;
    PositionType cy = y / MAP_DEFINDS::CELL_SIZE;

    switch (type) {
    case OBJECT_DEFINDS::OTHER_PLAYER: {
        players_[id]->x_ = x;
        players_[id]->y_ = y;
        cells_[cx][cy].emplace(id);
        break;
    }
    case OBJECT_DEFINDS::MONSTER: {
        monsters_[id]->x_ = x;
        monsters_[id]->y_ = y;
        cells_[cx][cy].emplace(id + OBJECT_DEFINDS::MAX_USER);
        break;
    }
    default:
        CLogCollector::GetInstance()->PrintLog("Not Defined Type");
        break;
    }
}

void CSector::MoveObject(const ObjectIDType id, const PositionType newX, const PositionType newY,
    const TextureDirection textureDirection) {

    PositionType newCellX = newX / MAP_DEFINDS::CELL_SIZE;
    PositionType newCellY = newY / MAP_DEFINDS::CELL_SIZE;

    players_[id]->srwLock_.Readlock();
    std::unordered_set<ObjectIDType>oldView = players_[id]->viewLIst_;
    players_[id]->srwLock_.Readunlock();

    //���� �̵��� ��ǥ�� �ش� ID�� ���ٸ� �߰�
    //20.11.17: �ٸ� �����尡 ���ͼ� Cells�� �ǵ� �� ������ ���߿� ó��
    if (cells_[newCellX][newCellY].count(id) == false) {
        cells_[newCellX][newCellY].emplace(id);

        PositionType oldX = players_[id]->x_;
        PositionType oldY = players_[id]->y_;

        players_[id]->x_ = newX;
        players_[id]->y_ = newY;

        //���� Sector���� ���� �����.
        PositionType oldCellX = oldX / MAP_DEFINDS::CELL_SIZE;
        PositionType oldCellY = oldY / MAP_DEFINDS::CELL_SIZE;
        cells_[oldCellX][oldCellY].erase(id);
    }
    //�̹� Sector�ȿ� �ִ� ID��� �׳� �̵����� ����
    else {
        players_[id]->x_ = newX;
        players_[id]->y_ = newY;
    }

    //�ش� Sector�� �ִ� Object ����
    std::unordered_set <ObjectIDType> nearList;

    //�ش� Sector �ֺ��� �ִ� 8������ Sector ������ NearList�� ����
    for (PositionType i = 0; i < NUMBER_OF_SEARCH; ++i) {
        short x = newCellX + searchDirection_[i].first;
        short y = newCellY + searchDirection_[i].second;
        if (x < 0 || y < 0)continue;

        for (auto i : cells_[x][y]) {
            /*
            2020.11.19 �̰� �ּ��ΰ�...
            */
            if (IsMonster(i) == true) {
                if (SafeCheckUsedInArray(OBJECT_DEFINDS::MONSTER, i) == false)continue;
                if (IsNearMonsterAndPlayer(i, id) == false)continue;
            }
            else {
                if (SafeCheckUsedInArray(OBJECT_DEFINDS::OTHER_PLAYER, i) == false)continue;
                if (IsNearPlayerAndPlayer(i, id) == false)continue;
            }
            nearList.emplace(i);
        }
    }
    //�� ��������
    nearList.erase(id);

    for (auto iter : nearList) {

        //�̵��� ���� oldView�� NearList�� ������ �ִٸ�
        if (oldView.find(iter) != oldView.end()) {

            //���� �ִ� �������� Ȯ�� �� �̵� ����
            StartMovedMonster(iter, id);

            //Object�� ����� ����ΰ�� �ؿ� ���� ����
            if (IsMonster(iter) == true)continue;

            //��� viewList�� ���� ������
            players_[iter]->srwLock_.Readlock();
            if (players_[iter]->viewLIst_.find(id) != players_[iter]->viewLIst_.end()) {
                //�̵��� �÷��̾��� ������ �þ߾ȿ� �ִ� �÷��̾�� ����
                players_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER,
                    textureDirection);
            }
            //��� viewList�� ���� ������
            else {
                //read UnLock
                players_[iter]->srwLock_.Readunlock();

                //View List�� ���
                players_[iter]->srwLock_.Writelock();
                players_[iter]->viewLIst_.insert(id);
                players_[iter]->srwLock_.Writeunlock();

                //�̵��� �÷��̾��� ������ �þ߾ȿ� �ִ� �÷��̾�� ����
                NETWORK::SendAddObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER);
            }
        }
        //�̵��� ���� oldview�� NearList�� ������ ���ٸ� 
        else {
            //���� ViewLIst�� ������� ���
            players_[id]->srwLock_.Writelock();
            players_[id]->viewLIst_.insert(iter);
            players_[id]->srwLock_.Writeunlock();

            //AddObject Packet ����
            if (IsMonster(iter) == true) {
                NETWORK::SendAddObject(players_[id]->socket_,
                    monsters_[iter - OBJECT_DEFINDS::MAX_USER]->x_, 
                    monsters_[iter - OBJECT_DEFINDS::MAX_USER]->y_, iter, OBJECT_DEFINDS::MONSTER);
            }
            else {
                NETWORK::SendAddObject(players_[id]->socket_,
                    players_[iter]->x_, players_[iter]->y_, iter, OBJECT_DEFINDS::OTHER_PLAYER);
            }

            //������ �ִ�(�þ߿� �ִ�) Monster ����
            WakeUpNearMonster(iter,id);

            //Object�� ����� ����ΰ�� �ؿ� ���� ����
            if (IsMonster(iter) == true)continue;

            players_[iter]->srwLock_.Readlock();
            if (players_[iter]->viewLIst_.find(id) != players_[iter]->viewLIst_.end()) {
                players_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER,
                    textureDirection);
            }
            //������ ����� viewList�� ���� ������ �� ���� ����
            else {
                players_[iter]->srwLock_.Readunlock();

                players_[iter]->srwLock_.Writelock();
                players_[iter]->viewLIst_.insert(id);
                players_[iter]->srwLock_.Writeunlock();

                //�̵��� �÷��̾��� ������ �þ߾ȿ� �ִ� �÷��̾�� ����
                NETWORK::SendAddObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER);
            }
        }
    } //End For NearList

     //oldView�� �ִ� ��� ��ü�� ����
    for (auto iter : oldView) {      //�þ߿��� �����

        if (nearList.find(iter) != nearList.end())continue;

        players_[id]->srwLock_.Writelock();
        players_[id]->viewLIst_.erase(iter);
        players_[id]->srwLock_.Writeunlock();

        //Remove Packet ����
        if (IsMonster(iter) == true) {
            // ���¸� Sleep ���·� ����
            monsters_[iter]->SetMonsterState(MonsterState::SLEEP);
            NETWORK::SendRemoveObject(players_[id]->socket_, iter, OBJECT_DEFINDS::MONSTER);
        }
        else {
            NETWORK::SendRemoveObject(players_[id]->socket_, iter, OBJECT_DEFINDS::OTHER_PLAYER);
        }

        //Object�� ����� ����ΰ�� �ؿ� ���� ����
        if (IsMonster(iter) == true)continue;

        players_[iter]->srwLock_.Readlock();
        if (players_[iter]->viewLIst_.find(id) != players_[iter]->viewLIst_.end()) {
            players_[iter]->srwLock_.Readunlock();

            players_[iter]->srwLock_.Writelock();
            players_[iter]->viewLIst_.erase(id);
            players_[iter]->srwLock_.Writeunlock();
            NETWORK::SendRemoveObject(players_[iter]->socket_, id, OBJECT_DEFINDS::OTHER_PLAYER);
        }
        else {
            players_[iter]->srwLock_.Readunlock();
        }
    }
}

bool CSector::WakeUpNearMonster(const ObjectIDType montserID,const ObjectIDType playerID) {
    //MonsterID�̸鼭 ����� ���Ͷ�� �����.
    if (IsMonster(montserID) && IsNearMonsterAndPlayer(montserID, playerID)) {
        ObjectIDType IndexingMonster = montserID - OBJECT_DEFINDS::MAX_USER;

        // ���¸� IDEL ���·� ����
        monsters_[IndexingMonster]->SetMonsterState(MonsterState::IDEL);
        return true;
    }
    return false;
}

void CSector::StartMovedMonster(const ObjectIDType montserID, const ObjectIDType playerID) {
    //���Ͱ� �ƴ϶�� return
    if (IsMonster(montserID) == false)return;

    //�̰� ��Ե� �ؾ߰��� �� ��ħ
    ObjectIDType IndexingMonster = montserID - OBJECT_DEFINDS::MAX_USER;

    //IDEL ���°� �ƴ϶�� PASS(��� �ൿ�� �ϰ� ������ �߰� X)
    if (monsters_[IndexingMonster]->GetMonsterState() != MonsterState::IDEL)return;

    // ���¸� MOVE���·� ����
    monsters_[IndexingMonster]->SetMonsterState(MonsterState::MOVE);

    //TimerQueue�� ���� �߰�
    CTimerQueueHandle::GetInstance()->queue_.Emplace(
        EVENT_ST{ IndexingMonster,playerID,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
}

void CSector::ProcessEvent(EVENT_ST& ev) {
    //������ �ൿ ����
    monsters_[ev.obj_id]->ExcuteMonster(*players_[ev.target_id]);
}

bool CSector::IsNearMonsterAndPlayer(const ObjectIDType montserID, const ObjectIDType playerID) {
    ObjectIDType IndexingMonster = montserID - OBJECT_DEFINDS::MAX_USER;

    PositionType mx = monsters_[IndexingMonster]->x_;
    PositionType my = monsters_[IndexingMonster]->y_;
    PositionType px = players_[playerID]->x_;
    PositionType py = players_[playerID]->y_;

    if (VIEW_SIZE < abs(mx - px) || VIEW_SIZE < abs(my - py))
        return false;
    return true;
}

bool CSector::IsNearPlayerAndPlayer(const ObjectIDType lhs, const ObjectIDType rhs) {
    PositionType mx = players_[lhs]->x_;
    PositionType my = players_[lhs]->y_;
    PositionType px = players_[rhs]->x_;
    PositionType py = players_[rhs]->y_;

    if (VIEW_SIZE < abs(mx - px) || VIEW_SIZE < abs(my - py))
        return false;
    return true;
}

bool CSector::SafeCheckUsedInArray(const ObjectClass type, const ObjectIDType id) {

    switch (type) {
    case OBJECT_DEFINDS::OTHER_PLAYER: {
        return players_[id]->isUsed_ ? true : false;
        break;
    }
    case OBJECT_DEFINDS::MONSTER: {
        return monsters_[id-OBJECT_DEFINDS::MAX_USER ]->isUsed_ ? true : false;
        break;
    }
    default:
        CLogCollector::GetInstance()->PrintLog("Not Defined Type In SafeCheckUsedArray Function");
        break;
    }
}

bool CSector::TestFunction(const ObjectIDType montserID, const ObjectIDType playerID) {

    //��ġ�ϸ� �׳� �����ϰ� ��
    if (monsters_[montserID]->followPlayerId_ == playerID) {
        return true;
    }
    //�� �����ŷ� ���� ID Change
    else {
        ObjectIDType followId = monsters_[montserID]->followPlayerId_;

        PositionType mx = monsters_[montserID]->x_;
        PositionType my = monsters_[montserID]->y_;
        PositionType px = players_[playerID]->x_;
        PositionType py = players_[playerID]->y_;
        PositionType fx = players_[followId]->x_;
        PositionType fy = players_[followId]->y_;

        PositionType newDistance = abs(mx - px) + abs(my - py);
        PositionType oldDistance = abs(mx - fx) + abs(my - fy);

        if (oldDistance < newDistance) {
            std::cout << "���ο� �÷��̾ �� �ָ� ���� FALSE\n";
            return false;
        }
        else {
            std::cout << "���ο� �÷��̾ �� ������ ���� TRUE\n";
            monsters_[montserID]->followPlayerId_ = playerID;
            return true;
        }
    }
}

bool CSector::IsMonster(const ObjectIDType id)const {
    return id >= OBJECT_DEFINDS::MAX_USER ? true : false;
}

