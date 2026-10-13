# Fenwick2D

维护可修改的二维数组，支持单点加法和矩形求和。下标从 `0` 开始，区间使用半开表示。

## 接口

```cpp
template <typename T>
class Fenwick2D;

Fenwick2D(int n, int m);
void add(int x, int y, T value);
T get(int x, int y) const;                    // [0,x) × [0,y)
T get(int l, int d, int r, int u) const;      // [l,r) × [d,u)
```

## 使用示例

```cpp
#include "fenwick2d.hpp"
#include <iostream>

int main() {
    Fenwick2D<long long> f(5, 5);
    f.add(2, 3, 10);
    f.add(4, 4, 20);
    std::cout << f.get(0, 0, 5, 5) << '\n'; // 30
    std::cout << f.get(0, 0, 3, 4) << '\n'; // 10
}
```

空间复杂度为 $O(nm)$，单次操作为 $O(\log n\log m)$。`T` 需要支持默认构造、`+=` 和减法。
