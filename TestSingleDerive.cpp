#include <iostream>
using namespace std;
class Base{
    
  public:
    virtual void f(){ cout << "Base f\n";}
    virtual void g(){ cout << "Base g\n";}
    int b = -1;
};
class Derive : public Base{
    public:
   
    virtual void g(){ cout << "Derive g\n";}
    virtual void h(){ cout << "Derive h\n";}
    int d = 1;
};

// 测试环境https://leetcode.com/playground/new/empty

int main() { 
    Derive d;
    typedef void(*Fun)(void);
    int** vptr = (int**)(&d);
    cout << sizeof(d) <<"\n";
    auto vtbl = *vptr;
    for(int i = 0; i < 3; ++i){
        Fun fun = reinterpret_cast<Fun>(*(vtbl + i * 2));
        fun();
    }
    for(int i = 0; i < 2; ++i)
        cout << *((int*)&d + 2 + i) << endl;
    return 0;
}