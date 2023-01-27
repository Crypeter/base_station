#include "BaseMap.h"
#include "terminal.h"
using namespace std;
int main() {
    string name1("../实验数据/jz001.txt"),name2("../实验数据/jz002.txt"),name3("../实验数据/test.txt"),name4("../实验数据/yd001.txt");
    BaseStationMap map1(name1);
    map1.readfile(name2);
//    Node *p=map1.BaseStationSearch(4000,35010);
//    int number;
//    Deque a =map1.RangeSearch(1,1,9,9,number);
//
//    if(p == NULL)
//        cout<<"超出信号范围"<<endl;
//    else
//        cout<<p->XPoint<<","<<p->YPoint<<","<<p->range<<","<<p->number<<endl;
    System b(name4);
    int count=0;
    for(int i=0;i<b.number;i++){
        cout<<"第"<<i<<"个终端的情况"<<endl;
        for(int j=0;j<b.group[i].MaxTick;j++){
            Node *p=b[i].connectTick(map1);
            if(p == NULL)
                cout<<"超出信号范围"<<endl;
            else{
                cout<<"连接到位于("<<p->XPoint<<","<<p->YPoint<<")的编号为"<<p->number<<"的基站"<<endl;
            }
        }
        cout<<endl;
    }
    //b.printAll();
    return 0;

}
