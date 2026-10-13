# SparseTable / DisjointSparseTable

同一个 `sparse_table.hpp` 提供两种静态区间表。下标为 0-based，查询区间为非空的 `[l,r)`。

## 接口

```cpp
template <typename T, typename Op = SparseTableMax<T>>
class SparseTable;
template <typename T, typename Op = SparseTableMax<T>>
class DisjointSparseTable;

SparseTable(std::vector<T> a, Op op = Op());
DisjointSparseTable(std::vector<T> a, Op op = Op());
T query(int l, int r);
```

`Op(a,b)` 返回两个区间的合并结果，默认 `SparseTableMax<T>` 求最大值。已有 `SparseTable<int> st(a)` 的用法不变。

- `SparseTable`：运算必须满足结合律和幂等性，例如 `max`、`min`、`gcd`。重叠区间会被合并，不能用求和。
- `DisjointSparseTable`：只要求结合律，支持求和、矩阵乘法、字符串拼接。保留从左到右的合并顺序，不要求交换律或幂等性。
- `T` 需能复制、赋值；支持空数组构造，但空数组不能查询。
- 查询必须满足 `0 <= l < r <= n`；两种表均不支持修改。

## 复杂度

建表时间和空间均为 $O(n\log n)$，每次查询调用一次合并操作。若合并为 $O(1)$，查询为 $O(1)$；字符串等变长对象还需计入复制和合并成本。

## 使用示例

```cpp
#include "sparse_table.hpp"
#include <cassert>
#include <functional>

int main() {
    SparseTable<int> mx({3, 1, 4, 2});
    assert(mx.query(1, 4) == 4);
    auto minimum = [](int a, int b) { return std::min(a, b); };
    SparseTable<int, decltype(minimum)> mn({3, 1, 4, 2}, minimum);
    assert(mn.query(1, 4) == 1);
    DisjointSparseTable<int, std::plus<int>> sum({3, 1, 4, 2});
    assert(sum.query(1, 4) == 7);
}
```

## 前置条件与实现约定

- 不要把 `std::less<T>` 当作最小值合并：它返回的是布尔值。应传入返回 `std::min(a,b)` 的函数。
- 不支持空区间，也不需要单位元。
- 合并函数应当对相同输入产生相同输出；查询期间不能改变其语义。

## 相关文档

- [SegmentTree](segtree.md) —— 需要修改时使用
- [WaveletTree](wavelet_tree.md) —— 静态区间第 k 小
