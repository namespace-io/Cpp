### 多线程同步机制：信号量，互斥量，读写锁，条件变量
互斥量mutex控制对共享资源的访问,C++一般使用RAII(Resource Acquisition Is Initialization)自动释放锁(如lock_guard)
+ 有了互斥锁为什么要条件变量  
[有了互斥锁为什么要条件变量](https://www.jianshu.com/p/01ad36b91d39)  
假定线程A一直增加count,线程B判断count大于100时将count置为0,如果仅仅用锁我们表示为  
```
global_var a = 0

threadA: // add 1
while(True)
  lock(&mutex)
  count++
  unlock(&mutex)

threadB: // count > 100 => reset count = 0
while(True)
  lock(&mutex)
  if(count > 100) count = 0
  unlock(&mutex)
```

这样我们可以知道线程B大部分时间获得了锁mutex根本不做任何事，白白浪费了CPU，给系统带来了不必要的开销。 
如果有方法能够让线程B一直Block**直到条件满足**再唤醒线程B就好了，这样也不用重复加锁解锁，这时候需要使用**条件变量**
+ 条件变量为什么要和互斥锁一起使用  
[calling-pthread-cond-signal-without-locking-mutex](https://stackoverflow.com/questions/4544234/calling-pthread-cond-signal-without-locking-mutex)

```c++
threadA:
  pthread_mutex_lock(&mutex);
  while(condition == false)
     pthread_cond_wait(&cond, &mutex);
  pthread_mutex_unlock(&mutex);

threadB:
  condition = TRUE;
  pthread_cond_signal(&cond)
```
如上条件变量不和互斥锁一起使用就有可能导致threadB先执行，而threadA没有执行都wait导致信号丢失  
比如下面的代码执行就会使得threadA被永远挂起，一直等待，**根本原因是线程A对条件的检查和进入等待的中间是有空档的**  
```
threadA                               threadB

pthread_mutex_lock(&mutex);
while (condition == FALSE)

                                      condition = TRUE;
                                      pthread_cond_signal(&cond);

pthread_cond_wait(&cond, &mutex);
```
正确应该写为
```
threadB:
pthread_mutex_lock(&mutex);
condition = TRUE;
pthread_cond_signal(&cond);
pthread_mutex_unlock(&mutex)
```
值得注意的是可能存在spurious wakeup虚假唤醒，线程A中必须循环判断条件是否满足，因为即使正确的线程B修改了条件为真并释放了锁，其他线程如C也可能修改条件使得条件不满足，当然还有其他原因，   
一个生产者消费者例子
```
threadA:
  if(queue.empty())
    pthread_cond_wait(&cond, &mutex);

threadA:
  while(queue.empty())
    pthread_cond_wait(&cond, &mutex);
```
假定两个线程AB为消费者，一个线程C为生产者，当队列为空时，AB进入阻塞等待状态，  
如果此时C产生了一个元素，此时AB都被唤醒，则必然有一个得到元素，另一个重新进入等待，如果用if，则不能重新判断条件所以用while
```
A,B wait
                             C notify A, B and release lock  
A wake up 
B wake up
A get lock 
A process
A release lock
B get lock but get nothing and continue process // error, should continue wait, this is spurious wakeup
```

### 生产者消费者问题  
[生产者消费者问题](https://zh.wikipedia.org/wiki/%E7%94%9F%E4%BA%A7%E8%80%85%E6%B6%88%E8%B4%B9%E8%80%85%E9%97%AE%E9%A2%98)
生产者消费者同时对资源进行写
```
empty = n
full  = 0
mutex = 1


producer:
  P(empty) //为了生产者和消费者同步
  P(mutex) // 防止多个生产者同时生产
  produce()
  V(mutex)
  V(full)  

consumer:
  P(full)
  P(mutex) // 防止多个消费者同时消费
  consume()
  V(mutex)
  V(empty)  
```
(这里我个人觉得应该分别设置生产者和消费者两个互斥量)
#### 按照如上的代码，互斥锁是必须的吗
+ 生产者=1. 消费者=1, 缓冲区>1
+ 生产者=1, 消费者=1, 缓冲区=1   
  互斥锁mutex可以不要。一个非空的缓冲区等于一个满的缓冲区，当缓冲区有一个item时，生产者阻塞，缓冲区为空时，消费者阻塞，所以任意时刻只有一个线程访问缓冲区，可以保证互斥
+ 生产者>1,消费者>1, 缓冲区=1
  同上

### 读者写者问题  
[Readers–writers_problem](https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem)  
读者只读，写者只写  
#### 读者优先  
如下读进程存在时，写进程将被延迟，如果一直有读进程读，写进程可能被饿死  
```c++
int readcnt = 0
semaphore resource=1;  //用于保护更新count变量时的互斥
semaphore rmutex=1; // 读写互斥
reader:
  while(True)
    P(rmutex)
    readcnt ++;
    if(readcnt == 1) // 第一个读者
      P(resource) // 阻止写
    V(rmutex)

    reading()

    // finish reading
    P(rmutex)
    readcnt --;
    if(readcnt == 0) // 最后一个读者
      V(resource) // 允许写
    V(rmutex)

writer:
  while(True)
    P(resource)
    writing
    V(resource)
```
#### 写者优先
有写进程访问时，阻止后续读进程
```c++
int readcnt = 0, writecnt = 0;
semaphore resource=1; 
semaphore rmutex=1;  //用于保护更新count变量时的互斥
semaphore wmutex=1; // 读写互斥
semaphore readTry = 1;
reader:
  P(readTry)
  P(rmutex)
    readcnt ++;
    if(readcnt == 1) // 第一个读者
      P(resource) // 阻止写
  V(rmutex)
  V(readTry)
  
  reading()
  // finish reading
  P(rmutex)
  readcnt --;
  if(readcnt == 0) // 最后一个读者
    V(resource) // 允许写
  V(rmutex)

writer:
  P(wmutex)
  writecnt++
  if(write == 1) 
    P(readTry)
  V(wmutex)

  P(resource)
  writing
  V(resource)

  P(wmutex)
  writecnt--
  if(write == 0) 
    V(readTry)
  V(wmutex)
```