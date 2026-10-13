# LazySegmentTree

维护一个可区间修改、可区间聚合查询的数组；区间信息和懒标记都由调用者定义。

```cpp
template<class V, auto op, auto e, class L,
         auto mapping, auto composition, auto id>
class LazySegmentTree;
```

必须提供：`op(V,V)->V`、`e()->V`、`mapping(L,V)->V`、`composition(L,L)->L`、`id()->L`。`composition(f,g)` 表示先应用 `g`，再应用 `f`。所有区间使用半开 `[l,r)`。

```cpp
struct S { long long sum; int len; };
struct F { long long a, b; };
S op(S x, S y) { return {x.sum + y.sum, x.len + y.len}; }
S e() { return {0, 0}; }
S mapping(F f, S x) { return {f.a * x.sum + f.b * x.len, x.len}; }
F composition(F f, F g) { return {f.a * g.a, f.a * g.b + f.b}; }
F id() { return {1, 0}; }
LazySegmentTree<S, op, e, F, mapping, composition, id> seg(init);
```

接口：`set`、`get`、`prod`、`all_prod`、`apply`、`max_right`、`min_left`。`V` 应携带区间长度等 `mapping` 所需信息。

复杂度：构造 `O(n)`，单点操作、区间查询和区间修改 `O(log n)`，边界二分 `O(log n)`。
