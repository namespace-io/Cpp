### TCP提供有确认，有连接的数据流服务，UDP提供无确认，无连接的数据报服务
当数据从发送方传送到接受方时，TCP保证接收方接受的是顺序的无丢失的包，而由于UDP不关心接受方是否接受到，每次只能读取一个报文的数据  
也就是TCP中发送方和接受方的write,read次数可以不对等，但是UDP中write,read的次数必须对等

### TCP三次握手四次挥手
####  三次握手
![](img/NetWork/tcp-connection-made-three-way-handshake.png)
```
Client ------(SYN + Seq = x)-----> Server
Client <-----(SYN + Seq = y + ACK = x+1)----- Server
Client ------(Seq = x+1 + ACK = y+1)-----> Server
```
#### 四次挥手
![](img/NetWork/tcp-connection-closed-four-way-handshake.png)
#### 为什么需要三次握手
信道是不可靠的
两次握手就确认的话，会使得信道中client发送的已经超时或失效报文被server接受，而如果此时client根本没有响应，server就会浪费很多资源
1,2次握手可以保证客户端能够被服务器正确响应  
2,3次握手可以保证服务器能够被客户端正确响应  

### SYN攻击
2次握手时，称为半连接
攻击方伪造大量的IP地址向服务器不断发SYN包，使得服务器维护大量半连接浪费资源，正常的SYN被丢弃，引起网络堵塞或系统瘫痪，SYN攻击是一种典型的DoS/DDoS攻击

#### 如何检测SYN攻击
检测SYN攻击非常的方便，当在服务器上看到大量的半连接状态时，特别是源IP地址是随机的，基本上可以断定这是一次SYN攻击。在Linux/Unix上可以使用系统自带的netstats命令来检测SYN攻击。


#### 如何防御SYN攻击
SYN攻击不能完全被阻止，除非将TCP协议重新设计。我们所做的是尽可能的减轻SYN攻击的危害，常见的防御SYN攻击的方法有如下几种：
+ 缩短超时（SYN Timeout）时间
+ 增加最大半连接数
+ 过滤网关防护
+ SYN cookies技术