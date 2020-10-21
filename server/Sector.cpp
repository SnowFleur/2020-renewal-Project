#include"Sector.h"
#include"Player.h"

CSector::CSector() {
    //일단 여기서 동적할당을 받고 나중에 풀에서 받자
    for (uIntType i = 0; i < OBJECT_DEFINDS::MAX_USER; ++i) {
        players_[i] = new CPlayer();
    }
}

CSector::~CSector() {}

void CSector::GetCellObject(PositionType x, PositionType y) {
}

void CSector::SetCellObject(PositionType x, PositionType y) {

}


    /*
      // 초기화 해줘야하는 인자들
        clients[new_id].socket = clientSocket;
        clients[new_id].prev_size = 0;
        clients[new_id].in_used = false;
        clients[new_id].m_lock.lock();
        clients[new_id].viewlist.clear();
        clients[new_id].m_lock.unlock();
    */

