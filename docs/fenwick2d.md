# 2D Fenwick Tree (`fenwick2d.hpp`)

`fenwick2d.hpp` 实现二维树状数组 `Fenwick2D<T>`，维护一个 $n \times m$ 的二维网格，支持单点加法修改与二维前缀/子矩形求和查询。依赖 [`bit.hpp`](bit.md)。

## 接口说明

```cpp
template <typename T>
class Fenwick2D {
public:
    Fenwick2D(int n, int m);
    void add(int x, int y, T value);
    T get(int x, int y) const;
    T get(int l, int d, int r, int u) const;
};
```

- **索引与区域约定**：所有坐标均为 **`0-based`**，所有二维区域均为**左闭右开笛卡尔积**。类型 `T` 必须构成加法阿贝尔群（支持值初始化 `T{}` 作为零元、`+=`、`+` 与 `-`）。
- `Fenwick2D(int n, int m)`：构造尺寸为 $n \times m$（要求 $n \ge 0, m \ge 0$）的全零二维树状数组。
- `add(int x, int y, T value)`：将网格点 `(x, y)` 的权值加上 `value`，要求 $0 \le x < n$ 且 $0 \le y < m$。
- `get(int x, int y) const`：返回二维前缀半开矩形 **`[0, x) × [0, y)`** 的元素总和 $\sum_{i=0}^{x-1}\sum_{j=0}^{y-1} a[i][j]$，要求 $0 \le x \le n$ 且 $0 \le y \le m$。当 `x == 0` 或 `y == 0` 时返回 `T{}`。
- `get(int l, int d, int r, int u) const`：通过二维容斥 `get(r, u) - get(l, u) - get(r, d) + get(l, d)` 返回半开子矩形 **`[l, r) × [d, u)`** 的元素总和 $\sum_{i=l}^{r-1}\sum_{j=d}^{u-1} a[i][j]$。**注意参数顺序为 `(左 l, 下 d, 右 r, 上 u)`**（即两个端点坐标 `(l, d)` 与 `(r, u)` 交替传入，而非 `(l, r, d, u)`），要求 $0 \le l \le r \le n$ 且 $0 \le d \le u \le m$。

## 复杂度

- 构造：时间与空间复杂度均为 $O(nm)$。
- `add`、前缀 `get(x, y)`、子矩形 `get(l, d, r, u)`：时间复杂度均为 $O(\log n \log m)$。

## 使用示例

```cpp
#include "fenwick2d.hpp"
#include <cassert>

int main() {
    Fenwick2D<long long> bit(4, 5);
    bit.add(1, 2, 5);
    bit.add(2, 3, 7);
    bit.add(0, 0, 3);

    // 前缀 [0, 3) x [0, 4) 包含 (0,0), (1,2), (2,3)
    assert(bit.get(3, 4) == 15);

    // 子矩形 x in [1, 3), y in [2, 4) 包含 (1,2) 和 (2,3)
    assert(bit.get(1, 2, 3, 4) == 12);
    return 0;
}
```

## 相关文档

- [Fenwick](fenwick.md) —— 一维树状数组
- [CDQ](cdq.md) —— 离线处理高维大值域偏序求和
