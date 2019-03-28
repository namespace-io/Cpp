### 事务
一个数据库的操作序列，要么全做，要么全不做
ACID
+ Atomic原子性：事务作为整体被执行，要么全做，要么全不做
+ Consistency一致性：事务确保从一个一致性状态转变到另一个一致性状态，一致性含义是数据库中的数据满足完整性约束
+ Isolation隔离性：多个事务并发时，一个事务的执行不影响其他事物的执行
+ Durability持久性：事务一旦提交，对数据库的修改应该永久保存在数据库中

四种隔离级别
[四种隔离级别](https://blog.csdn.net/u011296485/article/details/77675781)
+ Read UnCommitted
  读（到）未提交（的数据）（丢失更新）
  限制同一数据写时禁止其他写->排他写锁，写时加X锁禁止其他写，保证数据可以恢复
  避免丢失更新
+ Read Committed(Oracle)
  提交以后的数据才能被读取
  写时禁止其他读写->排他写锁，瞬间共享读锁
  避免丢失更新，脏读
+ Repeatable Read（MySQL）
  限制同一数据写事务禁止其他读写事务，读事务禁止其它写事务(允许读)
  避免丢失更新，脏读，不可重复读
  没有解决幻读，解决幻读应该增加行级锁
+ Serializable：事务序列化执行

### 乐观锁和悲观锁及其实现
  
### MySQL索引及其实现
索引是高效获取数据的数据结构
[MySQL索引背后的数据结构及算法原理](http://blog.codinglabs.org/articles/theory-of-mysql-index.html)
#### 为什么数据库不用二叉查找树或者红黑树作为索引
一般来说，索引本身也很大，不可能全部存储在内存中，因此索引往往以索引文件的形式存储的磁盘上，索引的结构组织要尽量减少查找过程中磁盘I/O的存取次数
过一般使用磁盘I/O次数评价索引结构的优劣，根据B-Tree的定义，可知检索一次最多需要访问h个节点。数据库系统的设计者巧妙利用了磁盘预读原理，将一个节点的大小设为等于一个页，这样每个节点只需要一次I/O就可以完全载入，B-Tree中一次检索最多需要h-1次I/O（根节点常驻内存），渐进复杂度为O(h)=O(logdN)。一般实际应用中，出度d是非常大的数字，通常超过100，因此h非常小（通常不超过3）
红黑树这种结构，h明显要深的多

MySQL就普遍使用B+Tree实现其索引结构
InnoDB也使用B+Tree作为索引结构，InnoDB的数据文件本身就是索引文件，InnoDB的辅助索引data域存储相应记录主键的值而不是地址，知道了InnoDB的索引实现后，就很容易明白为什么不建议使用过长的字段作为主键，因为所有辅助索引都引用主索引，过长的主索引会令辅助索引变得过大

MySQL的优化主要分为结构优化（Scheme optimization）和查询优化（Query optimization）
### join,union
```sql
// T1: A B C T2: A D E
SELECT * FROM Table1 JOIN Table2 ON Table1.ColumnA=Table2.columnA // A B C E D
```
UNION ALL 允许重复的值
```sql
SELECT * FROM Table3 UNION SELECT *FROM Table4
```

### NoSQL（Not Only SQL）中MongoDB, redis
[数据库区别](https://blog.csdn.net/CatStarXcode/article/details/79513425)
非关系型数据库
+ MongoDB
  + 对数据的操作大部分都在内存中，但 MongoDB 并不是单纯的内存数据库
  + MongoDB 的所有数据实际上是存放在硬盘的，所有要操作的数据通过 mmap 的方式映射到内存某个区域内。
  + 优点：内置分片，弱一致性，文档结构存储方式json->bson，内存快速
  + 缺点：不支持事务操作（最主要缺点）
+ Redis
  + 内存数据库
  + Redis所有数据都是放在内存中的，持久化是使用RDB方式或者aof方式
+ MySQL
  + 无论数据还是索引都存放在硬盘中。到要使用的时候才交换到内存中。能够处理远超过内存总量的数据。
  + 关系型数据库
  + 缺点：在海量数据处理的时候效率会显著变慢