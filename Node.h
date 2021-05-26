// ProjName: NPuzzle_assignment
// FileName: Node.h

/*
*  Author:  Peace
*  Date:    2021/5/14
*  Email:   caiml20@nudt.edu.cn
*  Desc:    declare class Node
*/

#ifndef NPUZZLE_ASSIGNMENT_NODE_H
#define NPUZZLE_ASSIGNMENT_NODE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>

#define PRINT_W 3
#define RIGHT 0
#define LEFT 1
#define DOWN 2
#define UP 3

typedef int direction_t;
typedef int8_t puzzle_t;

extern int dirX[4]; // RIGHT-LEFT-DOWN-UP
extern int dirY[4]; // RIGHT-LEFT-DOWN-UP


class Node
{
public:
    puzzle_t **A = nullptr;
    bool emptyNode = true;
    static int boardSqSize;

public:
    Node(); // 无参数构造函数

    Node(const Node &node); // 有参数构造函数

    ~Node();    // 析构函数

    friend std::ostream &operator<<(std::ostream &os, const Node &node);    // 重载运算符<<

    Node &operator=(const Node &node);  // 重载运算符=

    bool operator==(const Node &right) const; // 重载运算符==

    bool operator!=(const Node &right) const; // 重载运算符!=

    bool operator<(const Node &right) const;  // 重载运算符<

    static int getInvCount(const std::vector<int> &arr);    //

    bool isSolveAble(); //判断是否有解

    static bool getZeroPos(const Node &node, int &zX, int &zY);

    Node getNode(int direction, int zX = -1, int zY = -1);

    static int oppositeDirection(int direction);

    bool isEmptyNode() const;
};

#endif //NPUZZLE_ASSIGNMENT_NODE_H
