#pragma warning(disable: 4996) //JSON ���� ����
#include<algorithm>
#include<fstream>
#include"GameObject.h"
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
    //���� ����
    for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        gameobjects_[i] = new CPlayer(PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y, 100, 1, 0, 1, new CPlayerInputComponent);
    }

    //NPC ����
    for (ObjectIDType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        //gameobjects_[i] = new CNPC(PRIMARY_SPAWN_POSITION_X, PRIMARY_SPAWN_POSITION_Y, 100, 1, 0, 1, new CPlayerInputComponent);
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
    for (ObjectIDType i = OBJECT_DEFINDS::MAX_USER; i < OBJECT_DEFINDS::MAX_USER + OBJECT_DEFINDS::MAX_MONSER; ++i) {
        HpType hp{}; LevelType level{}; ExpType exp{}; AttackPowerType attackPower{};

        if (i < OBJECT_DEFINDS::MAX_USER + 100) {
            hp = root["Orc"]["INFOR"].get("HP", -1).asInt();
            level = root["Orc"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Orc"]["INFOR"].get("EXP", -1).asInt();
            attackPower = root["Orc"]["INFOR"].get("DAMAGE", -1).asInt();
            gameobjects_[i] = new CMonster(MonsterType::ORC, 0, 0, hp, level, exp, attackPower, new CMonsterInputComponent);
        }
        else if (i < OBJECT_DEFINDS::MAX_USER + 200) {
            gameobjects_[i] = new CMonster(MonsterType::ZOMBIE, 0, 0, hp, level, exp, attackPower, new CMonsterInputComponent);
        }
        else if (i < OBJECT_DEFINDS::MAX_USER + 300) {
            gameobjects_[i] = new CMonster(MonsterType::MUMMY, 0, 0, hp, level, exp, attackPower, new CMonsterInputComponent);

        }
        else if (i < OBJECT_DEFINDS::MAX_USER + 400) {
            hp = root["Bat"]["INFOR"].get("HP", -1).asInt();
            level = root["Bat"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Bat"]["INFOR"].get("EXP", -1).asInt();
            attackPower = root["Bat"]["INFOR"].get("DAMAGE", -1).asInt();
            gameobjects_[i] = new CMonster(MonsterType::BAT, 0, 0, hp, level, exp, attackPower, new CMonsterInputComponent);
        }
    }
    openjsonFile.close();
    CLogCollector::GetInstance()->PrintLog("Monster Init And Add In Sector");

    //�׽�Ʈ�� ���� �ϳ��� True Ŵ;
    gameobjects_[OBJECT_DEFINDS::MAX_USER + 300]->SetUsed(true);
    //21.02.02 ����� �ϳ��� ���͸� ����� ���̱� ������ �ϳ��� ��� ���߿��� ���� �÷��� �� ���
    AddObject(OBJECT_DEFINDS::MAX_USER + 300, PRIMARY_MONSTER_X, PRIMARY_MONSTER_Y);


    //�׽�Ʈ�� ���� �ϳ��� True Ŵ;
    gameobjects_[OBJECT_DEFINDS::MAX_USER + 301]->SetUsed(true);
    //21.02.02 ����� �ϳ��� ���͸� ����� ���̱� ������ �ϳ��� ��� ���߿��� ���� �÷��� �� ���
    AddObject(OBJECT_DEFINDS::MAX_USER + 301, PRIMARY_MONSTER_X + 1, PRIMARY_MONSTER_Y);


    ////�׽�Ʈ�� ���� �ϳ��� True Ŵ;
    //gameobjects_[OBJECT_DEFINDS::MAX_USER + 302]->SetUsed(true);
    ////21.02.02 ����� �ϳ��� ���͸� ����� ���̱� ������ �ϳ��� ��� ���߿��� ���� �÷��� �� ���
    //AddObject(OBJECT_DEFINDS::MAX_USER + 302, PRIMARY_MONSTER_X + 1, PRIMARY_MONSTER_Y+5);


    ////�׽�Ʈ�� ���� �ϳ��� True Ŵ;
    //gameobjects_[OBJECT_DEFINDS::MAX_USER + 303]->SetUsed(true);
    ////21.02.02 ����� �ϳ��� ���͸� ����� ���̱� ������ �ϳ��� ��� ���߿��� ���� �÷��� �� ���
    //AddObject(OBJECT_DEFINDS::MAX_USER + 303, PRIMARY_MONSTER_X, PRIMARY_MONSTER_Y+5);

}

void CSector::AddObject(const ObjectIDType id, const PositionType x, const PositionType y) {
    PositionType cx = x / MAP_DEFINDS::CELL_SIZE;
    PositionType cy = y / MAP_DEFINDS::CELL_SIZE;

    gameobjects_[id]->SetPosition(x, y);
    cells_[cx][cy].emplace(id);
}

/*
�̰͵� PlayerInputComponet�� �ű��
*/
void CSector::MoveObject(const ObjectIDType id, const PositionType newX, const PositionType newY,
    const TextureDirection textureDirection) {

    PositionType newCellX = newX / MAP_DEFINDS::CELL_SIZE;
    PositionType newCellY = newY / MAP_DEFINDS::CELL_SIZE;


    gameobjects_[id]->srwLock_.Readlock();
    std::unordered_set<ObjectIDType>oldView = gameobjects_[id]->GetViewList();
    gameobjects_[id]->srwLock_.Readunlock();


    //���� �̵��� ��ǥ�� �ش� ID�� ���ٸ� �߰�
    //20.11.17: �ٸ� �����尡 ���ͼ� Cells�� �ǵ� �� ������ ���߿� ó��
    if (cells_[newCellX][newCellY].count(id) == false) {
        cells_[newCellX][newCellY].emplace(id);

        PositionType oldX = gameobjects_[id]->GetPositionX();
        PositionType oldY = gameobjects_[id]->GetPositionY();

        //���� Sector���� ���� �����.
        PositionType oldCellX = oldX / MAP_DEFINDS::CELL_SIZE;
        PositionType oldCellY = oldY / MAP_DEFINDS::CELL_SIZE;
        cells_[oldCellX][oldCellY].erase(id);
    }

    gameobjects_[id]->SetPosition(newX, newY);

    //�ش� Sector�� �ִ� Object ����
    std::unordered_set <ObjectIDType> nearList;

    //�ش� Sector �ֺ��� �ִ� 8������ Sector ������ NearList�� ����
    for (PositionType i = 0; i < NUMBER_OF_SEARCH; ++i) {
        short x = newCellX + searchDirection_[i].first;
        short y = newCellY + searchDirection_[i].second;
        if (x < 0 || y < 0)continue;

        for (auto i : cells_[x][y]) {
            nearList.emplace(i);
        }
    }
    //�� ��������
    nearList.erase(id);

    for (auto iter : nearList) {

        //�̵��� ���� oldView�� NearList�� ������ �ִٸ�
        if (oldView.find(iter) != oldView.end()) {

            //���� �ִ� �������� Ȯ�� �� �̵� ����
            WakeUpNearMonster(iter, id);

            //Object�� ����� ����ΰ�� �ؿ� ���� ����
            if (IsMonster(iter) == true)continue;

            //��� viewList�� ���� ������
            gameobjects_[iter]->srwLock_.Readlock();
            if (gameobjects_[iter]->GetViewList().find(id) != gameobjects_[iter]->GetViewList().end()) {
                //�̵��� �÷��̾��� ������ �þ߾ȿ� �ִ� �÷��̾�� ����
                gameobjects_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(gameobjects_[iter]->GetSocket(),
                    gameobjects_[id]->GetPositionX(), gameobjects_[id]->GetPositionY(), id, textureDirection);
            }
            //��� viewList�� ���� ������
            else {
                //read UnLock
                gameobjects_[iter]->srwLock_.Readunlock();

                //View List�� ���
                gameobjects_[iter]->srwLock_.Writelock();
                gameobjects_[iter]->GetViewList().insert(id);
                gameobjects_[iter]->srwLock_.Writeunlock();

                //�̵��� �÷��̾��� ������ �þ߾ȿ� �ִ� �÷��̾�� ����
                NETWORK::SendAddObject(gameobjects_[iter]->GetSocket(),
                    gameobjects_[id]->GetPositionX(), gameobjects_[id]->GetPositionY(), id);
            }
        }
        //�̵��� ���� oldview�� NearList�� ������ ���ٸ� 
        else {
            //���� ViewLIst�� ������� ���
            gameobjects_[id]->srwLock_.Writelock();
            gameobjects_[id]->GetViewList().insert(iter);
            gameobjects_[id]->srwLock_.Writeunlock();

            //AddObject Packet ����
            if (IsMonster(iter) == true) {
                NETWORK::SendAddObject(gameobjects_[id]->GetSocket(),
                    gameobjects_[iter]->GetPositionX(), gameobjects_[iter]->GetPositionY(), iter);
            }
            else {
                NETWORK::SendAddObject(gameobjects_[id]->GetSocket(),
                    gameobjects_[iter]->GetPositionX(), gameobjects_[iter]->GetPositionY(), iter);
            }

            //������ �ִ�(�þ߿� �ִ�) Monster ����
            WakeUpNearMonster(iter, id);

            //Object�� ����� ����ΰ�� �ؿ� ���� ����
            if (IsMonster(iter) == true)continue;

            gameobjects_[iter]->srwLock_.Readlock();
            if (gameobjects_[iter]->GetViewList().find(id) != gameobjects_[iter]->GetViewList().end()) {
                gameobjects_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(gameobjects_[iter]->GetSocket(),
                    gameobjects_[id]->GetPositionX(), gameobjects_[id]->GetPositionY(), id, textureDirection);
            }
            //������ ����� viewList�� ���� ������ �� ���� ����
            else {
                gameobjects_[iter]->srwLock_.Readunlock();

                gameobjects_[iter]->srwLock_.Writelock();
                gameobjects_[iter]->GetViewList().insert(id);
                gameobjects_[iter]->srwLock_.Writeunlock();

                //�̵��� �÷��̾��� ������ �þ߾ȿ� �ִ� �÷��̾�� ����
                NETWORK::SendAddObject(gameobjects_[iter]->GetSocket(),
                    gameobjects_[id]->GetPositionX(), gameobjects_[id]->GetPositionY(), id);
            }
        }
    } //End For NearList

     //oldView�� �ִ� ��� ��ü�� ����
    for (auto iter : oldView) {

        //NearList�� ���ٸ� �þ߿��� ����� object
        if (nearList.find(iter) != nearList.end())continue;

        gameobjects_[id]->srwLock_.Writelock();
        gameobjects_[id]->GetViewList().erase(iter);
        gameobjects_[id]->srwLock_.Writeunlock();

        // ���¸� SLEEP ���·� ����
        gameobjects_[iter]->SetObjectState(ObjectState::SLEEP);
        //Remove Packet ����
        NETWORK::SendRemoveObject(gameobjects_[id]->GetSocket(), iter);

        //Object�� ����� ����ΰ�� �ؿ� ���� ����
        if (IsMonster(iter) == true)continue;

        gameobjects_[iter]->srwLock_.Readlock();
        if (gameobjects_[iter]->GetViewList().find(id) != gameobjects_[iter]->GetViewList().end()) {
            gameobjects_[iter]->srwLock_.Readunlock();

            gameobjects_[iter]->srwLock_.Writelock();
            gameobjects_[iter]->GetViewList().erase(id);
            gameobjects_[iter]->srwLock_.Writeunlock();
            NETWORK::SendRemoveObject(gameobjects_[iter]->GetSocket(), id);
        }
        else {
            gameobjects_[iter]->srwLock_.Readunlock();
        }
    }
}

bool CSector::WakeUpNearMonster(const ObjectIDType montserID, const ObjectIDType playerID) {

    //SLEEP ���°� �ƴ϶�� PASS(��� �ൿ�� �ϰ� �ִ� ��)
    if (gameobjects_[montserID]->GetObjectState() != ObjectState::SLEEP) return false;

    //�����鼭 ���Ͷ�� �����.
    if (IsMonster(montserID) && IsNearObject(montserID, playerID)) {

        // ���¸� IDEL ���·� ����
        gameobjects_[montserID]->SetObjectState(ObjectState::IDEL);

        //TimerQueue�� ���� �߰�
        CTimerQueueHandle::GetInstance()->queue_.Emplace(
            EVENT_ST{ montserID,playerID,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });

        return true;
    }
    return false;
}

//void CSector::StartMovedMonster(const ObjectIDType montserID, const ObjectIDType playerID) {
//    //���Ͱ� �ƴ϶�� return
//    if (IsMonster(montserID) == false)return;
//
//    //IDEL ���°� �ƴ϶�� PASS(��� �ൿ�� �ϰ� ������ �߰� X)
//    if (gameobjects_[montserID]->GetObjectState() != ObjectState::IDEL)return;
//
//    // ���¸� MOVE���·� ����
//    gameobjects_[montserID]->SetObjectState(ObjectState::MOVE);
//
//    //TimerQueue�� ���� �߰�
//    CTimerQueueHandle::GetInstance()->queue_.Emplace(
//        EVENT_ST{ montserID,playerID,EVENT_TYPE::EV_EXCUTE_MONSTER,high_resolution_clock::now() + 1s });
//}

void CSector::ProcessEvent(EVENT_ST& ev) {
    gameobjects_[ev.obj_id]->ProcessInputComponent(*this, ev);
}

bool CSector::IsNearObject(const ObjectIDType lhs, const ObjectIDType rhs) {

    PositionType lhsX{}, lhsY{}, rhsX{}, rhsY{};

    gameobjects_[lhs]->GetPosition(lhsX, lhsY);
    gameobjects_[rhs]->GetPosition(rhsX, rhsY);

    if (VIEW_SIZE < abs(lhsX - rhsX) || VIEW_SIZE < abs(lhsY - rhsY))
        return false;
    return true;
}

bool CSector::IsMonster(const ObjectIDType id)const {
    return id >= OBJECT_DEFINDS::MAX_USER ? true : false;
}

void  CSector::GetViewListByCell(std::unordered_set<ObjectIDType>& view, PositionType x, PositionType y) {
    PositionType CellX = x / MAP_DEFINDS::CELL_SIZE;
    PositionType CellY = y / MAP_DEFINDS::CELL_SIZE;
    for (PositionType i = 0; i < NUMBER_OF_SEARCH; ++i) {
        short x = CellX + searchDirection_[i].first;
        short y = CellY + searchDirection_[i].second;
        if (x < 0 || y < 0)continue;

        for (auto i : cells_[x][y]) {
            if (i < OBJECT_DEFINDS::MAX_USER) {
                view.emplace(i);
            }
        }
    }
}
