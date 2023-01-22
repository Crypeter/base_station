cmake_minimum_required(VERSION 3.23)
project(base_station)

set(CMAKE_CXX_STANDARD 98)

add_executable(base_station main.cpp BaseMap.cpp BaseMap.h UnitTest.cpp terminal.cpp terminal.h)
