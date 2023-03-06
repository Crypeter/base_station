cmake_minimum_required(VERSION 3.23)
project(base_station)

set(CMAKE_CXX_STANDARD 11)

add_executable(base_station main.cpp KdTreeBaseMap.cpp KdTreeBaseMap.h UnitTest.cpp terminal.cpp terminal.h fakeBase.cpp fakeBase.h myTime.cpp myTime.h BlocksBaseMap.cpp BlocksBaseMap.h)
