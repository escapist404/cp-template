# Wavelet Tree (`wavelet_tree.hpp`)

`wavelet_tree.hpp` 实现内置自动离散化的小波树（Wavelet Matrix 形式）`WaveletTree<T, Compare>`，维护长度为 $n$ 的静态序列，支持查询子区间 `[l, r)` 内的第 $k$ 小元素、严格小于给定值的元素个数以及落在值域区间 `[low, high)` 内的元素个数。依赖 [`bit.hpp`](bit.md)。

## 接口说明

```cpp
template <typename T, typename Compare = std::less<T>>
class WaveletTree {
public:
    WaveletTree(const std::vector<T>& values, Compare compare = Compare{});
    T kth(int l, int r, int k) const;
    int count_less(int l, int r, const T& value) const;
    int count(int l, int r, const T& low, const T& high) const;
};
```

- **模板参数与离散化特性**：
  - 元素类型 `T` **只需支持由 `Compare`（默认 `std::less<T>`）定义的严格弱序比较**，无需为整数或支持位运算（如 `double`、`std::string` 或自定义结构体均可直接使用）。构造函数内部会自动提取有序去重字母表 `alphabet`，并对离散化后的排名 $[0, \sigma)$ 按位建树（层数 `levels = ceil_log2(alphabet.size())`）。
- **下标、区间与排名约定**：所有序列下标与查询区间均为 **`0-based` 左闭右开 `[l, r)`**：
  - `kth(int l, int r, int k) const`：返回子区间 `[l, r)` 按 `compare` 排序后的 **`0-based` 第 `k` 小元素**（即 `k = 0` 表示区间最小值，`k = r - l - 1` 表示区间最大值）。**要求区间非空且 `0 <= l < r <= n`、`0 <= k < r - l`**（否则触发 `assert`）。
  - `count_less(int l, int r, const T& value) const`：返回子区间 `[l, r)`（要求 $0 \le l \le r \le n$）中严格满足 `compare(a[i], value)`（即 $< \text{value}$）的元素个数。`value` 不必曾出现在原数组中。
  - `count(int l, int r, const T& low, const T& high) const`：返回子区间 `[l, r)` 中落在**左闭右开值域区间 `[low, high)`** 内（即满足 `!compare(a[i], low) && compare(a[i], high)`，对应 `low <= a[i] < high`）的元素个数。**调用前必须保证 `!compare(high, low)`（即 `low <= high`）**，内部通过 `count_less(l, r, high) - count_less(l, r, low)` 计算。

## 复杂度

- 设 $n = \text{values.size()}$，$\sigma \le n$ 为原数组中不同元素的个数：
- 构造：时间复杂度 $O(n \log n)$（排序离散化加 $\lceil \log_2 \sigma \rceil$ 层划分），空间复杂度 $O(n \log \sigma)$。
- `kth`：时间复杂度 $O(\log \sigma)$。
- `count_less`、`count`：时间复杂度 $O(\log \sigma)$（含一次在 `alphabet` 上的二分查找 $O(\log \sigma)$）。

## 使用示例

```cpp
#include "wavelet_tree.hpp"
#include <cassert>
#include <vector>

int main() {
    std::vector<int> a = {5, 1, 7, 1, 3, 9, 2};
    WaveletTree<int> wt(a);

    // 子区间 [1, 5) 为 {1, 7, 1, 3}，排序后为 {1, 1, 3, 7}
    assert(wt.kth(1, 5, 0) == 1);
    assert(wt.kth(1, 5, 1) == 1);
    assert(wt.kth(1, 5, 2) == 3);
    assert(wt.kth(1, 5, 3) == 7);

    // 统计 [1, 5) 中 < 4 的元素个数: {1, 1, 3} 共 3 个
    assert(wt.count_less(1, 5, 4) == 3);

    // 统计 [0, 7) 中落在值域 [2, 7) 内的元素: {5, 3, 2} 共 3 个
    assert(wt.count(0, 7, 2, 7) == 3);
    return 0;
}
```

## 相关文档

- [PersistentSegmentTree](persistent_segtree.md) —— 同样可求解静态区间第 $k$ 小的可持久化线段树
- [CoordinateCompression](coordinate_compression.md) —— 独立使用的坐标压缩工具
