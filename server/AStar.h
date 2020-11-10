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
    using PairData      = std::pair<int, SharedPtrNode>;   //weight, Node
    using PairPosition  = std::pair<int, PairPositionType>;
    using ShortPath     = std::vector<PairPositionType>;
    using ShortPathIter = ShortPath::reverse_iterator;
    using OpenList      = std::priority_queue<PairData, std::vector<PairData>, Compare>;
    using CloseList     = std::vector<PairPositionType>;
}

struct Node {
public:
    PairPositionType    position;
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
    PairPositionType    direction_[4];
    Astar::ShortPath    shortPath_;
    Astar::OpenList     openList_;
    Astar::CloseList    closeList_;

    int                 GetHeuristic(PairPositionType lhs, PairPositionType rhs);
    bool                CheckVaildByNode(PairPositionType&& currentPosition, CNavigation& navigation);
    bool                CheckByCloseList(PairPositionType&& currentPosition);

public:
    CAstar();
    ~CAstar() = default;
    CAstar(const CAstar&) = delete;
    CAstar(CAstar&&) = delete;
    CAstar& operator=(const CAstar&) = delete;
    CAstar& operator=(CAstar&&) = delete;

    void                ResetData();
    Astar::ShortPath    StartFindPath(PairPositionType monster, PairPositionType player, CNavigation navigation);
};

