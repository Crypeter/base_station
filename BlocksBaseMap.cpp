//
// Created by 张飞扬 on 2023/1/10.
//
//此文件为废弃的实现文件，最终未使用该文件进行计算
#include "BlocksBaseMap.h"

void BNode::copy(BNode *a) {
    X = a->X;
    Y = a->Y;
    strength = a->strength;
    range = a->range;
    number = a->number;
}

bool BNode::operator<(const BNode &a) const {
    return this->number<a.number;
}

block::block() {
    group = new BNode[4];
    maxSize = 4;
    number = 0;
}

void block::resize() {
    BNode *newOne = new BNode[maxSize*2];
    for(int i = 0; i< maxSize;i++){
        newOne[i].copy(group+i);
    }
    maxSize *=2;
    group = newOne;
}

void block::addPoint(double X, double Y, int range, double strength, int number) {
    int i = this->number;
    group[i].X = X;
    group[i].Y = Y;
    group[i].range = range;
    group[i].strength = strength;
    group[i].number = number;
    this->number++;
    if(this->number >= maxSize)resize();
}


blocks::blocks() {
    group = new block[4];
    MaxX = 120000;
    MinX = -100;
    MaxY = 50000;
    MinY = -100;
    size = 2;
    contain = 0;
}

int blocks::hashX(double X) {
    int a=(X-MinX),b=(MaxX-MinX)/size;
    return (a/b);
}

int blocks::hashY(double Y) {
    return (Y-MinY)/((MaxY-MinY)/size);
}

void blocks::addPoint(double X, double Y, int range, double strength, int number) {
    int Xi = hashX(X),Yi = hashY(Y);
    int hashPoint = (Yi*size)+Xi;
    group[hashPoint].addPoint(X,Y,range,strength,number);
    contain++;
    if(size*size/contain < 0.75)resize();
}

int blocks::checkInBlock(int Xi, int Yi) {
    if(Xi >= size)return 0;
    if(Yi >= size)return 0;
    if(Xi < 0)return 0;
    if(Yi < 0)return 0;
    return 1;
}

BNode *blocks::nearestFind(double X, double Y, int Class) {
    int blocksNumber = size*size;
    int *visit = new int[blocksNumber];
    for(int i = 0;i<blocksNumber;i++){
        visit[i]=0;
    }
    int Xi = hashX(X),Yi = hashY(Y);
    block *nowSearch = NULL;
    BNode *best = NULL;
    int level = 0;
    deque<findBlock*> store;
    store.push_front(new findBlock(Xi,Yi,0));
    visit[Yi*size+Xi]=1;
    while(!store.empty()) {
        findBlock *nowFind = store.back();
        nowSearch = group + (nowFind->Yi * size) + nowFind->Xi;
        for (int i = 0; i < nowSearch->number; i++) {
            if (nowSearch->group[i].range == 300 && Class == 1) {
                if (best == NULL)best = nowSearch->group + i;
                else {
                    if (distance(nowSearch->group[i].X, nowSearch->group[i].Y, X, Y) <
                        distance(best->X, best->Y, X, Y)) {
                        best = nowSearch->group + i;
                    }
                }
            }
            if (nowSearch->group[i].range == 1000 && Class == 2) {
                if (best == NULL)best = nowSearch->group + i;
                else {
                    if (distance(nowSearch->group[i].X, nowSearch->group[i].Y, X, Y) <
                        distance(best->X, best->Y, X, Y)) {
                        best = nowSearch->group + i;
                    }
                }
            }
            if (nowSearch->group[i].range == 5000 && Class == 3) {
                if (best == NULL)best = nowSearch->group + i;
                else {
                    if (distance(nowSearch->group[i].X, nowSearch->group[i].Y, X, Y) <
                        distance(best->X, best->Y, X, Y)) {
                        best = nowSearch->group + i;
                    }
                }
            }
        }
        store.pop_back();
        if(levelCheck(&store,level)){
            if(best!=NULL){
                return best;
            }
            else{
                level++;
            }
        }
        if(checkInBlock(nowFind->Xi+1,nowFind->Yi)&& NotVisited(nowFind->Xi+1,nowFind->Yi,visit)){
            visit[nowFind->Yi*size+nowFind->Xi+1]=1;
            store.push_front(new findBlock(nowFind->Xi+1,nowFind->Yi,nowFind->distance+1));
        }
        if(checkInBlock(nowFind->Xi-1,nowFind->Yi)&& NotVisited(nowFind->Xi-1,nowFind->Yi,visit)){
            visit[nowFind->Yi*size+nowFind->Xi-1]=1;
            store.push_front(new findBlock(nowFind->Xi-1,nowFind->Yi,nowFind->distance+1));
        }
        if(checkInBlock(nowFind->Xi,nowFind->Yi+1)&& NotVisited(nowFind->Xi,nowFind->Yi+1,visit)){
            visit[(nowFind->Yi+1)*size+nowFind->Xi]=1;
            store.push_front(new findBlock(nowFind->Xi,nowFind->Yi+1,nowFind->distance+1));
        }
        if(checkInBlock(nowFind->Xi,nowFind->Yi-1)&& NotVisited(nowFind->Xi,nowFind->Yi-1,visit)){
            visit[(nowFind->Yi-1)*size+nowFind->Xi]=1;
            store.push_front(new findBlock(nowFind->Xi,nowFind->Yi-1,nowFind->distance+1));
        }
        if(checkInBlock(nowFind->Xi+1,nowFind->Yi+1)&& NotVisited(nowFind->Xi+1,nowFind->Yi+1,visit)){
            visit[(nowFind->Yi+1)*size+nowFind->Xi+1]=1;
            store.push_front(new findBlock(nowFind->Xi+1,nowFind->Yi+1,nowFind->distance+1));
        }
        if(checkInBlock(nowFind->Xi+1,nowFind->Yi-1)&& NotVisited(nowFind->Xi+1,nowFind->Yi-1,visit)){
            visit[(nowFind->Yi-1)*size+nowFind->Xi+1]=1;
            store.push_front(new findBlock(nowFind->Xi+1,nowFind->Yi-1,nowFind->distance+1));
        }
        if(checkInBlock(nowFind->Xi-1,nowFind->Yi+1)&& NotVisited(nowFind->Xi-1,nowFind->Yi+1,visit)){
            visit[(nowFind->Yi+1)*size+nowFind->Xi-1]=1;
            store.push_front(new findBlock(nowFind->Xi-1,nowFind->Yi+1,nowFind->distance+1));
        }
        if(checkInBlock(nowFind->Xi-1,nowFind->Yi-1)&& NotVisited(nowFind->Xi-1,nowFind->Yi-1,visit)){
            visit[(nowFind->Yi-1)*size+nowFind->Xi-1]=1;
            store.push_front(new findBlock(nowFind->Xi-1,nowFind->Yi-1,nowFind->distance+1));
        }
    }
}

