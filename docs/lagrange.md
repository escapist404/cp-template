# Lagrange

在有限域中计算经过给定点的多项式值。

## 接口

```cpp
template <typename T, T mod>
ModInt<T, mod> lagrange_evaluate(
    const std::vector<ModInt<T, mod>>& x,
    const std::vector<ModInt<T, mod>>& y,
    ModInt<T, mod> at);

template <typename T, T mod>
ModInt<T, mod> lagrange_evaluate_consecutive(
    const std::vector<ModInt<T, mod>>& y,
    ModInt<T, mod> at);
```

`lagrange_evaluate` 适用于任意互异横坐标，复杂度 $O(n^2)$；`lagrange_evaluate_consecutive` 适用于横坐标 `0,1,...,n-1`，复杂度 $O(n)$，并要求 `n < mod`。

## 使用示例

```cpp
#include "lagrange.hpp"
#include "modint.hpp"
#include <iostream>
#include <vector>

using mint = ModInt<int, 998244353>;

int main() {
    std::vector<mint> y = {1, 2, 4, 8};
    std::cout << lagrange_evaluate_consecutive(y, mint(10)) << '\n';

    std::vector<mint> x = {1, 3, 7};
    std::vector<mint> values = {2, 5, 11};
    std::cout << lagrange_evaluate(x, values, mint(10)) << '\n';
}
```

横坐标重复时会在除法处触发断言或产生无效结果。
