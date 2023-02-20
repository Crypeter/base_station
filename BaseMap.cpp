//
// Created by 张飞扬 on 2023/1/13.
//

#include "BaseMap.h"
double distance(double x1,double y1,double x2,double y2){
    return pow(x1-x2,2)+pow(y1-y2,2);
}

Node::Node(double XPoint, double YPoint, int range, double power, int number, int vector,Node* father) {
        this->XPoint = XPoint;
        this->YPoint = YPoint;
        this->range = range;
        this->power = power;
        this->number = number;
        this->vector = vector;
        this->lChild = NULL;
        this->rChild = NULL;
        this->father = father;
}

void Node::show() {
    cout<<"位于（"<<XPoint<<","<<YPoint<<")"<<"的点";
    cout<<" 范围为"<<range*power;
    cout<<" 强度为"<<power;
    cout<<" 编号为"<<number<<endl;
}

LinkNode *LinkNode::LinkNodeMake(Node *value, LinkNode *before) {
    LinkNode *now = new LinkNode;
    now->value = value;
    if(before != NULL)
        before->next = now;
    now->next = NULL;
    return now;
}

Deque::Deque(int i) {
    this->head = LinkNode::LinkNodeMake(NULL,NULL);
    LinkNode* L = this->head;
    for(int j = 1; j<i;j++){
        L = LinkNode::LinkNodeMake(NULL,L);
    }
    L->next = head;
    this->length = i;
    this->place = this->head;
}

void Deque::add(Node *value) {
    if(isFull()){
        this->place->value = value;
        this->place = this->place->next;
        this->head = this->place;
    }
    else{
        this->place->value = value;
        this->place = this->place->next;
    }
}

bool Deque::isFull() {
    return this->place == this->head;
}

Node* Deque::get(int i) {
    LinkNode *p = head;
    for(int j=0;j<i-1;j++){
        p = p->next;
    }
    if(p!= NULL)return p->value;
    else return NULL;
}

void Deque::printAll() {
    LinkNode *p = head;
    int i = length;
    while(i!=0){
        if(p->value !=NULL){
            p->value->show();
        }
        p=p->next;
        i--;
    }
}

Node* Deque::getNow() {
    LinkNode *p = head;
    while(p->next != place){
        p=p->next;
    }
    return p->value;
}

void KDTree::put(Node *now, double XPoint, double YPoint, int range, double power, int number) {
    if(now->vector == 0){//此处为x轴分化
        if(XPoint > now->XPoint){//大于进入右子树
            if(now->rChild == NULL)now->rChild = new Node(XPoint,YPoint,range,power,number,1,now);
            else put(now->rChild,XPoint,YPoint,range,power,number);
        }
        else {//小于进入左子树
            if(now->lChild == NULL)now->lChild = new Node(XPoint,YPoint,range,power,number,1,now);
            else put(now->lChild,XPoint,YPoint,range,power,number);
        }
    }
    else{//此处为y轴划分
        if(YPoint > now->YPoint){//大于则进入右子树
            if(now->rChild == NULL)now->rChild = new Node(XPoint,YPoint,range,power,number,0,now);
            else put(now->rChild,XPoint,YPoint,range,power,number);
        }
        else {//小于进入左子树
            if(now->lChild == NULL)now->lChild = new Node(XPoint,YPoint,range,power,number,0,now);
            else put(now->lChild,XPoint,YPoint,range,power,number);
        }
    }
}

void KDTree::put(double XPoint, double YPoint, int range, double power, int number) {
    if(root == NULL){
        root = new Node(XPoint,YPoint,range,power,number,0,NULL);
    }
    else put(root,XPoint,YPoint,range,power,number);
    return;
}

