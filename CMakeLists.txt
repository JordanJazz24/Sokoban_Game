cmake_minimum_required(VERSION 3.25)
project(Sokoban_Game)

set(CMAKE_CXX_STANDARD 17)

add_executable(Sokoban_Game main.cpp Data/List.cpp Data/List.h Data/Node.h
        GameLogic/GameMenu.h)
