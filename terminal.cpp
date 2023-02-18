//
// Created by 张飞扬 on 2023/1/22.
//

#include "terminal.h"
#define 精度 100
void Time::change(int hour,double minute) {
    if(minute>=60){
        this->minute = (int)minute%60;
        this->hour = hour + (int)minute/60;
    }
    else {
        this->hour = hour;
        this->minute = minute;
    }
    if(this->hour >24){
        this->hour /=24;
    }
}

Time::Time(int hour,double minute) {
    this->hour = hour;
    this->minute = minute;
}

void Time::show() {
    cout<<"在"<<hour<<":"<<minute<<"时";
}

void terminal::change(double StartXPoint, double StartYPoint, double EndXPoint, double EndYPoint, double Speed, int hour,double minute) {
    this->StartXPoint = StartXPoint;
    this->StartYPoint = StartYPoint;
    this->EndXPoint = EndXPoint;
    this->EndYPoint = EndYPoint;
    this->Speed = Speed;
    Start.change(hour,minute);
    now.change(hour,minute);
}

Node* terminal::connectTick(BaseStationMap map) {
    Node *p=map.BaseStationSearch(StartXPoint+(EndXPoint-StartXPoint)/MaxTick*place,StartYPoint+(EndYPoint-StartYPoint)/MaxTick*place);
    place++;
    double length,time;
    length = sqrt(distance(StartXPoint,StartYPoint,EndXPoint,EndYPoint))/MaxTick;
    time = length/Speed/1000.0*60.0;
    now.change(now.hour,now.minute+time);
    now.show();
    cout<<"位于("<<StartXPoint+(EndXPoint-StartXPoint)/MaxTick*place<<","<<StartYPoint+(EndYPoint-StartYPoint)/MaxTick*place<<")处"<<endl;
    return p;
}

void terminal::change(int tick) {
    MaxTick = tick;
    place = 0;
}

terminal::terminal():StartXPoint(0),StartYPoint(0),EndXPoint(0),EndYPoint(0),Speed(0),now(0,0),Start(0,0),MaxTick(精度),place(0){
}



System::System(string filename) {
    ifstream yd001;
    yd001.open(filename,ios::in);
    maxsize = 4;
    number = 0;
    group = new terminal[4];
    if(!yd001.is_open())
    {
        cout<<"读取失败"<<endl;
        throw uncaught_exception();
    }
    string buf;
    getline(yd001,buf);//读取文件开头
    if(!strcmp(buf.c_str(),"YD"))
    {
        cout<<"文件开头错误"<<endl;
    }
    double XPoint1,YPoint1,XPoint2,YPoint2,Speed;
    int hour,minute;
    while(getline(yd001,buf)){
        sscanf(buf.c_str(),"%lf,%lf,%lf,%lf,%lf,%d,%d",&XPoint1,&YPoint1,&XPoint2,&YPoint2,&Speed,&hour,&minute);
        if(XPoint1 == -1 && YPoint1 == -1)break;
        group[number].change(XPoint1,YPoint1,XPoint2,YPoint2,Speed,hour,minute);
        number++;
        if(number == maxsize)resize(maxsize*2);
    }
}

void System::resize(int size) {
    terminal *a = new terminal[size];
    maxsize = size;
    for(int i = 0 ; i <number; i++){
        a[i] = group[i];
    }
    group = a;
}

terminal &System::operator[](int a) {
    if(a >= number)throw uncaught_exception();
    else return group[a];
}

void System::printAll() {
    for (int i = 0; i < number; ++i) {
        cout<<group[i].StartXPoint<<","<<group[i].StartYPoint<<endl;
    }
}
void System::run(BaseStationMap &map,int degree) {
    for(int i=0;i<number;i++){
        cout<<"第"<<i<<"个终端的情况"<<endl;
        group[i].MaxTick = degree;
        for(int j=0;j<group[i].MaxTick;j++){
            Node *p=group[i].connectTick(map);
            if(p == NULL)
                cout<<"此时没有信号"<<endl;
            else{
                cout<<"连接到位于("<<p->XPoint<<","<<p->YPoint<<")的编号为"<<p->number<<"的基站"<<endl;
            }
        }
        cout<<endl;
    }
}

