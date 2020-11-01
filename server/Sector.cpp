#include"Sector.h"
#include"Player.h"

CSector::CSector() {
    //일단 여기서 동적할당을 받고 나중에 풀에서 받자
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

    //10칸씩 나누기
    PositionType cx = x / 10;
    PositionType cy = y / 10;

    cells_[cx][cy].emplace(id);

}

void CSector::MoveObject(const ObjectIDType id, const PositionType x, const PositionType y) {



}

