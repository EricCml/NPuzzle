"""
ProjName: NPuzzle_assignment
FileName: generator.py
Author:   Peace
Date:     2021/5/14
Email:    caiml20@nudt.edu.cn
Desc:     generate testcase
"""

import sys
import argparse
import random


def make_puzzle(s, solvable, iterations):
    def swap_empty(p):
        # 获取0的位置
        idx = p.index(0)
        poss = []
        # 如果不在第一列，取左边
        if idx % s > 0:
            poss.append(idx - 1)
        # 如果不在最后一列，取右边
        if idx % s < s - 1:
            poss.append(idx + 1)
        # 如果不在第一行，取上面
        if idx / s > 0 and idx - s >= 0:
            poss.append(idx - s)
        # 如果不在最后一行，取下面
        if idx / s < s - 1:
            poss.append(idx + s)
        # 随机选择一个进行替换
        swi = random.choice(poss)
        p[idx] = p[swi]
        p[swi] = 0

    p = make_goal(s)
    # 输出目标状态
    show_goal(p, s)
    for i in range(iterations):
        swap_empty(p)

    if not solvable:
        if p[0] == 0 or p[1] == 0:
            p[-1], p[-2] = p[-2], p[-1]
        else:
            p[0], p[1] = p[1], p[0]

    return p


def make_goal(s):
    # 棋盘尺寸为偶数时：0元素在棋盘右下角
    if s % 2 == 0:
        goal_lst = [i for i in range(1, s ** 2)]
        goal_lst.append(0)
    # 棋盘尺寸为奇数时：0元素在棋盘中心
    else:
        goal_lst_lhalf = [i for i in range(1, s * s // 2 + 1)]
        goal_lst_middle = [0]
        goal_lst_rhalf = [i for i in range(s * s // 2 + 1, s * s)]
        goal_lst = goal_lst_lhalf + goal_lst_middle + goal_lst_rhalf
    return goal_lst


def show_goal(goal_lst, s):
    w = len(str(s ** 2))
    print("# The puzzle's goal state:")
    for i in range(s):
        for j in range(s):
            print(str(goal_lst[i * s + j]).rjust(w), end=' ')
        print()


def show_puzzle(puzzle_lst, s):
    w = len(str(s ** 2))
    print("# This puzzle is %s:" % ("solvable" if solv else "unsolvable"))

    print("%d" % s)
    for i in range(s):
        for j in range(s):
            print("%s" % (str(puzzle_lst[i * s + j]).rjust(w)), end=' ')
        print()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("size", type=int, help="Size of the puzzle's side. Must be >3.")
    parser.add_argument("-s", "--solvable", action="store_true", default=False,
                        help="Force generation of a solvable puzzle. Overrides -u.")
    parser.add_argument("-u", "--unsolvable", action="store_true", default=False,
                        help="Force generation of an unsolvable puzzle")
    parser.add_argument("-i", "--iterations", type=int, default=10000, help="Number of passes")

    args = parser.parse_args()

    random.seed()

    if args.solvable and args.unsolvable:
        print("Can't be both solvable AND unsolvable, dummy !")
        sys.exit(1)

    if args.size < 3:
        print("Can't generate a puzzle with size lower than 2. It says so in the help. Dummy.")
        sys.exit(1)

    if not args.solvable and not args.unsolvable:
        solv = random.choice([True, False])
    elif args.solvable:
        solv = True
    elif args.unsolvable:
        solv = False

    s = args.size

    puzzle = make_puzzle(s, solvable=solv, iterations=args.iterations)

    show_puzzle(puzzle, s)
