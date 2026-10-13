# Comb

在固定上界内预处理阶乘和逆阶乘，快速计算组合数。

## 模板参数

```cpp
template <typename Z>
class Combinatorics;
```

`Z` 无默认值，通常是 [ModInt](modint.md) 的实例，例如 `ModInt<int, 998244353>`。

**对 `Z` 的要求**
- 可从 `int` 构造
- 支持 `operator*`、`operator/`
- 支持赋值与比较

除法用于求逆阶乘（内部用 `Z(1) / fact_[n]`），因此 **`Z` 的除法必须成立**——对 `ModInt` 而言这意味着模数必须为素数。

## 构造函数

```cpp
Combinatorics(const int n);
```

预处理 $0!$ 到 $n!$ 及其逆。

**约束**
- 源码无 `assert`
- `n < 0` 时两个表均为空，得到合法的「空对象」状态，之后所有查询返回 `Z(0)`

**复杂度**
- $O(n)$

## 成员函数

所有查询在越界时**静默返回 `Z(0)`**，不抛异常、不断言。

### factorial

```cpp
Z factorial(const int n) const;
```

返回 $n!$。`n < 0` 或 `n >= 表长` 时返回 `Z(0)`。

### inverse_factorial

```cpp
Z inverse_factorial(const int n) const;
```

返回 $(n!)^{-1}$。越界返回 `Z(0)`。

### choose

```cpp
Z choose(const int n, const int k) const;
```

返回 $\binom{n}{k}$。

- `n` 越界、`k < 0`、`k > n` 时返回 `Z(0)`
- 否则 `fact_[n] * ifact_[k] * ifact_[n - k]`

### operator()

```cpp
Z operator()(const int n, const int k) const;
```

等价于 `choose(n, k)`，写法更短。

## 使用示例

```cpp
#include "comb.hpp"
#include "modint.hpp"
#include <iostream>

using mint = ModInt<int, 998244353>;

int main() {
    Combinatorics<mint> comb(1000);

    std::cout << comb.choose(10, 3) << '\n';     // 120
    std::cout << comb(10, 3) << '\n';            // 120
    std::cout << comb.factorial(5) << '\n';      // 120

    // 越界静默返回 0
    std::cout << comb.choose(1000, 5000) << '\n';  // 0
    std::cout << comb.choose(-1, 0) << '\n';       // 0

    return 0;
}
```

### 与 Lucas 配合处理大 n

`Combinatorics` 要求 `n` 不超过构造上界，且（对 `ModInt`）`n < mod` 才能保证阶乘可逆。当 $n$ 极大时改用 [Lucas](lucas.md) 或 [exLucas](lucas.md)。

## 相关文档

- [ModInt](modint.md) —— 本类的类型参数来源
- [Lucas](lucas.md) —— 模数小、$n$ 大时的替代
- [Lagrange](lagrange.md) —— 插值，同样需要阶乘逆
- [FloorSum](floor_sum.md) —— 类欧几里得