Node* KDTree::nearest(Node *now, double XPoint, double YPoint, Node *best) {
    Node *goodSide,*badSide;
    if(now == NULL)return best;
    if(distance(now->XPoint,now->YPoint,XPoint,YPoint) < distance(best->XPoint,best->YPoint,XPoint,YPoint))
        best = now;
    if(now->vector == 0){
        if(XPoint < now->XPoint){
            goodSide = now->lChild;
            badSide = now->rChild;
        }
        else{
            goodSide = now->rChild;
            badSide = now->lChild;
        }
    }
    else{
        if(YPoint < now->YPoint){
            goodSide = now->lChild;
            badSide = now->rChild;
        }
        else{
            goodSide = now->rChild;
            badSide = now->lChild;
        }
    }
    best = nearest(goodSide,XPoint,YPoint,best);
    if(now->vector == 0){
        if(distance(best->XPoint,best->YPoint,XPoint,YPoint) > abs(now->XPoint-XPoint)){
            best = nearest(badSide,XPoint,YPoint,best);
        }
    }
    else{
        if(distance(best->XPoint,best->YPoint,XPoint,YPoint) >abs(now->YPoint-YPoint)){
            best = nearest(badSide,XPoint,YPoint,best);
        }
    }
    return best;
}

void KDTree::nearestN(Node *now, double XPoint, double YPoint, Deque& best) {
    Node *goodSide,*badSide;
    if(now == NULL)return;
    if(distance(now->XPoint,now->YPoint,XPoint,YPoint) < distance(best.getNow()->XPoint,best.getNow()->YPoint,XPoint,YPoint))
        best.add(now);
    if(now->vector == 0){
        if(XPoint < now->XPoint){
            goodSide = now->lChild;
            badSide = now->rChild;
        }
        else{
            goodSide = now->rChild;
            badSide = now->lChild;
        }
    }
    else{
        if(YPoint < now->YPoint){
            goodSide = now->lChild;
            badSide = now->rChild;
        }
        else{
            goodSide = now->rChild;
            badSide = now->lChild;
        }
    }
    nearestN(goodSide,XPoint,YPoint,best);
    if(now->vector == 0){
        if(distance(best.getNow()->XPoint,best.getNow()->YPoint,XPoint,YPoint) > abs(now->XPoint-XPoint)){
            nearestN(badSide,XPoint,YPoint,best);
        }
    }
    else{
        if(distance(best.getNow()->XPoint,best.getNow()->YPoint,XPoint,YPoint) >abs(now->YPoint-YPoint)){
            nearestN(badSide,XPoint,YPoint,best);
        }
    }
    return;
}

Node *KDTree::NearPointSearch(double XPoint, double YPoint) {
    return nearest(root,XPoint,YPoint,root);
}

Deque KDTree::NearNPointSearch(double XPoint, double YPoint, int n) {
    Deque store(n);
    store.add(root);
    nearestN(root,XPoint,YPoint,store);
    return store;
}

