//
// Created by 张飞扬 on 2023/1/22.
//

#include "terminal.h"
#define 精度 100
int checkEqual(Node *a,Node *b){
    if(a == NULL && b == NULL){
        return 1;//a和b都未连接到信号
    } else if(a == NULL || b == NULL){
        return 2;//a未连接到信号
    } else if(a->number == b->number){
        return 3;//a和b连接到同一个基站
    } else {
        return 4;//a和b连接到不同基站
    }
}


double checkIn(double x1, double y1, Node *p) {
    double dis = distance(x1,y1,p->XPoint,p->YPoint),R = p->power*pow(p->range,2);
    return dis - R;
}


void calculate(double x1, double x2, double y1, double y2, Node *p, double &endX, double &endY) {
    double middleX = (x1+x2)/2,middleY = (y1+y2)/2;
    double d1 = checkIn(x1,y1,p),d2 = checkIn(x2,y2,p),dm = checkIn(middleX,middleY,p);
    if(abs(dm)<0.1){
        endX = middleX;
        endY = middleY;
    }
    else if(d1 * dm < 0){
        calculate(x1,middleX,y1,middleY,p,endX,endY);
    }
    else if(d2 * dm < 0){
        calculate(middleX,x2,middleY,y2,p,endX,endY);
    }
    return;
}


void terminal::change(double StartXPoint, double StartYPoint, double EndXPoint, double EndYPoint, double Speed, int hour,int minute) {
    this->StartXPoint = StartXPoint;
    this->StartYPoint = StartYPoint;
    this->EndXPoint = EndXPoint;
    this->EndYPoint = EndYPoint;
    this->Speed = Speed;
    Start.change(hour,minute,0);
    now.change(hour,minute,0);
}

Node* terminal::connectTick(BaseStationMap map) {
    Node *p=map.BaseStationSearch(StartXPoint+(EndXPoint-StartXPoint)/MaxTick*(place),StartYPoint+(EndYPoint-StartYPoint)/MaxTick*place);
    place++;
    double length,time;
    length = sqrt(distance(StartXPoint,StartYPoint,EndXPoint,EndYPoint))/MaxTick;
    time = length/Speed/1000.0*60.0*60.0;
    now.change(now.hour,now.minute,now.second+time);
    return p;
}


void terminal::change(int tick) {
    MaxTick = tick;
    place = 0;
}


terminal::terminal():StartXPoint(0),StartYPoint(0),EndXPoint(0),EndYPoint(0),Speed(0),now(0,0,0),Start(0,0,0),MaxTick(精度),place(0){
}

Node *terminal::lastTick(BaseStationMap map) {
    if(place == 0 ||place == 1)return map.BaseStationSearch(StartXPoint,StartYPoint);
    else return map.BaseStationSearch(StartXPoint+(EndXPoint-StartXPoint)/MaxTick*(place-2),StartYPoint+(EndYPoint-StartYPoint)/MaxTick*(place-2));
}

void terminal::getNowPlace(double& XPoint,double& YPoint) {
    XPoint = StartXPoint+(EndXPoint-StartXPoint)/MaxTick*(place-1);
    YPoint = StartYPoint+(EndYPoint-StartYPoint)/MaxTick*(place-1);
}

void terminal::getLastPlace(double &XPoint, double &YPoint) {
    XPoint = StartXPoint+(EndXPoint-StartXPoint)/MaxTick*(place-2);
    YPoint = StartYPoint+(EndYPoint-StartYPoint)/MaxTick*(place-2);
}

