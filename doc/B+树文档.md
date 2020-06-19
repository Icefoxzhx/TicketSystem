# B+树文档

### 原型:
```cpp
template<class Key,class Value,class Compare=std::less<Key> >
class Bptree{};
```
实例化时需依次提供参数：键值类型Key, 值类型Value。

**请确保值类型Value有空构造函数**

- 构造函数:
```cpp
Bptree(const char* _filename)
```
提供存储B+树的文件名_filename

- 析构函数:
```cpp
~Bptree()
```

- 检查B+树是否为空
```cpp
bool empty()
```

- 查找键值为key的数据
```cpp
pair<bool,Value> find(const Key &key)
```
找不到返回false

- 查找键值在[k1,k2)内的数据
```cpp
vector<pair<Key,Value>> find(const Key &k1, const Key &k2)
```
不存在返回空vector

- 检查是否存在键值为key的数据
```cpp
bool count(const Key &key)
```

- 插入数据(key,val)
```cpp
bool insert(const Key &key,const Value &val)
```
如果键值key已存在，什么也不做，返回false，否则返回true

- 删除键值为key的数据
```cpp
bool remove(const Key &key)
```
如果键值key不存在，什么也不做，返回false，否则返回true

- 将键值为key的数据的值改为val
```cpp
bool set(const Key &key,const Value &val)
```
如果键值key不存在，什么也不做，返回false，否则返回true

- 清空B+树
```cpp
void clear()
```

- 输出B+树信息
```cpp
void print()
```