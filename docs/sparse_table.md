# Sparse Table & Disjoint Sparse Table (`sparse_table.hpp`)

`sparse_table.hpp` 同时提供处理**幂等结合运算**（如 RMQ、`gcd`、按位与/或）的静态区间表 `SparseTable<T, Op>`，以及处理**任意结合律运算**（无需幂等性或交换律，如区间乘积、矩阵乘法、仿射复合）的猫树静态表 `DisjointSparseTable<T, Op>`。依赖 [`bit.hpp`](bit.md)。

## 接口说明

### 模板参数与构造函数

```cpp
template <typename T>
struct SparseTableMax {
    T operator()(T a, T b) { return std::max(a, b); }
};

template <typename T, typename Op = SparseTableMax<T>>
class SparseTable {
public:
    SparseTable(std::vector<T> a, Op op = Op());
    T query(int l, int r);
};

template <typename T, typename Op = SparseTableMax<T>>
class DisjointSparseTable {
public:
    DisjointSparseTable(std::vector<T> a, Op op = Op());
    T query(int l, int r);
};
```

- **默认合并算子**：两类表的 `Op` 默认均为 `SparseTableMax<T>`（求区间最大值）。若需自定义运算（如最小值 `std::min` 或最大公约数 `std::gcd`），可在构造时传入函数对象或 Lambda，并将 `Op` 设为对应类型。
- **运算性质要求**：
  - `SparseTable`：每次查询将区间覆盖为两个可能重叠的 $2^k$ 长度子区间 `op(table[k][l], table[k][r - (1 << k)])`，因此要求 `op` 同时满足**结合律**与**幂等律**（`op(x, x) == x`），**绝不能用于区间求和或乘积**。
  - `DisjointSparseTable`：按分治中点预处理前后缀并严格做**不重叠拼接** `op(table[k][l], table[k][r])`，因此只需 `op` 满足**结合律**（**不要求幂等律，也不要求交换律**，严格保持从左到右的合并顺序）。
- **查询接口与非空约束**：
  - `query(int l, int r)`：查询 **`0-based` 左闭右开区间 `[l, r)`** 的聚合结果。由于两类表均不设单位元，**查询区间必须非空，严格要求 $0 \le l < r \le n$**（传入空区间 `l == r` 会触发 `assert`）。

## 复杂度

- 构造：时间与空间复杂度均为 $O(n \log n)$（假设单次 `op` 为 $O(1)$）。
- 单次 `query(l, r)`：严格 $O(1)$ 次 `op` 调用（内部使用 `__builtin_clz` 在 $O(1)$ 时间内定位层高）。

## 使用示例

```cpp
#include "sparse_table.hpp"
#include <cassert>
#include <functional>
#include <vector>

int main() {
    std::vector<int> a = {3, 1, 4, 1, 5, 9, 2};

    // 默认求区间最大值
    SparseTable<int> st_max(a);
    assert(st_max.query(1, 5) == 4); // max{1, 4, 1, 5} = 5 -> [1, 5) 为 {1, 4, 1} -> 4
    assert(st_max.query(1, 6) == 5);

    // 传入自定义最小算子
    auto min_op = [](int x, int y) { return x < y ? x : y; };
    SparseTable<int, decltype(min_op)> st_min(a, min_op);
    assert(st_min.query(0, 4) == 1);

    // DisjointSparseTable 支持非幂等运算（如区间求和）
    DisjointSparseTable<int, std::plus<int>> dst_sum(a);
    assert(dst_sum.query(1, 5) == 1 + 4 + 1 + 5);
    return 0;
}
```

## 相关文档

- [SegmentTree](segtree.md) —— 支持单点修改的 $O(\log n)$ 区间半群查询
- [Bit Utilities](bit.md) —— 提供建表所需的 `bit_width` 与 `ceil_log2`
