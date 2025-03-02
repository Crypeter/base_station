#include "KdTreeBaseMap.h"
#include "terminal.h"
#include "BlocksBaseMap.h"
using namespace std;
mutex tex;
int main(int argc, char const *argv[]) {
    string name1("../实验数据/jz001.txt"),name2("../实验数据/jz002.txt"),name3("../实验数据/wz001.txt"),name4("../实验数据/yd001.txt");
    char cmd[20];
    BaseStationMap *map = new BaseStationMap(name1);//town=62,village=23,fastRoad=7
    map->readfile(name2);
    System *tem = new System(name4);
    tem->addFake(name3);
    BlocksBaseMap *map2 = new BlocksBaseMap;
    map2->readfile(name1);
    map2->readfile(name2);
    //map2->nearestFind(10000,23500,2);
    /*
     * 可以使用的命令
     * loadMap/lm [filename]                                    加载系统构建地图，未输入则默认为jz001.txt
     * addMap/am [filename]                                     向现有地图中添加文件
     * loadSystem/ls [filename]                                 加载系统文件构建系统，未输入则默认为yd001.txt
     * loadFake/lf [filename]                                   加载伪基站文件构建系统，未输入则默认为wz001.txt
     * blockDisplay/bs [Xi][Yi]                                 显示某个区块及其中的点，通过Xi和Yi来指定区块
     * find/f [XPoint] [YPoint] [n] [class]                     在地图中寻找离输入的坐标最近的点，n表示搜索点的个数，class表示搜索点的类型
     * test/t [XPoint] [YPoint]                                 对地图上某个点进行实验，看是否有信号
     * rangeFind/rf [X1Point] [Y1Point] [X2Point] [Y2Point]     寻找矩形范围内的点
     * display/d [class]                                        显示地图中的k-d树，从根节点开始进行显示
        * lChild/l                                              访问当前节点的左儿子
        * rChild/r                                              访问当前节点的右儿子
        * father/f                                              访问当前节点的父亲节点
        * all/a                                                 显示所有叶节点
        * back/b                                                退出当前对树的访问
     * run/r [degree]                                           根据地图和系统，计算终端通过的轨迹上连接的基站序列，degree为当前计算的精度
     * blockRun/bkr [degree]                                    根据分块法计算终端通过轨迹上连接的基站序列，degree为精度，计算精确度及效率均不如k-d树法
     * betterRun/br                                             根据地图和系统，精确计算连接的基站的时间,以及路径上连接的伪基站
     * exit                                                     退出
     */
    while(1) {
        cin>>cmd;
        if(strcmp(cmd,"loadMap") == 0 || strcmp(cmd,"lm") == 0){
            char filename[100];
            cout<<"请输入文件路径"<<endl;
            cin >> filename;
            map = new BaseStationMap(filename);
            cout<<"地图k-d树建立完成"<<endl;
        } else if (strcmp(cmd,"addMap") == 0 || strcmp(cmd,"am") == 0){
            char filename[100];
            cout<<"请输入文件路径"<<endl;
            cin >> filename;
            map->readfile(filename);
        } else if (strcmp(cmd,"loadSystem") == 0 || strcmp(cmd,"ls") == 0){
            char filename[100];
            cout<<"请输入文件路径"<<endl;
            cin >> filename;
            tem = new System(filename);
            cout<<"系统建立完成"<<endl;
        } else if(strcmp(cmd,"loadFake") == 0 || strcmp(cmd,"lf") == 0){
            char filename[100];
            cout<<"请输入文件路径"<<endl;
            cin >> filename;
            tem->addFake(filename);
            cout<<"已添加伪基站"<<endl;
            //tem->includeFake(*map,1000);
        }else if(strcmp(cmd,"blockDisplay") ==0 || strcmp(cmd,"bs") ==0){
            int Xi,Yi;
            cout<<"请输入区块X编号和Y编号（以空格隔开）"<<endl;
            cout<<"且XY均小于"<<map2->size<<endl;
                cin>>Xi>>Yi;
            map2->blockDisplay(Xi,Yi);
            cout<<endl;
        }else if (strcmp(cmd,"find") == 0 || strcmp(cmd,"f") == 0){
            double X,Y;
            int n,Class;
            cout<<"请输入x和y坐标（以空格隔开）"<<endl;
            cin>>X>>Y;
            cout<<"请输入寻找点的个数"<<endl;
            cin>>n;
            if(n == 1)
            {
                Node *p = map->BaseStationSearch(X,Y);
                if(p == NULL){
                    cout<<"超出信号范围"<<endl;
                }
                else {
                    p->show();
                }
            }
            else if ( n>1 ){
                while(1){
                cout<<"请输入寻找点的类型"<<endl;
                cout<<"0为城镇，1为乡镇，2为高速"<<endl;
                cin>>Class;
                if(Class >= 0 && Class <= 2){
                    break;
                    }
                else cout<<"错误的类型,请重新输入"<<endl;
                }
                string name;
                switch (Class) {
                    case 0:name = "城区";break;
                    case 1:name = "乡镇";break;
                    case 2:name = "高速";
                }
                Deque store = map->NBaseStationSearch(X,Y,n,name);
                store.printAll();
            }
        } else if(strcmp(cmd,"test") == 0 || strcmp(cmd,"t") == 0){
            int X,Y;
            cout<<"请输入X,Y坐标"<<endl;
            cin>>X>>Y;
            Node *p = map->BaseStationSearch(X,Y);
            if(p == NULL){
                cout<<"超出信号范围"<<endl;
            }
            else {
                p->show();
            }
        }else if (strcmp(cmd,"rangeFind") == 0 || strcmp(cmd,"rf") == 0) {
            int n;
            n = 0;
            double x1,x2,y1,y2;
            cout<<"请输入第一个x坐标和y坐标（以空格隔开）"<<endl;
            cin>>x1>>y1;
            cout<<"请输入第二个x坐标和y坐标（以空格隔开）"<<endl;
            cin>>x2>>y2;
            Deque store = map->RangeSearch(x1,y1,x2,y2,n);
            cout<<"在该范围内共找到"<<n<<"个点"<<endl;
            //store.printAll();
        } else if (strcmp(cmd,"display") == 0 || strcmp(cmd,"d") == 0) {
            int Class;
            Node *p;
            while(1){
                cout<<"请输入访问的树"<<endl;
                cout<<"0为城镇，1为乡镇，2为高速"<<endl;
                cin>>Class;
                if(Class >= 0 && Class <= 2){
                    break;
                }
                else cout<<"错误的类型,请重新输入"<<endl;
            }
            KDTree *myTree = map->getKDTree(Class);
            p = myTree->getRoot();
            while(1){
                char next[100];
                p->show();
                if(p->vector == 0){
                    cout<<"当前为x分割,向左x更小,向右x更大"<<endl;
                }
                else if(p->vector == 1){
                    cout<<"当前为y分割,向左y更小,向右y更大"<<endl;
                }
                cout<<"请输入要访问的节点"<<endl;
                cin>>next;
                if(strcmp(next,"lChild") == 0 || strcmp(next,"l") == 0){
                    if(p->lChild != NULL){
                        p = p->lChild;
                    }
                    else cout<<"左儿子为空"<<endl;
                }else if(strcmp(next,"rChild") == 0 || strcmp(next,"r") == 0){
                    if(p->rChild != NULL){
                        p = p->rChild;
                    }
                   else cout<<"右儿子为空"<<endl;
                }else if(strcmp(next,"father") == 0 || strcmp(next,"f") == 0){
                    if(p->father != NULL){
                        p = p->father;
                    }
                    else cout<<"该节点为根节点"<<endl;
                }else if(strcmp(next,"all") == 0 || strcmp(next,"a") == 0){
                    myTree->showAllLeave();
                }else if(strcmp(next,"back") == 0 || strcmp(next,"b") == 0){
                    cout<<"已退出树访问"<<endl;
                    break;
                }else{
                    cout<<"指令错误"<<endl;
                }
            }
        } else if (strcmp(cmd,"run") == 0 || strcmp(cmd,"r") == 0) {
            int degree;
            cout<<"请输入本次运行的精度"<<endl;
            cin>>degree;
            tem->run(*map,degree);
        }else if (strcmp(cmd,"blockRun") == 0 || strcmp(cmd,"bkr") == 0) {
            int degree;
            cout<<"请输入本次运行的精度"<<endl;
            cin>>degree;
            tem->run(*map2,degree);
        } else if (strcmp(cmd,"betterRun") == 0 || strcmp(cmd,"br") == 0) {
            tem->betterRun(*map, 1000);
        } else if (strcmp(cmd,"exit") == 0) {
                cout<<"程序已退出"<<endl;
                break;
        } else {
            cout<<"错误的命令"<<endl;
        }
    }
    delete map;
    delete tem;
    return 0;

}
