cmake_minimum_required(VERSION 3.17)
project(NPuzzle_assignment)

set(CMAKE_CXX_STANDARD 14)

# 主程序
add_executable(NPuzzle_assignment Node.cpp Node.h AStarSearch.cpp AStarSearch.h npuzzle.cpp)

# 测试文件
#add_executable(NPuzzle_assignment Node.cpp Node.h AStarSearch.cpp AStarSearch.h test.cpp)
