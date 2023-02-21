//
// Created by 张飞扬 on 2023/2/21.
//

#include "fakeBase.h"

fakeBase::fakeBase():StartXPoint(0),StartYPoint(0),EndXPoint(0),EndYPoint(0),Speed(0),key(0),Start(0,0,0),End(0,0,0){
}

void fakeBase::change(double StartX, double StartY, double EndX, double EndY, double Speed, int hour, int minute,int number) {
    this->StartXPoint = StartX;
    this->StartYPoint = StartY;
    this->EndXPoint = EndX;
    this->EndYPoint = EndY;
    this->Speed = Speed;
    Start.change(hour,minute,0);
    double time = sqrt(distance(StartX,StartY,EndX,EndY))/Speed/1000*60.0*60.0;
    End.change(hour,minute,time);
    this->key = number;
}

void fakeBase::calculateXY(Time now,double &X,double &Y) {
    //time = 3.6*sqrt((X1-x2)^2+(y1-y2)^2)/speed
    //speedX =(x1-x2)/time*3.6
    //X = x1+time*speedX;
    double speedX = (EndXPoint - StartXPoint)/((End.hour-Start.hour)*3600+(End.minute-Start.minute)*60+End.second-Start.second);
    double speedY = (EndYPoint - StartYPoint)/((End.hour-Start.hour)*3600+(End.minute-Start.minute)*60+End.second-Start.second);
    X = StartXPoint + ((now.hour-Start.hour)*3600+(now.minute-Start.minute)*60+now.second-Start.second)*speedX;
    Y = StartYPoint + ((now.hour-Start.hour)*3600+(now.minute-Start.minute)*60+now.second-Start.second)*speedY;
}

fakeBaseMap::fakeBaseMap(string filename) {
    group = new fakeBase[4];
    MaxSize = 4;
    ifstream wz001;
    wz001.open(filename,ios::in);
    if(!wz001.is_open())
    {
        cout<<"读取失败"<<endl;
        throw uncaught_exception();
    }
    string buf;
    getline(wz001,buf);//读取文件开头
    if(!strcmp(buf.c_str(),"WZ"))
    {
        cout<<"文件开头错误"<<endl;
    }
    double XPoint1,YPoint1,XPoint2,YPoint2,Speed;
    int hour,minute,number;
    while(getline(wz001,buf)){
        sscanf(buf.c_str(),"%lf,%lf,%lf,%lf,%lf,%d,%d,%d",&XPoint1,&YPoint1,&XPoint2,&YPoint2,&Speed,&hour,&minute,&number);
        if(XPoint1 == -1 && YPoint1 == -1)break;
        group[this->number].change(XPoint1,YPoint1,XPoint2,YPoint2,Speed,hour,minute,number);
        this->number++;
        if(this->number == MaxSize)resize(MaxSize*2);
    }
}

void fakeBaseMap::resize(int size) {
    fakeBase *a = new fakeBase[size];
    MaxSize = size;
    for(int i = 0 ; i <number; i++){
        a[i] = group[i];
    }
}

KDTree *fakeBaseMap::GetMap(Time now) {
    KDTree *fake = new KDTree();
    double X,Y;
    for(int i = 0;i<number;i++){
        if(now.checkIN(group[i].Start,group[i].End)){
            group[i].calculateXY(now,X,Y);
            fake->put(X,Y,40,1,group[i].key);
        }
    }
    return fake;
}

Node *fakeBaseMap::NearestFakeFind(Time now, int x, int y) {
    KDTree *fake = GetMap(now);
    Node *f = fake->NearPointSearch(x,y);
    double powerF=0,distanceF=0;
    if(f != NULL){
        distanceF = pow(x-f->XPoint,2)+ pow(y-f->YPoint,2);
        powerF = f->power* pow(f->range,2)/distanceF;//计算离高速最近基站距离，distanceF = (x3-x0)^2+(y3-y0)^2
    }
    if(powerF < 1) return NULL;
    else return f;
}

