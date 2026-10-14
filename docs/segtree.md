# Segment Tree (`segtree.hpp`)

`segtree.hpp` 采用 AtCoder Library (ACL) 风格的非递归（ZKW）线段树设计，维护长度为 $n$ 的静态数组，支持单点修改、半开区间幺半群（Monoid）聚合查询以及树上 $O(\log n)$ 边界二分。依赖 [`bit.hpp`](bit.md)。

## 接口说明

### 类模板与构造函数

```cpp
template <class V, auto op, auto e>
class SegmentTree {
public:
    SegmentTree();
    SegmentTree(int n);
    SegmentTree(std::vector<V> v);
};
```

- **非类型模板参数（要求 C++17 及以上）**：
  - `V`：节点维护的聚合值类型；
  - `op`：函数指针或无捕获函数 `V op(V a, V b)`，要求满足**结合律** $(a \cdot b) \cdot c = a \cdot (b \cdot c)$，**不要求满足交换律**（查询时严格保持从左到右的合并顺序）；
  - `e`：函数指针 `V e()`，返回关于 `op` 的双边单位元（幺元），满足 `op(e(), x) == x` 且 `op(x, e()) == x`。
- `SegmentTree()`：构造 $n = 0$ 的空线段树。
- `SegmentTree(int n)`：构造长度为 $n$、初始每个位置均为 `e()` 的线段树。
- `SegmentTree(std::vector<V> v)`：用给定数组 `v` 在 $O(n)$ 时间内自底向上建树。

### 修改、查询与树上二分

所有下标均为 **`0-based`**，所有区间均为**左闭右开 `[l, r)`**：

```cpp
void set(int p, V x);
V get(int p);
V prod(int l, int r);
V all_prod();
template <class F> int max_right(int l, F f);
template <class F> int min_left(int r, F f);
```

- `set(int p, V x)`：将下标 `p` 处的值直接设为 `x`（要求 $0 \le p < n$），并自底向上更新祖先节点。
- `get(int p)`：返回下标 `p` 处的当前元素值（要求 $0 \le p < n$）。
- `prod(int l, int r)`：返回区间 `[l, r)` 按从左到右顺序合并的结果 $v[l] \cdot v[l+1] \cdots v[r-1]$（要求 $0 \le l \le r \le n$）；当 `l == r` 时返回 `e()`。
- `all_prod()`：返回整个数组 `[0, n)` 的聚合结果，等同于 `prod(0, n)` 但耗时 $O(1)$。
- `max_right(int l, F f)`：固定左端点 $l$（要求 $0 \le l \le n$），在线段树上二分查找最大的右端点 $r \in [l, n]$ 使得 `f(prod(l, r))` 为 `true`。**前置条件**：布尔谓词 `f` 必须满足 **`f(e()) == true`**（否则触发 `assert`），且对于固定的 $l$，`f(prod(l, r))` 关于 $r$ 单调（即由 `true` 变为 `false` 后不再变回 `true`）。返回满足条件的 $r$ 满足 `f(prod(l, r)) == true` 且（$r == n$ 或 `f(prod(l, r + 1)) == false`）。
- `min_left(int r, F f)`：固定右端点 $r$（要求 $0 \le r \le n$），在线段树上二分查找最小的左端点 $l \in [0, r]$ 使得 `f(prod(l, r))` 为 `true`。同样要求 **`f(e()) == true`** 且 `f(prod(l, r))` 关于 $l$ 单调。

## 复杂度

- 构造：时间复杂度 $O(n)$，空间复杂度 $O(2^{\lceil \log_2 n \rceil + 1}) = O(n)$。
- `get`、`all_prod`：$O(1)$。
- `set`、`prod`、`max_right`、`min_left`：$O(\log n)$。

## 使用示例

```cpp
#include "segtree.hpp"
#include <cassert>
#include <vector>

long long op(long long a, long long b) { return a + b; }
long long e() { return 0LL; }

int main() {
    SegmentTree<long long, op, e> seg(std::vector<long long>{2, 1, 4, 3, 5});
    assert(seg.prod(1, 4) == 8);
    assert(seg.all_prod() == 15);

    seg.set(2, 7); // {2, 1, 7, 3, 5}
    assert(seg.get(2) == 7);
    assert(seg.prod(1, 4) == 11);

    // 寻找从 l = 1 开始区间和 <= 10 的最大右端点 r:
    // prod(1, 3) = 1 + 7 = 8 <= 10, prod(1, 4) = 1 + 7 + 3 = 11 > 10 -> 返回 3
    int r = seg.max_right(1, [](long long sum) { return sum <= 10; });
    assert(r == 3);
    return 0;
}
```

## 相关文档

- [LazySegmentTree](lazy_segtree.md) —— 支持区间懒标记修改的线段树
- [DynamicSegmentTree](dynamic_segtree.md) —— 大值域动态开点与线段树合并
- [PersistentSegmentTree](persistent_segtree.md) —— 可持久化线段树（主席树）
