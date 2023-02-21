//
// Created by 张飞扬 on 2023/2/21.
//

#ifndef BASE_STATION_FAKE_BASE_H
#define BASE_STATION_FAKE_BASE_H
#include "BaseMap.h"
#include "myTime.h"

class fakeBase {
   double StartXPoint;
   double StartYPoint;
   double EndXPoint;
   double EndYPoint;
   double Speed;
   Time Start;
   Time End;
   int number;
   fakeBase(double StartX,double StartY,double EndX,double EndY,double Speed,int hour,int minute,int number);
};

class fakeBaseMap {
    fakeBase *start;

};
#endif //BASE_STATION_FAKE_BASE_H
