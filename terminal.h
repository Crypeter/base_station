//
// Created by 张飞扬 on 2023/1/22.
//

#ifndef BASE_STATION_TERMINAL_H
#define BASE_STATION_TERMINAL_H
#include "BaseMap.h"
class Time{
public:
    int hour;
    double minute;
    Time(int hour,double minute);
    void change(int hour,double minute);
    void show();
};
class terminal {
public:
    double StartXPoint;
    double StartYPoint;
    double EndXPoint;
    double EndYPoint;
    double Speed;
    Time Start;
    Time now;
    int MaxTick;//表示将路径划分为多少段
    int place;//表示当前在起点和终点连线上的哪一段
    terminal();
    void change(int tick);
    void change(double StartXPoint,double StartYPoint,double EndXPoint,double EndYPoint,double Speed,int hour,double minute);
    Node* connectTick(BaseStationMap map);//每次tick行进1%，返回当前连接的基站
};
class System {
public:
    terminal* group;
    int number;
    int maxsize;
    void resize(int size);
    void printAll();
    terminal& operator[](int a);
    System(string filename);
};

#endif //BASE_STATION_TERMINAL_H
