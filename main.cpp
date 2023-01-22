#include "BaseMap.h"
using namespace std;
void DequeTest(){
    Node a(1,2,3,4,5,0),b(2,3,4,5,6,7),c(3,4,5,6,7,0),d(4,5,6,7,8,0);
    Deque test(3);
    //Node *aa =test.getNow();
    //cout<<aa->XPoint<<","<<aa->YPoint<<endl;
    test.add(&a);
//    for(int i=0;i<3;i++){
//        Node *p = test.get(i+1);
//        if(p!= NULL)cout<<p->XPoint<<","<<p->YPoint<<endl;
//    }
//    cout<<endl;
    test.add(&b);
    test.add(&c);
    //test.printAll();
    test.add(&d);
    test.add(&c);
    test.printAll();
}
int main() {
    string name1("../实验数据/jz001.txt");
    string name2("../实验数据/jz002.txt");
    string name3("../实验数据/test.txt");
    BaseStationMap map1(name1);
    //map1.readfile(name2);
    Node *p=map1.BaseStationSearch(4000,35010);
    Deque a =map1.town->NearNPointSearch(4000,35010,5);
    a.printAll();
    if(p == NULL)
        cout<<"超出信号范围"<<endl;
    else
        cout<<p->XPoint<<","<<p->YPoint<<","<<p->range<<","<<p->number<<endl;
    return 0;
}
