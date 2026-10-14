# Lazy Segment Tree (`lazy_segtree.hpp`)

`lazy_segtree.hpp` 实现 AtCoder Library (ACL) 风格的非递归懒标记线段树 `LazySegmentTree`，支持单点修改、区间作用懒标记、半开区间聚合查询以及树上 $O(\log n)$ 边界二分。依赖 [`bit.hpp`](bit.md)。

## 接口说明

### 类模板与代数公理

```cpp
template <class V, auto op, auto e, class L, auto mapping, auto composition, auto id>
class LazySegmentTree {
public:
    LazySegmentTree();
    LazySegmentTree(int n);
    LazySegmentTree(std::vector<V> v);
};
```

- **模板参数与运算契约**：
  - `V`：数据幺半群元素类型；`V op(V a, V b)` 为满足结合律的区间合并函数；`V e()` 返回数据单位元。
  - `L`：作用算子（懒标记）幺半群类型；`L id()` 返回恒等标记（满足 `mapping(id(), x) == x`）。
  - `V mapping(L f, V x)`：将标记 `f` 作用在聚合值为 `x` 的区间上并返回更新后的聚合值。**由于 `mapping` 只接收 `(L f, V x)` 而不额外传入区间长度，若区间修改与长度相关（例如区间加 $b$ 对区间和贡献 $b \times \text{len}$），必须在结构体 `V` 内部显式维护区间长度字段 `len`**。
  - `L composition(L f, L g)`：返回两个标记复合后的新标记，**严格定义为“在已有标记 `g` 的基础上，再新施加一个标记 `f`”**，即满足 `mapping(composition(f, g), x) == mapping(f, mapping(g, x))`（左乘复合顺序，切勿将 `f` 和 `g` 的先后次序写反）。
  - 同态分配律：对于任意标记 `f` 和区间值 `x, y`，需满足 `mapping(f, op(x, y)) == op(mapping(f, x), mapping(f, y))`。

### 成员函数

所有下标均为 **`0-based`**，所有区间均为**左闭右开 `[l, r)`**：

```cpp
void set(int p, V x);
V get(int p);
V prod(int l, int r);
V all_prod();
void apply(int p, L f);
void apply(int l, int r, L f);
template <class G> int max_right(int l, G g);
template <class G> int min_left(int r, G g);
```

- `set(int p, V x)` / `get(int p)`：下推祖先路径上的懒标记后，设置或读取下标 `p`（$0 \le p < n$）的值。
- `prod(int l, int r)` / `all_prod()`：查询区间 `[l, r)`（$0 \le l \le r \le n$）或全区间 `[0, n)` 的聚合结果。
- `apply(int p, L f)` / `apply(int l, int r, L f)`：对单点 `p` 或区间 `[l, r)` 施加操作标记 `f`。
- `max_right(int l, G g)` / `min_left(int r, G g)`：与 [`SegmentTree`](segtree.md) 相同，在自动下推标记的同时二分查找满足 `g(prod(l, r)) == true` 的最远右端点 $r$ 或最远左端点 $l$，同样要求 **`g(e()) == true`** 且谓词单调。

## 复杂度

- 构造：时间复杂度 $O(n)$，空间复杂度 $O(n)$。
- `all_prod`：$O(1)$。
- `set`、`get`、`prod`、`apply`、`max_right`、`min_left`：$O(\log n)$。

## 使用示例

下面展示维护**区间仿射变换 $x \leftarrow ax + b$ 与区间求和**的标准写法：

```cpp
#include "lazy_segtree.hpp"
#include <cassert>
#include <vector>

struct S {
    long long sum;
    int len;
};
struct F {
    long long mul, add;
};

S op(S a, S b) { return {a.sum + b.sum, a.len + b.len}; }
S e() { return {0, 0}; }
S mapping(F f, S x) { return {f.mul * x.sum + f.add * x.len, x.len}; }
// 先执行旧标记 g: x' = g.mul * x + g.add，再执行新标记 f: f.mul * x' + f.add
F composition(F f, F g) { return {f.mul * g.mul, f.mul * g.add + f.add}; }
F id() { return {1, 0}; }

int main() {
    std::vector<S> init = {{1, 1}, {2, 1}, {3, 1}, {4, 1}};
    LazySegmentTree<S, op, e, F, mapping, composition, id> seg(init);

    seg.apply(1, 3, F{2, 1}); // [1, 3) 乘 2 加 1 -> {1, 5, 7, 4}
    assert(seg.prod(0, 4).sum == 17);
    assert(seg.get(2).sum == 7);
    return 0;
}
```

## 相关文档

- [SegmentTree](segtree.md) —— 仅需单点修改的轻量线段树
- [SegmentTreeBeats](segment_tree_beats.md) —— 支持区间取 `min` / `max` 与区间加的吉司机线段树
