# Segment Tree Beats (`segment_tree_beats.hpp`)

`segment_tree_beats.hpp` 实现吉司机线段树（Segment Tree Beats）`SegmentTreeBeats<T, Sum>`，在 **`0-based` 左闭右开区间 `[l, r)`** 上同时支持区间取最小值（`chmin`）、区间取最大值（`chmax`）、区间加法（`add`）以及区间和、区间最小值与最大值查询。

## 接口说明

### 类模板与构造函数

```cpp
template <typename T = long long, typename Sum = T>
class SegmentTreeBeats {
public:
    SegmentTreeBeats(const std::vector<T>& a);
    SegmentTreeBeats(int n, T value = T());
};
```

- **类型参数与值域约束**：
  - `T`：元素值与最值标记类型，默认为 `long long`（必须是有符号算术类型）。内部使用 `std::numeric_limits<T>::lowest()` 和 `std::numeric_limits<T>::max()` 作为次大值 `max2` 和次小值 `min2` 的无穷哨兵，因此**数组中的实际元素值以及加减后的中间值必须严格落在 `(numeric_limits<T>::lowest(), numeric_limits<T>::max())` 开区间内部**，不可触碰类型极值。
  - `Sum`：区间和累加类型，默认与 `T` 相同。若 `T` 为 `long long` 且区间加后区间和可能超过 `long long` 范围，可将 `Sum` 指定为 `i128`（`__int128_t`）。
- `SegmentTreeBeats(const std::vector<T>& a)`：以数组 `a` 初始化长度为 $n = a.\text{size()}$ 的线段树（允许 $n = 0$）。
- `SegmentTreeBeats(int n, T value = T())`：构造长度为 $n$（要求 $n \ge 0$）、初值全为 `value` 的线段树。

### 修改与查询接口

所有区间 `[l, r)` 均为 **`0-based` 左闭右开区间**：

```cpp
void add(int l, int r, T value);
void chmin(int l, int r, T value);
void chmax(int l, int r, T value);
Sum sum(int l, int r);
T min(int l, int r);
T max(int l, int r);
```

- `add(l, r, value)`：对区间 $i \in [l, r)$ 执行 $a[i] \leftarrow a[i] + \text{value}$（要求 $0 \le l \le r \le n$；当 $l == r$ 时无操作）。
- `chmin(l, r, value)`：对区间 $i \in [l, r)$ 执行 $a[i] \leftarrow \min(a[i], \text{value})$（即将所有大于 `value` 的元素削平为 `value`，要求 $0 \le l \le r \le n$）。
- `chmax(l, r, value)`：对区间 $i \in [l, r)$ 执行 $a[i] \leftarrow \max(a[i], \text{value})$（即将所有小于 `value` 的元素抬高为 `value`，要求 $0 \le l \le r \le n$）。
- `sum(l, r)`：返回区间 $[l, r)$ 的元素和（要求 $0 \le l \le r \le n$；当 $l == r$ 时返回 `Sum()` 即 $0$）。
- `min(l, r)` / `max(l, r)`：返回区间 $[l, r)$ 的最小值或最大值。**注意查询最值时区间必须非空，严格要求 $0 \le l < r \le n$**（传入空区间 `l == r` 会触发 `assert`）。

## 复杂度

- 构造：时间复杂度 $O(n)$，空间复杂度 $O(n)$（分配 `4 * n` 个节点）。
- 仅含 `chmin` 与 `chmax` 时：单次操作均摊时间复杂度 $O(\log n)$。
- 同时混用 `add`、`chmin`、`chmax` 与查询时：单次操作均摊时间复杂度 $O(\log^2 n)$。

## 使用示例

```cpp
#include "segment_tree_beats.hpp"
#include <cassert>
#include <vector>

int main() {
    SegmentTreeBeats<long long> seg({1, 5, 3, 8, 2});

    seg.chmin(0, 5, 4); // {1, 4, 3, 4, 2}
    assert(seg.max(0, 5) == 4);
    assert(seg.sum(0, 5) == 14);

    seg.chmax(1, 4, 3); // [1, 4) 对 3 取 max -> {1, 4, 3, 4, 2}
    seg.add(0, 3, 2);   // [0, 3) 加 2 -> {3, 6, 5, 4, 2}
    assert(seg.min(0, 5) == 2);
    assert(seg.max(0, 5) == 6);
    assert(seg.sum(1, 4) == 15);
    return 0;
}
```

## 相关文档

- [LazySegmentTree](lazy_segtree.md) —— 通用懒标记线段树
- [SegmentTree](segtree.md) —— 基础单点修改线段树
