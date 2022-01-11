cmake_minimum_required(VERSION 3.0.0)

project(game VERSION 0.1.0 LANGUAGES CXX)

set(FLAGS_DEBUG "-std=c++11 -DNDEBUG -Wall --pedantic -errors -Werror")
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CPP_FLAGS ${FLAGS_DEBUG})

set(CPP_FILES Player.cpp ChainHashTable.cpp PlayerNode.cpp library2.cpp PlayersManager.cpp Group.cpp main2.cpp)

add_executable(game ${CPP_FILES})