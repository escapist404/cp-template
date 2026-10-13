# DynamicSegmentTree

维护超大离散坐标范围上的区间信息，支持按需创建节点、区间修改、区间查询和版本或树的合并。

```cpp
template<class V, auto op, auto e, class Index = long long>
class DynamicSegmentTree;
```

`op/e` 与静态线段树相同。构造 `DynamicSegmentTree<...> seg(low, high)`，根编号由调用者初始化为 `0`。接口使用半开 `[l,r)`：`set(root,p,x)`、`add(root,p,x)`、`prod(root,l,r)`、`get(root,p)`、`all_prod(root)`、`max_right(root,l,f)`、`min_left(root,r,f)`。边界二分返回 `Index`。`merge` 把 `b` 合入 `a` 并清零 `b`，要求 `op` 可交换。

```cpp
long long add(long long a, long long b) { return a + b; }
long long zero() { return 0; }
DynamicSegmentTree<long long, add, zero> seg(-1000000000LL, 1000000001LL);
int root = 0;
seg.add(root, 5, 3);
```

未创建节点的值为 `e()`；空间为实际修改点数乘坐标树高。
