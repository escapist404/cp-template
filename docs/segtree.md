# SegmentTree

维护一个静态长度的数组，支持单点修改、区间聚合和按谓词寻找边界；区间聚合类型为满足结合律的 `V`。

```cpp
template<class V, auto op, auto e>
class SegmentTree;
```

`op(a,b)` 合并两个区间，`e()` 返回幺元；查询区间为半开 `[l,r)`。`op` 可以是非交换的，查询会保持从左到右的顺序。

```cpp
long long op(long long a, long long b) { return a + b; }
long long e() { return 0; }
SegmentTree<long long, op, e> seg(vector<long long>{1, 2, 3});
seg.set(1, 8);
auto x = seg.prod(0, 3);
auto y = seg.all_prod();
int r = seg.max_right(0, [](long long x) { return x <= 10; });
int l = seg.min_left(3, [](long long x) { return x <= 10; });
```

接口：`set`、`get`、`prod`、`all_prod`、`max_right`、`min_left`。`max_right/min_left` 要求谓词满足 ACL 的单调性，并且 `f(e())` 为真。

复杂度：构造 `O(n)`，单点修改和区间查询 `O(log n)`，边界二分 `O(log n)`。