void KDTree::RangeSearch(double MaxXPoint, double MaxYPoint, double MinXPoint, double MinYPoint, int &number, Deque &store) {
    rangeSearch(root,MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
}

void KDTree::rangeSearch(Node *now, double MaxXPoint, double MaxYPoint, double MinXPoint, double MinYPoint, int &number,Deque &store) {
    if(now == NULL)return;
    if(now->vector == 0){
        if(now->XPoint <= MaxXPoint && now->XPoint >= MinXPoint ){
            if(now->YPoint <= MaxYPoint && now->YPoint >=MinYPoint) {
                store.add(now);
                number++;
            }
            rangeSearch(now->lChild,MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
            rangeSearch(now->rChild,MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
        }
        else if(now->XPoint > MaxXPoint){
            rangeSearch(now->lChild,MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
        }
        else if(now->XPoint < MinXPoint){
            rangeSearch(now->rChild,MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
        }
    }
    else if(now->vector == 1){
        if(now->YPoint <= MaxYPoint && now->YPoint >= MinYPoint ){
            if(now->XPoint <= MaxXPoint && now->XPoint >=MinXPoint) {
                store.add(now);
                number++;
            }
            rangeSearch(now->lChild,MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
            rangeSearch(now->rChild,MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
        }
        else if(now->YPoint > MaxYPoint){
            rangeSearch(now->lChild,MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
        }
        else if(now->YPoint < MinYPoint){
            rangeSearch(now->rChild,MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
        }
    }
}

Node *KDTree::getRoot() {
    return root;
}

BaseStationMap::BaseStationMap(std::string filename) {
    ifstream file_jz001;
    this->town = new KDTree();
    this->village = new KDTree();
    this->fastRoad = new KDTree();
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
        //cout<<XPoint<<","<<YPoint<<","<<space<<","<<power<<","<<number<<endl;
    }
}

void BaseStationMap::readfile(std::string filename) {
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

void BaseStationMap::put(double XPoint, double YPoint, char *space, double power, int number) {
    if(strcmp(space,"城区") == 0)town->put(XPoint,YPoint,300,power,number);
    if(strcmp(space,"乡镇") == 0)village->put(XPoint,YPoint,1000,power,number);
    if(strcmp(space,"高速") == 0)fastRoad->put(XPoint,YPoint,5000,power,number);
}

Node *BaseStationMap::BaseStationSearch(double XPoint, double YPoint) {
    Node *t = town->NearPointSearch(XPoint,YPoint);
    Node *v = village->NearPointSearch(XPoint,YPoint);
    Node *f = fastRoad->NearPointSearch(XPoint,YPoint);
    double distanceT=0,distanceV=0,distanceF=0,powerT=0,powerV=0,powerF=0;
    if(t != NULL){
        distanceT = pow(XPoint-t->XPoint,2)+ pow(YPoint-t->YPoint,2);
        powerT = t->power*pow(t->range,2)/distanceT;
    }//计算离城镇最近基站距离，distanceT = (x1-x0)^2+(y1-y0)^2
    if(v != NULL){
        distanceV = pow(XPoint-v->XPoint,2)+ pow(YPoint-v->YPoint,2);
        powerV = v->power*pow(v->range,2)/distanceV;//计算离乡镇最近基站距离，distanceV = (x2-x0)^2+(y2-y0)^2
    }
    if(f != NULL){
        distanceF = pow(XPoint-f->XPoint,2)+ pow(YPoint-f->YPoint,2);
        powerF = f->power* pow(f->range,2)/distanceF;//计算离高速最近基站距离，distanceF = (x3-x0)^2+(y3-y0)^2
    }
    if(powerT < 1)powerT = 0;
    if(powerF < 1)powerF = 0;
    if(powerV < 1)powerV = 0;//超出信号范围无信号
    if(powerT == 0 && powerF == 0 && powerV == 0)return NULL;
    if(powerT > powerV && powerT > powerF)return t;
    if(powerV > powerT && powerV > powerF)return v;
    if(powerF > powerV && powerF > powerT)return f;
}

Deque BaseStationMap::NBaseStationSearch(double XPoint, double YPoint, int n, string name) {
    if(strcmp(name.c_str(),"城区") == 0)return town->NearNPointSearch(XPoint,YPoint,n);
    if(strcmp(name.c_str(),"乡镇") == 0)return village->NearNPointSearch(XPoint,YPoint,n);
    if(strcmp(name.c_str(),"高速") == 0)return fastRoad->NearNPointSearch(XPoint,YPoint,n);
}

Deque BaseStationMap::RangeSearch(double XPoint1, double YPoint1, double XPoint2, double YPoint2, int &number) {
    if(XPoint1 == XPoint2)throw uncaught_exception();
    if(YPoint1 == YPoint2)throw uncaught_exception();
    Deque store(MaxStore);//设置存储最大上限为100
    double MaxXPoint,MaxYPoint,MinXPoint,MinYPoint;
    if(XPoint1 > XPoint2){
        MaxXPoint = XPoint1;
        MinXPoint = XPoint2;
    }
    else{
        MaxXPoint = XPoint2;
        MinXPoint = XPoint1;
    }
    if(YPoint1 > YPoint2){
        MaxYPoint = YPoint1;
        MinYPoint = YPoint2;
    }
    else{
        MaxYPoint = YPoint2;
        MinYPoint = YPoint1;
    }
    town->RangeSearch(MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
    village->RangeSearch(MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
    fastRoad->RangeSearch(MaxXPoint,MaxYPoint,MinXPoint,MinYPoint,number,store);
    cout<<"矩形内的点有"<<number<<"个"<<endl;
    store.printAll();
    return store;
}

KDTree *BaseStationMap::getKDTree(int n) {
    switch (n) {
        case 0:return town;
        case 1:return village;
        case 2:return fastRoad;
    }
    return NULL;
}
