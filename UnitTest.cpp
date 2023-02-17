//
// Created by 张飞扬 on 2023/1/22.
//
#include "BaseMap.h"
void DequeTest(){
    Node a(1,2,3,4,5,0,NULL),b(2,3,4,5,6,7,NULL),c(3,4,5,6,7,0,NULL),d(4,5,6,7,8,0,NULL);
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