//
// Created by 张飞扬 on 2023/2/21.
//
#include "BaseMap.h"
#include "myTime.h"
void Time::change(int hour,int minute,double second) {
    if(second>=60 || second <0){
        this->second = (int)second%60;
        this->minute = minute + (int)second/60;
    }
    else {
        this->second = second;
        this->minute = minute;
    }
    if(second<0){
        this->second += 60;
        this->minute--;
    }
    if(minute>=60 || minute <0){
        this->minute = this->minute%60;
        this->hour = hour + this->minute/60;
    }
    else {
        this->hour = hour;
    }
    if(minute<0){
        this->minute += 60;
        this->hour--;
    }
    if(this->hour >24){
        this->hour /=24;
    }
}

Time::Time(int hour,int minute,double second) {
    this->hour = hour;
    this->minute = minute;
    this->second = second;
}

void Time::show() {
    cout<<"在"<<hour<<":"<<minute<<":"<<second<<"时";
}

Time::Time(const Time &a) {
    this->hour = a.hour;
    this->minute = a.minute;
    this->second = a.second;
}

int Time::checkIN(Time start, Time end) {
    double startSecond = start.hour*3600.0+start.minute*60.0+start.second;
    double endSecond = end.hour*3600.0+end.minute*60.0+end.second;
    double nowSecond = this->hour*3600+this->minute*60.0+this->second;
    if(nowSecond < startSecond)return 0;
    else if(nowSecond > endSecond)return 0;
    else return 1;
}
