// ProjName: NPuzzle_assignment
// FileName: Node.cpp

/*
*  Author:  Peace
*  Date:    2021/5/14
*  Email:   caiml20@nudt.edu.cn
*  Desc:    define class Node
*/

#include "Node.h"

using namespace std;

int dirX[4] = {0, 0, 1, -1}; // RIGHT-LEFT-DOWN-UP
int dirY[4] = {1, -1, 0, 0}; // RIGHT-LEFT-DOWN-UP
int Node::boardSqSize = 0;


Node::Node()
{
    emptyNode = true;
    A = new puzzle_t *[boardSqSize];
    for (int i = 0; i < boardSqSize; ++i)
    {
        A[i] = new puzzle_t[boardSqSize];
        memset(A[i], 0, boardSqSize * sizeof(A[0][0]));
    }
}


Node::Node(const Node &node)
{
    this->~Node();
//    emptyNode = false;
    this->emptyNode = node.emptyNode;
    A = new puzzle_t *[boardSqSize];
    for (int i = 0; i < boardSqSize; ++i)
    {
        A[i] = new puzzle_t[boardSqSize];
    }
    for (int i = 0; i < boardSqSize; i++)
    {
        for (int j = 0; j < boardSqSize; j++)
        {
            A[i][j] = node.A[i][j];
        }
    }
}


Node::~Node()
{
    if (A == nullptr) return;
    for (int i = 0; i < boardSqSize; ++i)
    {
        delete[] A[i];
    }
    delete[] A;
    A = nullptr;
}


ostream &operator<<(ostream &os, const Node &node)
{
    if (!node.A) return os;
    for (int i = 0; i < Node::boardSqSize; i++)
    {
        for (int j = 0; j < Node::boardSqSize; j++)
            os << setw(PRINT_W) << (static_cast<int>(node.A[i][j])) << " ";
        os << endl;
    }
    os << " ----------- " << std::endl;
    return os;
}


Node &Node::operator=(const Node &node)
{
    this->~Node();
    this->emptyNode = node.emptyNode;
    A = new puzzle_t *[boardSqSize];
    for (int i = 0; i < boardSqSize; ++i)
    {
        A[i] = new puzzle_t[boardSqSize];
    }
    for (int i = 0; i < boardSqSize; i++)
    {
        for (int j = 0; j < boardSqSize; j++)
        {
            A[i][j] = node.A[i][j];
        }
    }
    return *this;
}


bool Node::operator==(const Node &right) const
{
    for (int i = 0; i < boardSqSize; i++)
        for (int j = 0; j < boardSqSize; j++)
            if (A[i][j] != right.A[i][j]) return false;
    return true;
}


bool Node::operator!=(const Node &right) const
{
    return !(*this == right);
}


bool Node::operator<(const Node &right) const
{
    for (int i = 0; i < boardSqSize; i++)
    {
        for (int j = 0; j < boardSqSize; j++)
        {
            if (A[i][j] < right.A[i][j]) return true;
            else if (A[i][j] == right.A[i][j]) continue;
            else return false;
        }
    }
    return false;
}


int Node::getInvCount(const std::vector<int> &arr)
{
    int inv_count = 0;
    for (int i = 0; i < arr.size() - 1; i++)
        for (int j = i + 1; j < arr.size(); j++)
            if (arr[i] > arr[j])
                inv_count++;

    return inv_count;
}


bool Node::isSolveAble()
{
    int blank_row_no = -1;
    vector<int> arr;
    for (int i = 0; i < boardSqSize; i++)
        for (int j = 0; j < boardSqSize; j++)
        {
            if (A[i][j])
                arr.push_back(A[i][j]);
            else
                blank_row_no = i;
        }
    int invCount = getInvCount(arr);
    bool boardSizeOdd = static_cast<bool>(boardSqSize & 1);
//    cout << boardSizeOdd << " " << blank_row_no << " " << invCount << endl;
    if (boardSizeOdd && !(invCount & 1)) // odd-board & even-inversions
        return true;
    else if (!boardSizeOdd && ((blank_row_no + getInvCount(arr)) & 1)) // even-board & odd-sum
        return true;
    return false;
}


bool Node::getZeroPos(const Node &node, int &zX, int &zY)
{
    zX = zY = -1;
    for (int i = 0; i < Node::boardSqSize; i++)
    {
        for (int j = 0; j < Node::boardSqSize; j++)
            if (!node.A[i][j])
            {
                zX = i, zY = j;
                return true;
            }
    }
    return false;
}


Node Node::getNode(int direction, int zX, int zY)
{
    if (A == nullptr || direction > 3)
        return *this;

    if (zX == -1 || zY == -1)
        if (!getZeroPos(*this, zX, zY))
            return Node();

    int zXnew = zX + dirX[direction];
    int zYnew = zY + dirY[direction];

    if (zXnew < 0 || zYnew < 0 || zXnew >= Node::boardSqSize || zYnew >= Node::boardSqSize)
        return Node();

    Node v = *this;
//    cout << v;
    swap(v.A[zX][zY], v.A[zXnew][zYnew]);
    return v;
}


int Node::oppositeDirection(int direction)
{
    switch (direction)
    {
        case LEFT:
            return RIGHT;
        case RIGHT:
            return LEFT;
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        default:
            return EOF;
    }
}

bool Node::isEmptyNode() const
{
    return emptyNode;
}

