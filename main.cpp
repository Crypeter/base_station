#include "BaseMap.h"
using namespace std;
int main() {
    string name1("../实验数据/jz001.txt");
    string name2("../实验数据/jz002.txt");
    string name3("../实验数据/test.txt");
    BaseStationMap map1(name3);
    //map1.readfile(name2);
    Node *p=map1.BaseStationSearch(4000,35010);
    int number;
    Deque a =map1.RangeSearch(1,1,9,9,number);
    if(p == NULL)
        cout<<"超出信号范围"<<endl;
    else
        cout<<p->XPoint<<","<<p->YPoint<<","<<p->range<<","<<p->number<<endl;
    return 0;
}
