# FloorSum

计算
\[
\sum_{i=l}^{r-1} \left\lfloor\frac{a i+b}{m}\right\rfloor
\]
而不逐项枚举。

## 接口

```cpp
inline i64 floor_sum(i64 l, i64 r, i64 m, i64 a, i64 b);
```

要求 `0 <= l <= r`、`m > 0`；`a`、`b` 可以为负。复杂度为 $O(\log m)$，结果必须能表示为 `i64`。

## 使用示例

```cpp
#include "floor_sum.hpp"
#include <iostream>

int main() {
    // sum_{i=0}^{9} floor((3*i + 5) / 7)
    std::cout << floor_sum(0, 10, 7, 3, 5) << '\n';
}
```

`i` 的范围是半开区间 `[l, r)`。若要计算 `[0,n)`，调用 `floor_sum(0, n, m, a, b)`。
