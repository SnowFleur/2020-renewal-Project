#pragma once
#include<tuple>
#include<vector>
#include<queue>
#include<memory>
#include"DataType.h"

class CNavigation;
class Compare;
class Node;
constexpr int ADD_WEIGHT = 1;
using SharedPtrNode = std::shared_ptr<Node>;

namespace Astar {
    using PairPosition      = std::pair<short,short>;              //x, y
    using TuplePosition     = std::tuple<short,short,char>;       //x,y,texture Direciton
    using PairData          = std::pair<int, SharedPtrNode>;        //weight, Node
    //using PairPosition      = std::pair<int, PairPositionType>;
    using ShortPath         = std::vector<TuplePosition>;
    //using ShortPathIter     = ShortPath::reverse_iterator;
    using OpenList          = std::priority_queue<PairData, std::vector<PairData>, Compare>;
    using CloseList         = std::vector<PairPosition>;
}

struct Node {
public:
    Astar::TuplePosition        position;
    SharedPtrNode               next;
};

struct Compare {
public:
    Compare() = delete;
    bool operator()(const Astar::PairData& lhs, const Astar::PairData& rhs) {
        return lhs.first > rhs.first;
    }
};

class CAstar{
private:
    Astar::PairPosition        direction_[4];
    Astar::OpenList            openList_;
    Astar::CloseList           closeList_;

    int                 GetHeuristic(Astar::PairPosition lhs, Astar::PairPosition rhs);
    bool                CheckVaildByNode(Astar::PairPosition&& currentPosition, CNavigation& navigation);
    bool                CheckByCloseList(Astar::PairPosition&& currentPosition);
    char                GetNextCharacterTexture(Astar::PairPosition&& oldPosition, Astar::PairPosition&& newPosition);
public:
    Astar::ShortPath           shortPath_;

    CAstar();
    ~CAstar() = default;
    CAstar(const CAstar&) = delete;
    CAstar(CAstar&&) = delete;
    CAstar& operator=(const CAstar&) = delete;
    CAstar& operator=(CAstar&&) = delete;

    void               ResetData();
    bool               StartFindPath(Astar::PairPosition monsterPosition, Astar::PairPosition playerPosition, CNavigation navigation);
};

