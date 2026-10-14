# modular_arithmetic

防溢出的模乘与快速幂，供库内其他数论模块复用。

## 接口

```cpp
template <typename T> constexpr T multiply_mod(const T a, const T b, const T mod);
template <typename T> constexpr T power_mod(T base, T exponent, const T mod);
```

两个函数直接位于全局，可写作 `multiply_mod(a, b, mod)` 和 `power_mod(base, exponent, mod)`。

**注意**：这组函数与 [ModInt](modint.md) 里的 `power` 是**不同的东西**——`power` 是类模板泛型快速幂（可用于 `ModInt` 或矩阵），`power_mod` 是整数专用、内建取模的版本。两者不冲突但用途不同。

## 模板参数

```cpp
template <typename T>
```

- 要求 `std::is_integral_v<T>` 且 `sizeof(T) <= sizeof(u64)`，有 `static_assert`
- **`__int128` 不满足该断言**（`sizeof` 为 16），会编译失败

## multiply_mod

```cpp
constexpr T multiply_mod(const T a, const T b, const T mod);
```

返回 $(a \cdot b) \bmod \mathrm{mod}$。

**行为**
- 有符号 `T`：(i128)a * b % mod，再把负结果加 `mod` 归正
- 无符号 `T`：(u128)a * b % mod
- 结果总在 $[0, \mathrm{mod})$ 内

**约束**
- 源码有 `static_assert`（整型、`sizeof(T) <= sizeof(u64)`）与 `assert(mod > 0)`

## power_mod

```cpp
constexpr T power_mod(T base, T exponent, const T mod);
```

返回 $\mathrm{base}^{\mathrm{exponent}} \bmod \mathrm{mod}$，二进制快速幂。

**行为**
- `base %= mod`，有符号类型时若 `base < 0` 再加 `mod` 归正
- 从 `result = 1 % mod` 起算，因此 **`mod == 1` 时返回 0**（而不是 1）

**约束**
- 同上的 `static_assert` 与 `assert(mod > 0)`
- 有符号 `T` 时有 `assert(exponent >= 0)`

**重要**：所有参数必须**同为 `T`**。模板只有一个类型参数，混用 `int` 与 `long long` 会导致推导失败。

## 使用示例

```cpp
#include "modular_arithmetic.hpp"
#include <iostream>

int main() {
    // 大数取模的中间乘法，用 i64 显式指定类型
    i64 a = 1000000000000LL;
    i64 b = 1000000000000LL;
    const i64 mod = 998244353;
    std::cout << multiply_mod<i64>(a % mod, b % mod, mod) << '\n';

    // 快速幂
    std::cout << power_mod<i64>(2, 30, 1000000007) << '\n';

    // mod == 1 时返回 0
    std::cout << power_mod<i64>(2, 30, 1) << '\n';   // 0

    return 0;
}
```

### 补充说明与使用约定

- 必须显式写出 `T`，或确保三个实参类型一致；`multiply_mod(2, 3, 1000000007)` 中字面量都是 `int`，会以 `T = int` 实例化——若乘积超 `i128` 能容纳的范围则没问题，但结果类型也是 `int`。
- 不能用于 `__int128`：`sizeof` 断言会拦下。需要 128 位模运算时得自己写，或用 [ModInt](modint.md)。
- 两个函数都是 `constexpr`，可用于常量表达式；但若在常量求值中触发 `assert`，会直接编译错误（而非运行时报错）。
- `power_mod` 的指数在有符号类型下不可为负，会断言失败。

## 相关文档

- [ModInt](modint.md) —— 面向对象的模运算，带运算符重载
- [ExGCD](exgcd.md) —— 合数模数求逆
