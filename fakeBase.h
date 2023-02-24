//
// Created by 张飞扬 on 2023/2/21.
//

#ifndef BASE_STATION_FAKE_BASE_H
#define BASE_STATION_FAKE_BASE_H
#include "BaseMap.h"
#include "myTime.h"

class fakeBase {
public:
   double StartXPoint;//伪基站初始x坐标
   double StartYPoint;//伪基站初始y坐标
   double EndXPoint;//伪基站终点x坐标
   double EndYPoint;//伪基站终点y坐标
   double Speed;//伪基站的移动速度
   Time Start;//开始时间
   Time End;//结束时间
   int key;//伪基站编号
   void change(double StartX,double StartY,double EndX,double EndY,double Speed,int hour,int minute,int key);//对伪基站进行初始化
   void calculateXY(Time now,double &X,double &Y);//计算对应时间伪基站所处的位置
   fakeBase();//
};

class fakeBaseMap {
private:
    fakeBase *group;//伪基站数组
    int number;//伪基站的数量
    int MaxSize;//数组的最大数量
    void resize(int size);//调整数组的大小
    KDTree* GetMap(Time now);//
public:
    fakeBaseMap(string filename);//
    Node* NearestFakeFind(Time now,int x,int y);//
};
#endif //BASE_STATION_FAKE_BASE_H
