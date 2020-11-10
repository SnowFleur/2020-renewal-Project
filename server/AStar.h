#pragma once
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
    using PairPositionType  = std::pair<short, short>;
    using PairData          = std::pair<int, SharedPtrNode>;   //weight, Node
    using PairPosition      = std::pair<int, PairPositionType>;
    using ShortPath         = std::vector<PairPositionType>;
    using ShortPathIter     = ShortPath::reverse_iterator;
    using OpenList          = std::priority_queue<PairData, std::vector<PairData>, Compare>;
    using CloseList         = std::vector<PairPositionType>;
}

struct Node {
public:
    Astar::PairPositionType    position;
    SharedPtrNode       next;
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
    Astar::PairPositionType    direction_[4];
    Astar::OpenList            openList_;
    Astar::CloseList           closeList_;

    int                 GetHeuristic(Astar::PairPositionType lhs, Astar::PairPositionType rhs);
    bool                CheckVaildByNode(Astar::PairPositionType&& currentPosition, CNavigation& navigation);
    bool                CheckByCloseList(Astar::PairPositionType&& currentPosition);
public:
    Astar::ShortPath           shortPath_;

    CAstar();
    ~CAstar() = default;
    CAstar(const CAstar&) = delete;
    CAstar(CAstar&&) = delete;
    CAstar& operator=(const CAstar&) = delete;
    CAstar& operator=(CAstar&&) = delete;

    void                ResetData();
    Astar::ShortPath    StartFindPath(Astar::PairPositionType monster, Astar::PairPositionType player, CNavigation navigation);
};

