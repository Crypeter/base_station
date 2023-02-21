//
// Created by 张飞扬 on 2023/1/13.
//
#ifndef BASE_STATION_BASEMAP_H
#define BASE_STATION_BASEMAP_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <cmath>
#define MaxStore 100
using namespace std;
double distance(double x1,double y1,double x2,double y2);//计算点与点之间点欧式几何距离
class Node{
public:
    double XPoint;//x坐标
    double YPoint;//y坐标
    int range;//辐射半径
    double power;//辐射强度
    int number;//基站编号
    int vector;//维度,0表示左右儿子为x轴分割,1表示左右儿子为y轴分割
    Node *lChild;//小于的儿子
    Node *rChild;//大于的儿子
    Node *father;//父亲节点
    Node(double XPoint,double YPoint,int range,double power,int number,int vector,Node* father);
    void show();//显示该节点内的信息
};
class LinkNode{
public:
    Node *value;//存储节点信息
    LinkNode *next;//链表连接
    static LinkNode *LinkNodeMake(Node *value,LinkNode *before);
};
class Deque{
private:
    LinkNode *head;
    LinkNode *place;
    int length;
public:
    Deque(int i);
    void add(Node *value);
    Node* get(int i);
    void printAll();
    bool isFull();
    Node *getNow();
};
class KDTree{
private:
    Node *root;
    void put(Node *now,double XPoint, double YPoint, int range, double power, int number);//向树中加入节点的辅助方法
    Node* nearest(Node *now,double XPoint,double YPoint,Node *best);//寻找最近节点的辅助方法
    void nearestN(Node *now,double XPoint,double YPoint,Deque& best);//寻找n个节点的辅助方法 结果存储在best这个队列中
    void rangeSearch(Node *now,double XPoint1,double YPoint1,double XPoint2,double YPoint2,int &number,Deque &store);//搜索矩形范围内的节点
public:
    KDTree(){root = NULL;};
    Node *getRoot();
    void put(double XPoint,double YPoint,int range,double power,int number);//向树中加入节点
    Node* NearPointSearch(double  XPoint,double YPoint);//寻找一个节点的最近节点
    Deque NearNPointSearch(double  XPoint,double YPoint,int n);//寻找最近的n个节点
    void RangeSearch(double XPoint1,double YPoint1,double XPoint2,double YPoint2,int &number,Deque &store);//搜索矩形范围内的节点
};
class BaseStationMap{
private:
    KDTree *town;//存储城区基站信息
    KDTree *village;//存储乡镇基站信息
    KDTree *fastRoad;//存储高速基站信息
public:
    KDTree *getKDTree(int n);
    BaseStationMap(string filename);
    void readfile(string filename);
    void put(double XPoint,double YPoint,char *space,double power,int number);//向图中加入节点
    Node* BaseStationSearch(double XPoint,double YPoint);//寻找离某个点信号最好的基站
    Deque NBaseStationSearch(double  XPoint,double YPoint,int n,string name);//寻找某个种类的n个基站
    Deque RangeSearch(double XPoint1,double YPoint1,double XPoint2,double YPoint2,int &number);//搜索矩形范围内的基站,number存储返回的个数
};
#endif //BASE_STATION_BASEMAP_H
