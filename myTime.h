//
// Created by 张飞扬 on 2023/2/21.
//

#ifndef BASE_STATION_MY_TIME_H
#define BASE_STATION_MY_TIME_H


class Time{
public:
    int hour;//小时
    int minute;//分钟
    double second;//秒
    Time(int hour,int minute,double second);//初始化函数
    void change(int hour,int minute,double second);//修改时间，自动进位
    void show();//显示当前的时间
    Time(const Time &a);//拷贝函数
    void copy(Time &a);//将a的值复制到当前时间类中
    int checkIN(Time start,Time end);//检查当前时间是否在开始和结束时间之间
};


#endif //BASE_STATION_MY_TIME_H
