#include "AStar.h"
#include"Protocol.h"
#include"Navigation.h"
#include"LogCollector.h"

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


int CAstar::GetHeuristic(Astar::PairPosition lhs, Astar::PairPosition rhs) {
    //Manhattan Distance
    return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
}

bool CAstar::CheckVaildByNode(Astar::PairPosition&& currentPosition, CNavigation& navigation) {
    int x = currentPosition.first;
    int y = currentPosition.second;

    //범위 안에 있으며
    if ((x >= 0 && x < MAP_DEFINDS::SECTOR_X) && (y >= 0 && y < MAP_DEFINDS::SECTOR_Y)) {
        //Close List에 없고
        bool isCLoseList = CheckByCloseList(std::move(currentPosition));
        //이동이 가능하다면
        auto type = navigation.GetTileType(x, y);
        if (!isCLoseList
            && type != TILE_TYPE::WALL
            /*&& type != CELL_TYPE::PLAYER*/) {
            return true;
        }
    }
    return false;
}

bool CAstar::CheckByCloseList(Astar::PairPosition&& currentPosition) {
    auto iter = std::find(closeList_.begin(),closeList_.end(), currentPosition);
    if (iter != closeList_.end())
        return true;
    return false;
}

char CAstar::GetNextCharacterTexture(Astar::PairPosition&& oldPosition, Astar::PairPosition&& newPosition) {
    if (oldPosition.first < newPosition.first)return CHARACTER_RIGHT;
    if (oldPosition.first > newPosition.first)return CHARACTER_LEFT;
    if (oldPosition.second < newPosition.second)return CHARACTER_DOWN;
    if (oldPosition.second > newPosition.second)return CHARACTER_UP;
}

void CAstar::ResetData() {
    //Best Way Use Memory Pool
    //Reset shortpath, closeList, openList
   shortPath_.clear();
   closeList_.clear();
   openList_ = Astar::OpenList();
}


bool CAstar::StartFindPath(Astar::PairPosition monsterPosition, Astar::PairPosition playerPosition, CNavigation navigation) {


    ResetData();

    //Node Data에 맞게 변환
    Astar::TuplePosition tuplePosition{ monsterPosition.first,monsterPosition.second,0 };

    //시작 지점을 Open List에 넣는다.
    openList_.emplace(0, new Node{ tuplePosition,nullptr });
    navigation.SetTileType(playerPosition.first, playerPosition.second, TILE_TYPE::PLAYER);

    while (openList_.empty() == false) {
        //First: Weight Second: Position
        auto topPosition = openList_.top().second->position;
        auto topNode = openList_.top().second;
        auto topWeight = openList_.top().first;
        openList_.pop();


        //Add topVaule Position in Clost List 
        closeList_.emplace_back(
            Astar::PairPosition(std::get<0>(topPosition), std::get<1>(topPosition))
        );

        int tx = std::get<0>(topPosition);
        int ty = std::get<1>(topPosition);

        //if (cheese.first == tx && cheese.second == ty) {
        if (navigation.GetTileType(tx, ty) == TILE_TYPE::PLAYER) {
            while (topNode != nullptr) {
                shortPath_.emplace_back(topNode->position);
                topNode = topNode->next;
            }
            shortPath_.pop_back();
            return true;
        }

        //Left->Right->Up->Down
        for (int i = 0; i < 4; ++i) {
            int x = tx + direction_[i].first;
            int y = ty + direction_[i].second;
            
            //유효 여부 체크
            if (CheckVaildByNode(Astar::PairPosition{ x,y }, navigation) == true) {
                //시작 지점부터 현재까지의 값 g(x) 
                int g = topWeight + ADD_WEIGHT;
                //현재 위치(상)에서 포지션까지의 값 h(x)
                int h = GetHeuristic(Astar::PairPosition{ x,y }, Astar::PairPosition{ playerPosition });
                //f(x)=g(x)+h(x)
                int f = g + h;
                //old weight
                int oldWeight = navigation.GetWeight(x, y);

                if (oldWeight > f) {
                    navigation.SetWeight(x, y, f);
                    char dir = GetNextCharacterTexture(Astar::PairPosition(tx, ty), Astar::PairPosition(x, y));
                    openList_.emplace(Astar::PairData{ f,new Node{Astar::TuplePosition{x,y,dir},topNode} });
                }
            }
        }
    } //End  While Close List

    //Not Found Target
    return false;
}
