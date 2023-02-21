//
// Created by 张飞扬 on 2023/2/21.
//

#ifndef BASE_STATION_FAKE_BASE_H
#define BASE_STATION_FAKE_BASE_H
#include "BaseMap.h"
#include "myTime.h"

class fakeBase {
public:
   double StartXPoint;
   double StartYPoint;
   double EndXPoint;
   double EndYPoint;
   double Speed;
   Time Start;
   Time End;
   int key;
   void change(double StartX,double StartY,double EndX,double EndY,double Speed,int hour,int minute,int key);
   void calculateXY(Time now,double &X,double &Y);
   fakeBase();
};

class fakeBaseMap {
public:
    fakeBase *group;
    int number;
    int MaxSize;
    void resize(int size);
    KDTree* GetMap(Time now);
public:
    fakeBaseMap(string filename);
    Node* NearestFakeFind(Time now,int x,int y);
};
#endif //BASE_STATION_FAKE_BASE_H
