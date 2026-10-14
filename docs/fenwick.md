# Fenwick Tree (`fenwick.hpp`)

`fenwick.hpp` 实现一维树状数组（Binary Indexed Tree）`Fenwick<T>`，支持单点加法更新、前缀/区间和查询以及在树状数组上直接树上倍增二分前缀和下界。依赖 [`bit.hpp`](bit.md)。

## 接口说明

```cpp
template <typename T>
class Fenwick {
public:
    Fenwick(int n);
    Fenwick(std::vector<T> init);
    void add(int p, T value);
    T get(int p);
    T get(int l, int r);
    int lower_bound(T value);
};
```

- **索引与区间约定**：对外接口全部采用 **`0-based` 下标**与**左闭右开区间 `[l, r)`**（内部自动转换为 `1-based` 树状数组下标维护）。类型 `T` 需支持默认构造 `T()`（作为零元）、`+=`、`-` 和 `<`。
- `Fenwick(int n)`：构造长度为 $n$（要求 $n \ge 0$）的全零数组。
- `Fenwick(std::vector<T> init)`：用给定数组 `init` 初始化长度为 `init.size()` 的树状数组。
- `add(int p, T value)`：将下标 `p` 处的元素加上 `value`（即 `a[p] += value`），要求 $0 \le p < n$。
- `get(int p)`：返回前缀半开区间 **`[0, p)`** 的元素和 $\sum_{i=0}^{p-1} a[i]$，要求 $0 \le p \le n$。注意当 `p == 0` 时返回 `T()`（即 $0$），查询包含下标 `i` 在内的前缀和应传入 `get(i + 1)`。
- `get(int l, int r)`：返回半开区间 **`[l, r)`** 的元素和 $\sum_{i=l}^{r-1} a[i]$，要求 $0 \le l \le r \le n$；当 `l == r` 时返回 `T()`。
- `lower_bound(T value)`：在树状数组上执行倍增二分，返回满足前缀和 **`get(p + 1) >= value`**（即 $\sum_{i=0}^{p} a[i] \ge \text{value}$）的最小 `0-based` 下标 `p`：
  - **单调性前提**：调用 `lower_bound` 时，原数组中**所有元素必须非负**（$a[i] \ge 0$），以保证前缀和单调不降；
  - 若 `value <= T()`（或 `n == 0`），直接返回 `0`；
  - 若整个数组的总和 `get(n) < value`，则返回 `n`。

## 复杂度

- 构造：全零构造为 $O(n)$；从数组构造为 $O(n \log n)$。
- `add`、`get`、`lower_bound`：时间复杂度均为 $O(\log n)$。
- 空间复杂度：$O(n)$。

## 使用示例

```cpp
#include "fenwick.hpp"
#include <cassert>
#include <vector>

int main() {
    Fenwick<long long> fw(std::vector<long long>{2, 1, 4, 3, 5});

    assert(fw.get(0) == 0);
    assert(fw.get(3) == 7);      // [0, 3) -> 2 + 1 + 4 = 7
    assert(fw.get(1, 4) == 8);   // [1, 4) -> 1 + 4 + 3 = 8

    fw.add(2, 3);                // 数组变为 {2, 1, 7, 3, 5}
    assert(fw.get(1, 4) == 11);

    // 前缀和依次为: p=0:2, p=1:3, p=2:10, p=3:13, p=4:18
    assert(fw.lower_bound(10) == 2);
    assert(fw.lower_bound(11) == 3);
    assert(fw.lower_bound(20) == 5);
    return 0;
}
```

## 相关文档

- [Fenwick2D](fenwick2d.md) —— 二维树状数组
- [SegmentTree](segtree.md) —— 支持一般结合律半群运算与区间二分的线段树
- [CoordinateCompression](coordinate_compression.md) —— 配合树状数组处理大值域偏序统计
