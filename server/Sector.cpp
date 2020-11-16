#pragma warning(disable: 4996) //JSON ���� ����
#include<algorithm>
#include<fstream>
#include"Sector.h"
#include"Player.h"
#include"Network.h"
#include"Monster.h"
#include"TimerThread.h"
#include"..\JSON\json\json.h"


CSector::CSector() {
    //�ϴ� ���⼭ �����Ҵ��� �ް� ���߿� Ǯ���� ����
    for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        players_[i] = new CPlayer();
    }

    std::ifstream openjsonFile("../JSON/Data/MonsterData.json");
    Json::Reader reader;
    Json::Value root;

    if (reader.parse(openjsonFile, root) == false)
        CLogCollector::GetInstance()->PrintLog("Json File Open is Fail");

    //ORC 0~100, ZOMBLE 101~200, MUMMY 201~300, BAT 301~400
    for (int i = 0; i < OBJECT_DEFINDS::MAX_MONSER; ++i) {
        HpType hp{}; LevelType level{}; ExpType exp{}; DamageType damage{};

        switch (i / 100) {
        case 0: {
            hp = root["Orc"]["INFOR"].get("HP", -1).asInt();
            level = root["Orc"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Orc"]["INFOR"].get("EXP", -1).asInt();
            damage = root["Orc"]["INFOR"].get("DAMAGE", -1).asInt();
            monsters_[i] = new CMonster(MonsterType::ORC, 0, 0, hp, level, exp, damage);

            break;
        }
        case 1: {
            //monsters_[i] = new CMonster(MonsterType::ZOMBIE);
            break;
        }
        case 2: {
            //monsters_[i] = new CMonster(MonsterType::MUMMY);
            break;
        }
        case 3: {
            hp = root["Bat"]["INFOR"].get("HP", -1).asInt();
            level = root["Bat"]["INFOR"].get("LEVEL", -1).asInt();
            exp = root["Bat"]["INFOR"].get("EXP", -1).asInt();
            damage = root["Bat"]["INFOR"].get("DAMAGE", -1).asInt();
            monsters_[i] = new CMonster(MonsterType::BAT, 0, 0, hp, level, exp, damage);
            break;
        }
        default:
            break;
        }
    }

    openjsonFile.close();

}

CSector::~CSector() {}

void CSector::GetCellObject(const PositionType x, const PositionType y) {}

/*
2020.11.16: ���� Object�� ID ������ Monster���� Player���� ����� ��Ȳ�̱� ������
Monster�� ���� �� �ִ� �÷��̾� ������ 2500�� �̻���� +�ؼ� �־�� �Ұ����� ����
*/
void CSector::SetCellObject(const PositionType x, const PositionType y) {}

void CSector::AddObject(const ObjectIDType id, const PositionType x, const PositionType y) {
    PositionType cx = x / MAP_DEFINDS::CELL_SIZE;
    PositionType cy = y / MAP_DEFINDS::CELL_SIZE;
    cells_[cx][cy].emplace(id);

    players_[id]->x_ = x;
    players_[id]->y_ = y;
}

