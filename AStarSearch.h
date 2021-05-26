// ProjName: NPuzzle_assignment
// FileName: AStarSearch.h

/*
*  Author:  Peace
*  Date:    2021/5/14
*  Email:   caiml20@nudt.edu.cn
*  Desc:    declare class aStarSearch
*/

#ifndef NPUZZLE_ASSIGNMENT_ASTARSEARCH_H
#define NPUZZLE_ASSIGNMENT_ASTARSEARCH_H

#include "Node.h"
#include <set>

#define MISPLACED_TILES 1
#define MANHATTAN_DISTANCE 2
#define LINEAR_CONFLICT 3
#define LIMIT_DEPTH 60
#define NODE_LIMIT 10000000
#define cost_ cost
#define parent_ parent


typedef int cost_t;
typedef int parent_t;

struct NodeInfo
{
    bool isClosed;
    cost_t cost;
    parent_t parent;

    bool operator==(const NodeInfo &rhs) const
    {
        return parent == rhs.parent &&
               cost == rhs.cost;
    }

    bool operator!=(const NodeInfo &rhs) const
    {
        return !(rhs == *this);
    }
};

class aStarSearch
{
public:
    std::map<Node, NodeInfo> visited;
    size_t openedCount;
    int max_depth;
    int nPushed;
    int heuristicType = 0;

public:
    virtual ~aStarSearch();

    bool isValid(int x, int y);

    static double MisplacedTiles(const Node &a, const Node &b);

    static double ManhattanDistance(const Node &a, const Node &b);

    static double LineConflicts(const Node &a, const Node &b);

    static double LinearConflicts(const Node &a, const Node &b);

    double Heuristic(const Node &a, const Node &b);

    void setHeuristic(int heuristic = MANHATTAN_DISTANCE);

    int AStarSearch(const Node &Start, const Node &Goal);
};

#endif //NPUZZLE_ASSIGNMENT_ASTARSEARCH_H
