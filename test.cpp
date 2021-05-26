// ProjName: NPuzzle_assignment
// FileName: test.cpp

/*
*  Author:  Peace
*  Date:    2021/5/15
*  Email:   caiml20@nudt.edu.cn
*  Desc:    随机生成例子并输出
*/

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

#include "Node.h"
#include "AStarSearch.h"

using namespace std;


int findZero(int p[], const int s)
{
    for (int i = 0; i < s * s; ++i)
        if (p[i] == 0)
            return i;
    return -1;
}


int *makeGoals(const int s)
{
    const int num = s * s;
    int *goal = new int[num];
    // 棋盘尺寸为偶数时：0元素在棋盘右下角
    if (s % 2 == 0)
    {
        for (int i = 0; i < num - 1; ++i)
        {
            goal[i] = i + 1;
        }
        goal[num - 1] = 0;
    }
        // 棋盘尺寸为奇数时：0元素在棋盘中心
    else
    {
        for (int i = 0; i < num / 2; ++i)
        {
            goal[i] = i + 1;
        }
        goal[num / 2] = 0;
        for (int j = num / 2 + 1; j < num; ++j)
        {
            goal[j] = j;
        }
    }
    return goal;
}


void swapEmpty(int *p, const int s)
{
    // 获取0的位置
    int idx = findZero(p, s);
    vector<int> poss;
    // 如果不在第一列，取左边
    if (idx % s > 0)
        poss.push_back(idx - 1);
    // 如果不在最后一列，取右边
    if (idx % s < s - 1)
        poss.push_back(idx + 1);
    // 如果不在第一行，取上面
    if ((float) idx / s > 0 && idx - s >= 0)
        poss.push_back(idx - s);
    // 如果不在最后一行，取下面
    if ((float) idx / s < s - 1)
        poss.push_back(idx + s);

    // 设置随机种子
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(poss.begin(), poss.end(), default_random_engine(seed));
    // 随机选择一个下标
    int swi = poss.front();

    // 交换
    p[idx] = p[swi];
    p[swi] = 0;
}


int *makePuzzle(const int s, const int iterations)
{
    int *p;
    p = makeGoals(s);

    for (int i = 0; i < iterations; ++i)
        swapEmpty(p, s);
    return p;
}


void setGoal(int goal[], const int s, Node &Goal)
{
    for (int i = 0; i < s; ++i)
        for (int j = 0; j < s; ++j)
            Goal.A[i][j] = goal[i * s + j];
}


void setPuzzle(int puzzle[], const int s, Node &Start)
{
    for (int i = 0; i < s; ++i)
        for (int j = 0; j < s; ++j)
            Start.A[i][j] = puzzle[i * s + j];
}


void printSolution(aStarSearch &starSearch, const Node &Start, const Node &Goal)
{
    auto now = Goal;

    // 输出解的详细步骤
    vector<Node> Path;
    now = now.getNode(starSearch.visited[now].parent_);
    while (starSearch.visited[now].parent_ != EOF)
    {
        Path.push_back(now);
        now = now.getNode(starSearch.visited[now].parent_);
    }
    reverse(Path.begin(), Path.end());

    cout << "初始状态\n" << Start;
    int i = 1;
    for (auto &p:Path)
    {
        cout << "第" << i++ << "步" << endl;
        cout << p;
    }
    cout << "目标状态\n" << Goal << endl;
}


void executeSearch(const Node &Start, const Node &Goal, int heuristic, bool printSol = true)
{
    auto *starSearch = new aStarSearch();
    starSearch->setHeuristic(heuristic);
    auto startTime = chrono::steady_clock::now();
    int nExpanded = starSearch->AStarSearch(Start, Goal);
    auto endTime = chrono::steady_clock::now();

    auto diff = endTime - startTime;
    cout << "No of Steps: " << (int) starSearch->visited[Goal].cost_ << endl;
    cout << "No of Nodes Expanded: " << nExpanded << endl;
    cout << "No of Nodes Opened: " << starSearch->openedCount << endl;
    cout << "No of Nodes Pushed: " << starSearch->nPushed << endl;
    cout << "Max Depth Reached: " << starSearch->max_depth << endl;
    cout << "Execution Time: " << chrono::duration<double, milli>(diff).count() << "ms" << endl;
    cout << endl;
    fflush(stdout);

    if (printSol)
    {
        cout << "共" << (int) starSearch->visited[Goal].cost_ << "步" << endl;
        printSolution(*starSearch, Start, Goal);
    }
    delete starSearch;
}


int main()
{
    // 设置结果输出
    if (freopen(".\\Testcase\\Test_outcome\\random_outcome.txt", "w", stdout) == NULL)
        fprintf(stderr, "error redirecting stdout\n");

    /*
     * boardSqSize: 方针的维度
     * Node Start: 初始状态
     * Node Goal: 目标状态
     */
    int boardSqSize = 3;
    int *puzzle, *goal;

    // 生成目标数组
    goal = makeGoals(boardSqSize);
    // 生成初始数组
    puzzle = makePuzzle(boardSqSize, 100000);

    Node::boardSqSize = boardSqSize;
    Node Start;
    Node Goal;

    // 录入目标状态
    setGoal(goal, boardSqSize, Goal);
    // 录入初始状态
    setPuzzle(puzzle, boardSqSize, Start);

    cout << "Start: \n" << Start;
    cout << "Goal: \n" << Goal << endl;

    if (!Start.isSolveAble())
    {
        cout << "No Solution" << endl;
    } else
    {
        {
            cout << "# Misplaced Tiles Heuristics: " << endl;
            executeSearch(Start, Goal, MISPLACED_TILES, false);

            cout << "# Manhattan Distance Heuristics: " << endl;
            executeSearch(Start, Goal, MANHATTAN_DISTANCE, false);

            cout << "# Linear Conflicts Heuristics: " << endl;
            executeSearch(Start, Goal, LINEAR_CONFLICT, true);
        }
    }

    // 关闭输入输出流
    fclose(stdin);
    fclose(stdout);

    return 0;
}