void CSector::MoveObject(const ObjectIDType id, const PositionType newX, const PositionType newY,
    const TextureDirection textureDirection) {


    PositionType newCellX = newX / MAP_DEFINDS::CELL_SIZE;
    PositionType newCellY = newY / MAP_DEFINDS::CELL_SIZE;

    players_[id]->srwLock_.Writelock();
    std::unordered_set<ObjectIDType>oldView = players_[id]->viewLIst_;
    players_[id]->srwLock_.Writeunlock();


    //�ش� Sector�� �ִ� Object ����
    std::unordered_set <ObjectIDType> nearList = cells_[newCellX][newCellY];


    /*
    Near�� ��� ��ü�� ����

    1. viewList�� ������
    2. ViewList�� ������



    */

    for (auto iter : nearList) {

        //�̵��� ���� oldView�� NearList�� ������ �ִٸ�
        if (oldView.find(iter) != oldView.end()) {

            //������ �ִ� Monster ����
            WakeUpNearMonster(iter, id);

            //Object�� ����� ��� ����
            if (IsMonster(iter) == false)continue;

            players_[iter]->srwLock_.Readlock();
            //��� viewList�� ���� ������
            if (players_[iter]->viewLIst_.count(id)) {
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

            //��������� ����
            NETWORK::SendAddObject(players_[id]->socket_,
                players_[iter]->x_, players_[iter]->y_, iter, OBJECT_DEFINDS::OTHER_PLAYER);

            //������ �ִ� Monster ����
            WakeUpNearMonster(id, iter);

            //Object�� ����� ��� ����
            if (IsMonster(iter) == false)continue;

            players_[iter]->srwLock_.Readlock();
            if (players_[iter]->viewLIst_.count(id)) {
                players_[iter]->srwLock_.Readunlock();

                NETWORK::SendMoveObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER,
                    textureDirection);
            }

            //������ ����� viewList�� ���� ������
            //�� ���� ����
            else {
                players_[iter]->srwLock_.Writelock();
                players_[iter]->viewLIst_.insert(id);
                players_[iter]->srwLock_.Writeunlock();

                //�̵��� �÷��̾��� ������ �þ߾ȿ� �ִ� �÷��̾�� ����
                NETWORK::SendAddObject(players_[iter]->socket_,
                    players_[id]->x_, players_[id]->y_, id, OBJECT_DEFINDS::OTHER_PLAYER);
            }

        }

    } //End For NearList

     //oldView�� �ִ� ��� ��ä�� ����
    for (auto iter : oldView) {      //�þ߿��� �����

        if (0 != nearList.count(iter))
            continue;

        players_[id]->srwLock_.Writelock();
        players_[id]->viewLIst_.erase(iter);
        players_[id]->srwLock_.Writeunlock();

        NETWORK::SendRemoveObject(players_[id]->socket_, iter, OBJECT_DEFINDS::MONSTER);

        //Object�� ����� ��� ����
        if (IsMonster(iter) == false)continue;

        players_[iter]->srwLock_.Readlock();
        if (0 != players_[iter]->viewLIst_.count(id)) {
            players_[iter]->srwLock_.Readunlock();

            players_[iter]->srwLock_.Writelock();
            players_[iter]->viewLIst_.erase(id);
            players_[id]->srwLock_.Writeunlock();

            NETWORK::SendRemoveObject(players_[iter]->socket_, ), OBJECT_DEFINDS::MONSTER);
        }
        else {
            players_[iter]->srwLock_.Readunlock();
        }
    }


    //���� �̵��� ��ǥ�� �ش� ID�� ���ٸ� �߰�
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

    //�̵��� Ŭ���̾�Ʈ�� ���� ���Ϳ� �ִ� Ŭ��鿡�� Move ����
    //����� �̵� üũ�� �� �����̱� ������ �׳� ��ü ���� (20.11.01)
    for (int i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        //���� ���� �� ������� Ŭ���̾�Ʈ��
        if (i == id || players_[i]->isUsed_ == false)continue;
        //socket, �̵�x, �̵�y, �̵���id, Ŭ����(�÷��̾�),�̵���id�� �ؽ�������
        NETWORK::SendMoveObject(players_[i]->socket_, newX, newY, id,
            OBJECT_DEFINDS::OTHER_PLAYER, textureDirection);
    }
}

bool CSector::WakeUpNearMonster(const ObjectIDType montserID, const ObjectIDType playerID) {

    //MonsterID���� Ȯ��
    if (IsMonster(montserID) == false)return false;

    //�÷��̾�� ������ �ִ� �������� Check
    if (IsNearPlayer(montserID, playerID)) {
        // �̹� �����ִ� Monster�� pass
        monsters_[montserID]->isUsed_ = true;
        return true;
    }
    return false;
}

void CSector::ProcessEvent(EVENT_ST& ev) {

    switch (ev.type) {
    case EV_MONSTER_MOVE: {
        //�ڱ⸦ ���� �÷��̾� ������ �ִ´�.
        monsters_[ev.obj_id]->MoveMonster(*players_[ev.target_id]);

        //if(cells_.)

        break;
    }
    default:
        break;
    }

}

bool CSector::IsNearPlayer(const ObjectIDType montserID, const ObjectIDType playerID) {
    PositionType mx = monsters_[montserID]->x_;
    PositionType my = monsters_[montserID]->y_;
    PositionType px = players_[playerID]->x_;
    PositionType py = players_[playerID]->y_;

    if (VIEW_SIZE < abs(mx - px) || VIEW_SIZE < abs(my - py))
        return false;

    return true;
}

bool CSector::TestFunction(const ObjectIDType montserID, const ObjectIDType playerID) {


    PositionType mx = monsters_[montserID]->x_;
    PositionType my = monsters_[montserID]->y_;
    PositionType px = players_[playerID]->x_;
    PositionType py = players_[playerID]->y_;

    PositionType distance = abs(mx - px) + abs(my - py);

    /*
    �־����� ShortDistance���� �־����� ������ A ��Ÿ�� �����ϴ� ������ �߻�

    2���̻��� �÷��̾ �þ߿� ���� �� ��� �÷��̾ ���� A��Ÿ�� ��������
    �Ǵ� ���������

    */

    /*
    �Ÿ� �� -> �� ª���� ID ��ü �ƴϸ� FALSE
    */

    if (monsters_[montserID]->shortDistanceByPlayer_ > distance) {
        monsters_[montserID]->shortDistanceByPlayer_ = distance;

        std::cout << "Short Player ID: " << (int)playerID << "\n";
        return true;
    }

    return false;
}

bool CSector::IsMonster(const ObjectIDType id) {
    return id >= OBJECT_DEFINDS::MAX_USER ? true : false;
}