void terminal::display() {
    Time last(now);
    double length = sqrt(distance(StartXPoint,StartYPoint,EndXPoint,EndYPoint))/MaxTick;
    double time = length/Speed/1000.0*60.0*60.0;
    last.change(last.hour,last.minute,last.second-time);
    last.show();
    cout<<"位于("<<StartXPoint+(EndXPoint-StartXPoint)/MaxTick*(place-1)<<","<<StartYPoint+(EndYPoint-StartYPoint)/MaxTick*(place-1)<<")处"<<endl;
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
    for(int i = 0;i<number;i++){
        group[i].place = 0;
        group[i].now.copy(group[i].Start);
    }
    int town(0),village(0),fastRoad(0);
    for(int i=0;i<number;i++){
        cout<<"第"<<i+1<<"段终端的情况"<<endl;
        if(degree >0){
            group[i].MaxTick = degree;
        }
        set<Node> townCollection,villageCollection,fastRoadCollection;
        for(int j=0;j<group[i].MaxTick;j++){
            Node *p=group[i].connectTick(map);
            if(p!= NULL){
                if(p->range == 300){
                    townCollection.insert(*p);
                }
                if(p->range == 1000){
                    villageCollection.insert(*p);
                }
                if(p->range == 5000){
                    fastRoadCollection.insert(*p);
                }
            }
        }
        set<Node>::iterator it;
        town += townCollection.size();
        village += villageCollection.size();
        fastRoad += fastRoadCollection.size();
        cout<<"连接到"<<townCollection.size()<<"个城市基站"<<endl<<"编号为";
        for(it = townCollection.begin();it != townCollection.end();it++){
            cout<<it->number<<",";
        }
        cout<<endl;
        cout<<"连接到"<<villageCollection.size()<<"个乡镇基站"<<endl<<"编号为";
        for(it = villageCollection.begin();it != villageCollection.end();it++){
            cout<<it->number<<",";
        }
        cout<<endl;
        cout<<"连接到"<<fastRoadCollection.size()<<"个高速基站"<<endl<<"编号为";
        for(it = fastRoadCollection.begin();it != fastRoadCollection.end();it++){
            cout<<it->number<<",";
        }
        cout<<endl;
        cout<<"共连接到"<<townCollection.size()+villageCollection.size()+fastRoadCollection.size()<<"个基站"<<endl;
        townCollection.clear();
        villageCollection.clear();
        fastRoadCollection.clear();
    }
    cout<<"连接到"<<town<<"个城市基站"<<endl;
    cout<<"连接到"<<village<<"个乡镇基站"<<endl;
    cout<<"连接到"<<fastRoad<<"个高速基站"<<endl;
    cout<<"共连接到"<<town+village+fastRoad<<"个基站"<<endl;
    cout<<endl;
}

