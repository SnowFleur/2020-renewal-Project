#include"Sector.h"
#include"Player.h"

CSector::CSector() {
    //�ϴ� ���⼭ �����Ҵ��� �ް� ���߿� Ǯ���� ����
    for (uIntType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        players_[i] = new CPlayer();
    }
}

CSector::~CSector() {}

void CSector::GetCellObject(const PositionType x, const PositionType y) {


}

void CSector::SetCellObject(const PositionType x, const PositionType y) {

}

void CSector::AddObject(const ObjectIDType id ,const PositionType x, const PositionType y) {

    //10ĭ�� ������
    PositionType cx = x / 10;
    PositionType cy = y / 10;

    cells_[cx][cy].emplace(id);

}

void CSector::MoveObject(const ObjectIDType id, const PositionType x, const PositionType y) {



}

