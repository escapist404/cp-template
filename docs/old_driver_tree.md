# OldDriverTree

维护由若干半开区间组成的常值段，支持把区间整体赋值和查询单点值。区间为半开 `[l,r)`。

```cpp
template<class T>
class OldDriverTree;
```

构造函数接收初始数组：

```cpp
OldDriverTree<int> tree({1, 1, 2, 2});
```

接口只有三个：

```cpp
void set(int l, int r, T value); // 将 [l,r) 推平
T get(int p);                    // 查询单点
```

`set` 会删除覆盖范围内的旧段；`get` 返回位置 `p` 所在段的值。ODT 依赖操作序列的随机性，最坏情况下可能退化，不适合替代普通懒线段树。

```cpp
OldDriverTree<int> tree({0, 0, 0, 0, 0});
tree.set(1, 4, 7);
assert(tree.get(0) == 0);
assert(tree.get(1) == 7);
assert(tree.get(3) == 7);
```
