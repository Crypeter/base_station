//
// Created by 张飞扬 on 2023/1/22.
//

#ifndef BASE_STATION_TERMINAL_H
#define BASE_STATION_TERMINAL_H
#include "BaseMap.h"
#include "myTime.h"
#include "fakeBase.h"
int checkEqual(Node *a,Node *b);//检验基站是否相同
double checkIn(double x1,double y1,Node *p);//检验是否在基站信号范围内
void calculate(double x1,double x2,double y1,double y2,Node *p,double &endX,double &endY);//计算信号的边界点
class terminal {
public:
    double StartXPoint;//起点的x坐标
    double StartYPoint;//起点的y坐标
    double EndXPoint;//终点的x坐标
    double EndYPoint;//终点的y坐标
    double Speed;//速度
    Time Start;//开始的时间
    Time now;//当前的时间
    int MaxTick;//表示将路径划分为多少段
    int place;//表示当前在起点和终点连线上的哪一段
    terminal();
    void display();//显示当前终端的所在位置
    void change(int tick);//初始化终端精度
    void change(double StartXPoint,double StartYPoint,double EndXPoint,double EndYPoint,double Speed,int hour,int minute);//初始化终端
    Node* connectTick(BaseStationMap map);//每次tick行进1个精度，返回当前连接的基站
    Node* lastTick(BaseStationMap map);//查询上一个tick时连接的基站
    void getNowPlace(double &XPoint,double &YPoint);//获取终端当前的位置
    void getLastPlace(double &XPoint,double &YPoint);//获取终端在上一个tick时所处的位置
};
class System {
private:
    terminal* group;//存储终端数组
    fakeBaseMap *map;
    int number;//终端的数量
    int maxsize;//终端最大的存储数量
    void resize(int size);//超过上限后对数组大小进行调整
public:
    void printAll();
    terminal& operator[](int a);
    System(string filename);
    void addFake(string filename);
    void run(BaseStationMap &map,int degree);//对每个终端轨迹上连接的基站进行计算,不计算离开的时间
    void betterRun(BaseStationMap &map,int degree);//对每个终端轨迹上连接的基站进行计算，精准计算离开的时间和更换基站的时间点
    void includeFake(BaseStationMap &map,int degree);
};

#endif //BASE_STATION_TERMINAL_H
