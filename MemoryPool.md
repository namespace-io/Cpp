### new, operator new, placement new
[new 表达式 Cppreference](https://zh.cppreference.com/w/cpp/language/new)
+ new 是操作符不能被重载
  + 调用operator new 分配内存
  + 调用构造函数构造对象
  + 返回指针
+ operator new是函数，可以被重载，但不能重载对全局的void operator new(size_t size)这个原型进行重载，  
  如果类中没有重载，则调用全局的::operator new来完成分配
+ placement new是operator new的重载版本，用于在已分配的内存上重新构造对象，返回首地址  
  需要反复创建并删除的对象上，可以降低分配释放内存的性能消耗
```c++
  #include <iostream>
using namespace std;

struct Plus
{
    Plus(){ k = 1;}
    Plus(int x){ k = x;}
    void operator()(){ cout << "k = " << k << ", Plus Called\n";} 
    int k;
};

int main(){
    Plus p;
    char* buf = new char[sizeof(p)];
    new (buf) Plus(5);
    Plus* k = (Plus*)buf;
    (*k)();
    k->~Plus(); // 对象销毁必须显式调用析构函数，此时内存还未被回收，k的地址与buf的地址相同
    delete[] buf; // 回收内存
    return 0;
}
```

### 简单内存池实现原理

#### 目的
避免多次重复分配释放内存
#### 数据结构

内存池由多个内存块组成，一个内存块中由多个槽，一个槽存放一个对象  
```c++
union Slot
    {
        T e;
        Slot *next;
    };
```
当slot可用时，可以用其中的next指针，分配后只保存元素e, 避免了用额外区域存储指针造成空间的浪费  
我们需要维护四个变量  
```c++
    Slot* cur_block;
    Slot* free_slot;
    Slot* cur_slot;
    Slot* last_slot;
```
其中cur_block代表当前内存块， 也就是链表中的第一个块，当申请新的内存块时，采用头插法讲新的内存块插入链表头部    
cur_slot, last_slot分别表示cur_block中第一个可分配的槽的位置和距离内存块底部一个槽的位置，  
当cur_slot>=last_slot时，表示内存块中剩余空间不够分配一个槽，应该重新申请内存块  

free_slot为可用槽的链表头， 不为空则可以分配

### 实现
+ 分配时，优先查看free_slot是否有可用的槽，  
    + 如果```free_slot == nullptr```表示无可用的槽，则去当前块中查找，分配cur_slot或者不够分配时申请一个新的内存块，并重置cur_slot等变量
    + 否则，分配free_slot，并让free_slot只想free_slot->next

+ 回收时，直接讲需要回收的地址p加入到free_slot链表的头部，同样也是采用头插法

  
使用placement new 来根据分配的内存初始化对象

### 参考
+ [C++ 实现高性能内存池](https://www.jianshu.com/p/18c7e8a900a5)
+ [memory-allocators](https://github.com/mtrebi/memory-allocators#introduction)
+ [Memory Pool](https://github.com/cacay/MemoryPool/blob/master/C-11/MemoryPool.h)
+ [tiny MP](https://github.com/double-free/tiny-projects/blob/master/memory-pool/MemoryPool.h)
+ [经典的内存池技术](http://cplusplus.wikidot.com/cn:dive-into-memory-pool#toc1)
+ [C++ 应用程序性能优化，第 6 章：内存池](https://www.ibm.com/developerworks/cn/linux/l-cn-ppp/index6.html)