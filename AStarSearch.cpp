// ProjName: NPuzzle_assignment
// FileName: AStarSearch.cpp

/*
*  Author:  Peace
*  Date:    2021/5/14
*  Email:   caiml20@nudt.edu.cn
*  Desc:    define class aStarSearch
*/

#include "AStarSearch.h"

using namespace std;

aStarSearch::~aStarSearch()
{
    heuristicType = 0;
    visited.clear();
}

bool aStarSearch::isValid(int x, int y)
{
    return x >= 0 && y >= 0 && x < Node::boardSqSize && y < Node::boardSqSize;
}


double aStarSearch::MisplacedTiles(const Node &a, const Node &b)
{
    int conflicts = 0;
    for (int i = 0; i < Node::boardSqSize; i++)
        for (int j = 0; j < Node::boardSqSize; j++)
            if (a.A[i][j] && a.A[i][j] != b.A[i][j])
                conflicts++;
    return conflicts;
}


double aStarSearch::ManhattanDistance(const Node &a, const Node &b)
{
    int sum = 0;
    puzzle_t pR[(Node::boardSqSize * Node::boardSqSize) + 1];
    puzzle_t pC[(Node::boardSqSize * Node::boardSqSize) + 1];
    for (int r = 0; r < Node::boardSqSize; r++)
    {
        for (int c = 0; c < Node::boardSqSize; c++)
        {
            pR[a.A[r][c]] = static_cast<puzzle_t>(r);
            pC[a.A[r][c]] = static_cast<puzzle_t>(c);
        }
    }
    for (int r = 0; r < Node::boardSqSize; r++)
        for (int c = 0; c < Node::boardSqSize; c++)
            if (b.A[r][c])
                sum += abs(pR[b.A[r][c]] - r) + abs(pC[b.A[r][c]] - c);
    return sum;
}


double aStarSearch::LineConflicts(const Node &a, const Node &b)
{
    int conflicts = 0;
    puzzle_t pR[(Node::boardSqSize * Node::boardSqSize) + 1];
    puzzle_t pC[(Node::boardSqSize * Node::boardSqSize) + 1];
    for (int r = 0; r < Node::boardSqSize; r++)
    {
        for (int c = 0; c < Node::boardSqSize; c++)
        {
            pR[a.A[r][c]] = static_cast<puzzle_t>(r);
            pC[a.A[r][c]] = static_cast<puzzle_t>(c);
        }
    }

    // row conflicts
    for (int r = 0; r < Node::boardSqSize; r++)
        for (int cl = 0; cl < Node::boardSqSize; cl++)
            for (int cr = cl + 1; cr < Node::boardSqSize; cr++)
                if (b.A[r][cl] && b.A[r][cr] && r == pR[b.A[r][cl]] && pR[b.A[r][cl]] == pR[b.A[r][cr]] &&
                    pC[b.A[r][cl]] > pC[b.A[r][cr]])
                    conflicts++;

    // column conflicts
    for (int c = 0; c < Node::boardSqSize; c++)
        for (int rU = 0; rU < Node::boardSqSize; rU++)
            for (int rD = rU + 1; rD < Node::boardSqSize; rD++)
                if (b.A[rU][c] && b.A[rD][c] && c == pC[b.A[rU][c]] && pC[b.A[rU][c]] == pC[b.A[rD][c]] &&
                    pR[b.A[rU][c]] > pR[b.A[rD][c]])
                    conflicts++;

    return 2 * conflicts;
}


double aStarSearch::LinearConflicts(const Node &a, const Node &b)
{
    return ManhattanDistance(a, b) + LineConflicts(a, b);
}


double aStarSearch::Heuristic(const Node &a, const Node &b)
{
    if (heuristicType == MISPLACED_TILES) return MisplacedTiles(a, b);
    if (heuristicType == MANHATTAN_DISTANCE) return ManhattanDistance(a, b);
    if (heuristicType == LINEAR_CONFLICT) return LinearConflicts(a, b);
    return 0;
}


void aStarSearch::setHeuristic(int heuristic)
{
    heuristicType = heuristic;
}


int aStarSearch::AStarSearch(const Node &Start, const Node &Goal)
{
    int nExpanded = 0;
    max_depth = 0;
    nPushed = 0;

    priority_queue<pair<double, Node> > openList;
    openList.push({0, Start});
    visited[Start] = {false, 0, EOF};

    while (!openList.empty())
    {
        Node u = openList.top().second;
        openList.pop();
        ++nExpanded;
        NodeInfo &uInfo = visited[u];
        uInfo.isClosed = true;

        max_depth = max(max_depth, visited[u].cost);

        if (u == Goal)
        {
            break;
        }

        if (uInfo.cost > LIMIT_DEPTH)
        {
            cout << "Height limit Exceeded @" << endl << u;
            break;
        }


        if (visited.size() > NODE_LIMIT)
        {
            cout << "Node limit Exceeded @" << endl << u;
            break;
        }

        int zX = -1, zY = -1;
        Node::getZeroPos(u, zX, zY);

        for (direction_t dir = 0; dir < 4; dir++)
        {
            int zXnew = zX + dirX[dir];
            int zYnew = zY + dirY[dir];
            if (isValid(zXnew, zYnew))
            {
                Node v = u;
                swap(v.A[zX][zY], v.A[zXnew][zYnew]);

                bool isVisited = visited.find(v) != visited.end();
                if (isVisited && visited[v].isClosed)continue;

                double newCost = uInfo.cost + 1;
                if (!isVisited || newCost < visited[v].cost)
                { // 2nd condition might not be needed
                    ++nPushed;
                    visited[v] = {false, static_cast<cost_t>(newCost), Node::oppositeDirection(dir)};
                    double Priority = newCost + Heuristic(v, Goal);
                    openList.push({-Priority, v});
                }
            }
        }

    }
    openedCount = visited.size();
    return nExpanded;
}