int levelCheck(deque<findBlock*> *store,int level){
    deque<findBlock*>::iterator it;
    int flag=1;
    for(it = store->begin();it!=store->end();it++){
        if((*it)->distance == level){
            flag = 0;
            break;
        }
    }
    return flag;
}
void blocks::resize() {
    size *= 2;
    block *newOne = new block[size*size];
    for(int i =0;i<size*size/4;i++){
        for(int j=0;j<group[i].number;j++){
            BNode *pNow = group[i].group+j;
            int newXi = hashX(pNow->X),newYi = hashY(pNow->Y);
            int newHashPoint = newYi*size+newXi;
            newOne[newHashPoint].addPoint(pNow->X,pNow->Y,pNow->range,pNow->strength,pNow->number);
        }
    }
    group = newOne;
}

int blocks::NotVisited(int Xi, int Yi, int *visit) {
    return visit[Yi*size+Xi] == 0;
}

blocks *blocks::getBlocks() {
    return this;
}

void BlocksBaseMap::put(double X, double Y, char *range, double power, int number) {
    if(strcmp(range,"城区") == 0)addPoint(X,Y,300,power,number);
    if(strcmp(range,"乡镇") == 0)addPoint(X,Y,1000,power,number);
    if(strcmp(range,"高速") == 0)addPoint(X,Y,5000,power,number);
}

void BlocksBaseMap::readfile(string filename) {
    ifstream file_jz001;
    file_jz001.open(filename,ios::in);
    if(!file_jz001.is_open())
    {
        cout<<"读取失败"<<endl;
        throw uncaught_exception();
    }
    string buf;
    getline(file_jz001,buf);//读取文件开头
    if(!strcmp(buf.c_str(),"JZ"))
    {
        cout<<"文件开头错误"<<endl;
    }
    double XPoint;//存储x坐标
    double YPoint;//存储y坐标
    char space[50];//存储位置
    double power;//存储强度
    int number;//存储编号
    while(getline(file_jz001,buf))
    {
        sscanf(buf.c_str(),"%lf,%lf,%s %lf,%d",&XPoint,&YPoint,space,&power,&number);
        if(XPoint == -1 && YPoint == -1)break;
        put(XPoint,YPoint,space,power,number);
        // cout<<XPoint<<","<<YPoint<<","<<space<<","<<power<<","<<number<<endl;
    }
}

void BlocksBaseMap::blockDisplay(int Xi, int Yi) {
    double dx=(MaxX-MinX)/size,dy = (MaxY-MinY)/size;
    printf("当前区块的左上角坐标为(%lf,%lf)\n",MinX+Xi*dx,MinY+Yi*dy);
    printf("dx = %lf,dy = %lf\n",dx,dy);
    printf("当前区块内的基站有%d个\n编号为",group[Yi*size+Xi].number);
    for(int i=0;i<group[Yi*size+Xi].number;i++){
        printf("%d ",group[Yi*size+Xi].group[i].number);
    }
}

BNode *BlocksBaseMap::bestFind(double X, double Y) {
    //BNode *town = nearestFind1(this,X,Y,1),*village = nearestFind1(this,X,Y,2),*fast = nearestFind1(this,X,Y,3);
    BNode *town = nearestFind(X,Y,1),*village = nearestFind(X,Y,2),*fast = nearestFind(X,Y,3);
    double pt,pv,pf;
    if(town!=NULL) { pt = town->strength * (town->range * town->range) / distance(town->X, town->Y, X, Y); }
    if(village!= NULL){pv = village->strength*(village->range*village->range)/ distance(village->X,village->Y,X,Y);}
    if(fast != NULL){pf = fast->strength*(fast->range*fast->range)/ distance(fast->X,fast->Y,X,Y);}
    if(pt < 1 && pv < 1 && pf < 1)return NULL;
    if(pt > pv && pt > pf)return town;
    if(pv >pt && pv >pf)return village;
    if(pf>pt && pf >pv)return fast;
}


findBlock::findBlock(int Xi, int Yi, int distance) {
    this->Xi = Xi;
    this->Yi = Yi;
    this->distance = distance;
}
