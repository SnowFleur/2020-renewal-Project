#include"Sector.h"
#include"Player.h"

CSector::CSector() {
    //�ϴ� ���⼭ �����Ҵ��� �ް� ���߿� Ǯ���� ����
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
      // �ʱ�ȭ ������ϴ� ���ڵ�
        clients[new_id].socket = clientSocket;
        clients[new_id].prev_size = 0;
        clients[new_id].in_used = false;
        clients[new_id].m_lock.lock();
        clients[new_id].viewlist.clear();
        clients[new_id].m_lock.unlock();
    */

