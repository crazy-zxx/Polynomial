#include <iostream>

using namespace std;

//多项式结构体
typedef struct item {
    double coefficient;     //系数
    int exponent;           //指数
    struct item *next;      //下一项
} Item, *Polynomial;

//查找并返回指向指定指数的多项式指针
Polynomial locateExponent(Polynomial p, int exponent) {
    Polynomial temp = p->next;
    while (temp != NULL) {
        if (temp->exponent == exponent) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

//删除q指定的项，并将q指向其原来的后继
void deletePoly(Polynomial &p, Polynomial &q) {
    Polynomial temp = p->next, pre = p;
    while (temp != NULL) {
        if (temp == q) {
            pre->next = temp->next;
            q = temp->next;
            free(temp);
            return;
        }
        pre = temp;
        temp = temp->next;
    }

}

//对多项式按指数升序排列
void sortPoly(Polynomial &p){
    Polynomial q=p->next,pre=p,temp;
    pre->next=NULL;
    while (q!=NULL){
        temp=q->next;   //防止断链

        //直接插入排序
        pre=p;
        while (pre->next!=NULL&&pre->next->exponent<q->exponent){
            pre=pre->next;
        }
        q->next=pre->next;
        pre->next=q;

        q=temp;
    }

}

//创建多项式
void createPoly(Polynomial &p) {
    p = (Polynomial) malloc(sizeof(Item));
    p->next = NULL;

    cout << "输入项数：";
    int n;
    cin >> n;

    if (n > 0) {
        cout << "输入每项系数、指数(如：10 2)：" << endl;
        while (n--) {
            Polynomial temp = (Polynomial) malloc(sizeof(Item)), q;
            cin >> temp->coefficient >> temp->exponent;
            temp->next=NULL;

            if (temp->coefficient!=0){      //忽略0系数项
                if ((q = locateExponent(p, temp->exponent)) != NULL) {  //合并到已有的同指数项
                    q->coefficient += temp->coefficient;
                    if (q->coefficient==0){ //合并后系数为0，则删除该节点
                        deletePoly(p,q);
                    }
                } else { //无同指数项，则插入新节点，头插法
                    temp->next = p->next;
                    p->next = temp;
                }
            }
        }
    }
    //按指数升序排列
    sortPoly(p);
}

//比较指数
int cmpExponent(int a, int b) {
    int result = a - b;
    return result == 0 ? 0 : (result > 0 ? 1 : -1);
}

//多项式加法
void addPoly(Polynomial &pa, Polynomial &pb) {

    Polynomial p = pa->next, q = pb->next;
    Polynomial pre_a = pa, pre_b = pb;

    while (p != NULL && q != NULL) {
        switch (cmpExponent(p->exponent, q->exponent)) {    //按照两多项式双指针对应比较位置指数判断取舍操作
            case 0:     //同指数，合并到pa
                p->coefficient += q->coefficient;
                if (p->coefficient == 0.0) {    //合并后系数为0，删除该节点
                    deletePoly(pa,p);
                } else{ //不为0，后移指针pre_a、p
                    pre_a->next=p;
                    pre_a=pre_a->next;
                    p=p->next;
                }
                deletePoly(pb,q);   //删除pb中的无用节点
                break;
            case -1:    //pa指数 < pb指数，修改指针pre_a、p
                pre_a->next=p;
                pre_a=pre_a->next;
                p=p->next;
                break;
            case 1:     //pa指数 > pb指数，修改指针pre_a、pre_b、q
                pre_a->next=q;
                pre_a=pre_a->next;
                pre_b->next=q->next;
                q=q->next;
                break;
        }
    }
    //处理剩余链
    if(p!=NULL){
        pre_a->next=p;
    }
    if (q!=NULL){
        pre_a->next=q;
    }
    //释放pb
    free(pb);
    pb=NULL;
}

//多项式减法，用加法实现
void subPoly(Polynomial &pa,Polynomial &pb){
    Polynomial p=pb->next;
    while (p!=NULL){
        p->coefficient*=-1.0;
        p=p->next;
    }
    addPoly(pa,pb);
}

//打印多项式
void printPoly(Polynomial p){
    Polynomial temp=p->next;
    while (temp!=NULL){
        cout<<temp->coefficient<<","<<temp->exponent<<"  ";
        temp=temp->next;
    }
}

int main() {

    Polynomial p1,p2;
    createPoly(p1);
    createPoly(p2);
//    addPoly(p1,p2);
    subPoly(p1,p2);
    printPoly(p1);

    return 0;
}
