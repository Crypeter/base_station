//
// Created by 张飞扬 on 2023/2/21.
//

#ifndef BASE_STATION_MY_TIME_H
#define BASE_STATION_MY_TIME_H


class Time{
public:
    int hour;
    int minute;
    double second;
    Time(int hour,int minute,double second);//
    void change(int hour,int minute,double second);//修改时间，自动进位
    void show();//显示当前的时间
    Time(const Time &a);//拷贝函数
    int checkIN(Time start,Time end);
};


#endif //BASE_STATION_MY_TIME_H