void System:: betterRun(BaseStationMap &map, int degree) {
    for(int i = 0;i<number;i++){
        group[i].place = 0;
        group[i].now.copy(group[i].Start);
    }
    for(int i=0;i<number;i++){
        cout<<"第"<<i+1<<"段路径终端的情况"<<endl;
        if(degree >0){
            group[i].MaxTick = degree;
        }
        Time start(0,0,0),end(0,0,0);
        int flag = 1;
        for(int j=0;j<group[i].MaxTick;j++){
            Time last(group[i].now);
            last.change(last.hour,last.minute,last.second-sqrt(pow((group[i].StartXPoint-group[i].EndXPoint)/group[i].MaxTick,2)+pow((group[i].StartYPoint-group[i].EndYPoint)/group[i].MaxTick,2))/group[i].Speed*3.6);
            if(this->map != NULL){
                Node *pLastFake=this->map->NearestFakeFind(last,group[i].StartXPoint+(group[i].EndXPoint-group[i].StartXPoint)/group[i].MaxTick*(group[i].place-1),group[i].StartYPoint+(group[i].EndYPoint-group[i].StartYPoint)/group[i].MaxTick*(group[i].place-1));
                Node *pFake=this->map->NearestFakeFind(group[i].now,group[i].StartXPoint+(group[i].EndXPoint-group[i].StartXPoint)/group[i].MaxTick*group[i].place,group[i].StartYPoint+(group[i].EndYPoint-group[i].StartYPoint)/group[i].MaxTick*group[i].place);
                if(pLastFake == NULL && pFake != NULL) {
                    start.copy(group[i].now);
                    group[i].display();
                    cout<<"开始连接到编号为"<<pFake->number<<"的伪基站"<<endl;
                }
                else if(pLastFake != NULL && pFake == NULL) {
                    end.copy(group[i].now);
                    group[i].display();
                    cout<<"最后一次连接到编号为"<<pLastFake->number<<"的伪基站"<<endl;
                    cout<<"连接时间为"<<(end.hour-start.hour)*3600+(end.minute-start.minute)*60+(end.second-start.second)+sqrt(pow((group[i].StartXPoint-group[i].EndXPoint)/group[i].MaxTick,2)+pow((group[i].StartYPoint-group[i].EndYPoint)/group[i].MaxTick,2))/group[i].Speed*3.6<<"s"<<endl;
                }
            }
            Node *pNow=group[i].connectTick(map);
            Node *pLast=group[i].lastTick(map);
            double xIn,yIn,xOut,yOut,nowX,nowY,lastX,lastY,length,time;
            group[i].getNowPlace(nowX,nowY);
            group[i].getLastPlace(lastX,lastY);
            switch(checkEqual(pNow,pLast)){
                case 1:
                   // cout<<"此时没有信号"<<endl;
                case 3:
                    break;
                case 2:
                    if(pNow == NULL){
                        calculate(nowX,lastX,nowY,lastY,pLast,xIn,yIn);
                        Time out(group[i].now);
                        length = sqrt(distance(lastX,lastY,xIn,yIn))-2*sqrt(distance(group[i].StartXPoint,group[i].StartYPoint,group[i].EndXPoint,group[i].EndYPoint))/group[i].MaxTick;
                        time = length/group[i].Speed/1000.0*60.0*60.0;
                        out.change(out.hour,out.minute,out.second+time);
                        out.show();
                        cout<<"位于（"<<xIn<<","<<yIn<<")处"<<endl<<"离开编号为"<<pLast->number<<"的基站信号范围"<<endl;
                    }
                    if(pLast == NULL){
                        calculate(nowX,lastX,nowY,lastY,pNow,xOut,yOut);
                        Time out(group[i].now);
                        length = sqrt(distance(lastX,lastY,xOut,yOut))-2*sqrt(distance(group[i].StartXPoint,group[i].StartYPoint,group[i].EndXPoint,group[i].EndYPoint))/group[i].MaxTick;
                        time = length/group[i].Speed/1000.0*60.0*60.0;
                        out.change(out.hour,out.minute,out.second+time);
                        out.show();
                        cout<<"位于（"<<xOut<<","<<yOut<<")处"<<endl<<"进入编号为"<<pNow->number<<"的基站信号范围"<<endl;
                    }
                    break;
                case 4:
                    group[i].display();
                    double beforeX = lastX,beforeY = lastY,afterX = nowX,afterY = nowY;
                    while(checkIn(beforeX,beforeY,pNow) < 0){
                        beforeX = beforeX - (group[i].EndXPoint-group[i].StartXPoint)/group[i].MaxTick;
                        beforeY = beforeY - (group[i].EndYPoint-group[i].StartYPoint)/group[i].MaxTick;
                    }
                    while(checkIn(afterX,afterY,pLast) < 0){
                        afterX = afterX + (group[i].EndXPoint-group[i].StartXPoint)/group[i].MaxTick;
                        afterY = afterY + (group[i].EndYPoint-group[i].StartYPoint)/group[i].MaxTick;
                    }
                    calculate(beforeX,nowX,beforeY,nowY,pNow,xIn,yIn);//calculate 18000,30915
                    calculate(lastX,afterX,lastY,afterY,pLast,xOut,yOut);//在11:40:48.6时位于(18000,31470)处
                    length = sqrt(distance(xIn,yIn,xOut,yOut));
                    time = length/group[i].Speed/1000*60.0*60.0;
                    cout<<"经过重叠区的时间为"<<time<<endl;
                    cout<<"从位于（"<<pLast->XPoint<<","<<pLast->YPoint<<")处"<<"编号为"<<pLast->number<<"的基站切换到";
                    cout<<"位于（"<<pNow->XPoint<<","<<pNow->YPoint<<")处"<<"编号为"<<pNow->number<<"的基站信号范围"<<endl;
            }
        }
        cout<<endl;
    }
}

void System::includeFake(BaseStationMap &map, int degree) {
    for(int i=0;i<number;i++){
        cout<<"第"<<i<<"个终端的情况"<<endl;
        if(degree >0){
            group[i].MaxTick = degree;
        }
        for(int j=0;j<group[i].MaxTick;j++){
            Node *p2=this->map->NearestFakeFind(group[i].now,group[i].StartXPoint+(group[i].EndXPoint-group[i].StartXPoint)/group[i].MaxTick*group[i].place,group[i].StartYPoint+(group[i].EndYPoint-group[i].StartYPoint)/group[i].MaxTick*group[i].place);
            Node *p=group[i].connectTick(map);
            if(p2 == NULL){

            }
            else{
                group[i].display();
                cout<<"连接到位于("<<p2->XPoint<<","<<p2->YPoint<<")的编号为"<<p2->number<<"的伪基站"<<endl;
            }
        }
        cout<<endl;
    }
}

void System::addFake(string filename) {
    map = new fakeBaseMap(filename);
}


