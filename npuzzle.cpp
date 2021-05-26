// ProjName: NPuzzle_assignment
// FileName: npuzzle.cpp

/*
*  Author:  Peace
*  Date:    2021/5/15
*  Email:   caiml20@nudt.edu.cn
*  Desc:    读写特定文件，输出结果
*/


#include <chrono>
#include <iostream>

#include "Node.h"
#include "AStarSearch.h"

using namespace std;

#define MAX 105


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
    /*
     * in.txt文件结构：
     * 第一行指明维度N
     * 第2~N+1行指定初始状态
     * 最后N行设置目标状态
     */


    // 设置读入文件
    if (freopen(".\\Testcase\\Inputcase\\Npuzzle_in.txt", "r", stdin) == NULL)
        fprintf(stderr, "error redirecting stdin\n");

    // 设置结果输出
    if (freopen(".\\Testcase\\Outcome\\Npuzzle_out.txt", "w", stdout) == NULL)
        fprintf(stderr, "error redirecting stdout\n");

    /*
     * boardSqSize: 方针的维度
     * Node Start: 初始状态
     * Node Goal: 目标状态
     */
    int boardSqSize = 3;
    // 录入维度
    cin >> boardSqSize;
    Node::boardSqSize = boardSqSize;
    Node Start;
    Node Goal;
    int x;  //临时变量

    // 录入初始状态
    for (int i = 0; i < boardSqSize; i++)
    {
        for (int j = 0; j < boardSqSize; j++)
        {
            cin >> x;
            Start.A[i][j] = static_cast<puzzle_t>(x);
        }
    }

    // 录入目标状态
    for (int i = 0; i < boardSqSize; i++)
    {
        for (int j = 0; j < boardSqSize; j++)
        {
            cin >> x;
            Goal.A[i][j] = static_cast<puzzle_t>(x);
        }
    }

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
