> What You Must Know In C++

### 常量指针和指针常量
```c++
#include <iostream>
using namespace std;
int main(){
    int a = 1,b = 2;
    const int * p; 
    p = &a;
    // *p = *p + 1;
    // 报read-only错误，p为常量指针，不能通过p来改变a的值(可以通过其他方式改变, 如下), 常量指针也可写为int const* p;
    a = 3;
    cout << (*p) << endl;
    int* const q = &a;
    // q为指针常量，q的地址不改变，内容可以改变, 即指针常量 = 常指针
    *q = *q + 1;
    // q = &b;
    // 报read-only错误，指针常量不能再次赋值
    cout << (*q) << endl;
    return 0;
}
```


### 什么是智能指针以及shared_ptr实现原理
参考[智能指针](https://www.cnblogs.com/wxquare/p/4759020.html)  
我们先看下列代码
```c++
Object* obj = new Object();
dosomething();
delete obj;
```
上述代码很容易理解，假如程序员忘记写```delete```语句或者```dosomething```时出现错误，则```obj```所指的对象便不会被安全删除，使用智能指针可以很大程度避免这个问题：智能指针是一个类，当智能指针超出类的作用域时，类会自动调用析构函数，析构函数就会自动释放资源。

智能指针在普通指针的基础上增加了对指针的引用计数，作为一个类，在栈上分配资源，离开作用域时析构
代码中有两点需要注意
+ 引用计数应该是一个指针```size_t *count```,而不是```size_t count```,因为假如以下语句   
```c++
SmartPointer<int> sp1(new int(5));
SmartPointer<int> sp2(sp1);
```
创建了两个对象，两个对象的引用计数```count```相同，但是当其中一个析构时，另一个对象的引用计数无法改变，离开作用域时便无法析构，所以这里使用指针，通过对同一地址的访问来判断

+ 赋值时如```sp1 = sp2```，需要判断```sp1```的引用计数，如果是最后一个持有者，应当释放资源，
