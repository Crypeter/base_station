//
// Created by 张飞扬 on 2023/1/10.
//
//此文件为废弃的头文件，最终未采用该数据结构进行计算
#ifndef BASE_STATION_BLOCKS_BASEMAP_H
#define BASE_STATION_BLOCKS_BASEMAP_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <cmath>
#include <set>
#include <thread>
#include <future>
double distance(double x1,double x2,double y1,double y2);//计算点与点之间点欧式几何距离
using namespace std;
class BNode{
public:
    double X;//X坐标
    double Y;//Y坐标
    int range;//基站半径
    double strength;//信号强度
    int number;//基站编号
    void copy(BNode *);//拷贝基站
    bool operator < (const BNode &) const;//比较基站编号
};
class block{
public:
    BNode *group;//基站数组
    int number;//基站的数量
    int maxSize;//最大的基站数量
    void resize();//达到最大数量进行动态调整
    block();
    void addPoint(double X,double Y,int range,double strength,int number);//向区块中加入点
};
class blocks{
public:
    double MaxX;//总块的最大X坐标
    double MaxY;//总块的最大Y坐标
    double MinX;//总块的最小X坐标
    double MinY;//总块的最小Y坐标
    int contain;//包含的基站个数
    block *group;//总块中的分区块
    blocks *getBlocks();
    int checkInBlock(int Xi,int Yi);//检查当前值是否在总块内
    int NotVisited(int Xi,int Yi,int *visit);//检验该区块是否被访问过
    void resize();//当总块的基站数达到一定数目后，将每个区块切分为4个更小的区块
    BNode *NearestFind(int Xi,int Yi,int Class,double X,double Y,int *visit);//找寻最近基站的辅助方法
public:
    int hashX(double X);//计算X坐标对应的X区块
    int hashY(double Y);//计算y坐标对应的Y区块
    int size;//总块每条边上的区块个数
    blocks();//初始化函数
    void addPoint(double X,double Y,int range,double strength,int number);//向总块中添加基站
    BNode *nearestFind(double X,double Y,int Class);//寻找最近基站
};
BNode *NearestFind(blocks *group,int Xi,int Yi,int Class,double X,double Y,int *visit);//找寻最近基站的辅助方法,为多线程准备
BNode *nearestFind1(blocks *group,double X,double Y,int Class);//寻找最近基站,为多线程准备
class BlocksBaseMap : public blocks{
public:
    void put(double X,double Y,char* range,double strength,int number);//向图中加入基站
    void readfile(string filename);//读取文件中的基站数据
    void blockDisplay(int Xi,int Yi);//显示指定区块
    BNode *bestFind(double X,double Y);//找到指定坐标附近信号最好的基站
};


#endif //BASE_STATION_BLOCKS_BASEMAP_H
