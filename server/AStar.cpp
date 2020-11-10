#include "AStar.h"
#include"Protocol.h"
#include"Navigation.h"

CAstar::CAstar() {
    //left
    direction_[0].first = -1;
    direction_[0].second = 0;
    //right
    direction_[1].first = +1;
    direction_[1].second = 0;
    //up
    direction_[2].first = 0;
    direction_[2].second = -1;
    //down
    direction_[3].first = 0;
    direction_[3].second = +1;
}


int CAstar::GetHeuristic(PairPositionType lhs, PairPositionType rhs) {
    //Manhattan Distance
    return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
}

bool CAstar::CheckVaildByNode(PairPositionType&& currentPosition, CNavigation& navigation) {
    int x = currentPosition.first;
    int y = currentPosition.second;

    //범위 안에 있으며
    if ((x >= 0 && x < MAP_DEFINDS::WORLD_WIDTH) && (y >= 0 && y < MAP_DEFINDS::WORLD_HEIGHT)) {
        //Close List에 없고
        bool isCLoseList = CheckByCloseList(std::move(currentPosition));
        //이동이 가능하다면
        auto type = navigation.GetCellType(x, y);
        if (!isCLoseList
            && type != CELL_TYPE::WALL
            && type != CELL_TYPE::PLAYER) {
            return true;
        }
    }
    return false;
}

bool CAstar::CheckByCloseList(PairPositionType&& currentPosition) {
    auto iter = std::find(closeList_.begin(),closeList_.end(), currentPosition);
    if (iter != closeList_.end())
        return true;
    return false;
}

void CAstar::ResetData() {
    //Best Way Use Memory Pool
    //Reset shortpath, closeList, openList
   shortPath_.clear();
   closeList_.clear();
   openList_ = Astar::OpenList();
}

Astar::ShortPath CAstar::StartFindPath(PairPositionType monster, PairPositionType player, CNavigation navigation) {

    ResetData();

    //시작 지점을 Open List에 넣는다.
    openList_.emplace(0, new Node{ monster,nullptr });
    navigation.SetCellType(player.first, player.second, CELL_TYPE::PLAYER);

    while (openList_.empty() == false) {
        //First: Weight Second: Position
        auto topPosition = openList_.top().second->position;
        auto topNode = openList_.top().second;
        auto topWeight = openList_.top().first;
        openList_.pop();


        //Add topVaule Position in Clost List
        closeList_.emplace_back(topPosition);

        int tx = topPosition.first;
        int ty = topPosition.second;

        //if (cheese.first == tx && cheese.second == ty) {
        if (navigation.GetCellType(tx, ty) == CELL_TYPE::PLAYER) {
            while (topNode != nullptr) {
                shortPath_.emplace_back(topNode->position);
                topNode = topNode->next;
            }
#ifdef _DEBUG
            std::cout << "Open  List Size" << pimpl_->openList_.size() << std::endl;
            std::cout << "Close List Size" << pimpl_->closeList_.size() << std::endl;
#endif // _DEBUG
            shortPath_.pop_back();
            return shortPath_;
        }

        //Left->Right->Up->Down
        for (int i = 0; i < 4; ++i) {
            int x = tx + direction_[i].first;
            int y = ty + direction_[i].second;
            //유효 여부 체크
            if (CheckVaildByNode(PairPositionType{ x,y }, navigation) == true) {
                //시작 지점부터 현재까지의 값 g(x) 
                int g = topWeight + ADD_WEIGHT;
                //현재 위치(상)에서 포지션까지의 값 h(x)
                int h = GetHeuristic(PairPositionType{ x,y }, PairPositionType{ player });
                //f(x)=g(x)+h(x)
                int f = g + h;
                //old weight
                int oldWeight = navigation.GetWeight(x, y);

                if (oldWeight > f) {
                    navigation.SetWeight(x, y, f);
                    openList_.emplace(Astar::PairData{ f,new Node{PairPositionType{x,y},topNode} });
                }
            }
        }
    } //End  While Close List
#ifdef _DEBUG
    std::cout << "Not Find Player\n";
#endif // _DEBUG
    return shortPath_;
}
