# MillerRabin

Miller-Rabin 素性测试，确定性覆盖整个 `u64` 范围。

## 接口

```cpp
inline bool is_prime_mr(i64 n);
```

## is_prime_mr

```cpp
inline bool is_prime_mr(i64 n);
```

判断 `n` 是否为素数。

**返回值**
- 素数为 `true`
- **负数、0、1 一律返回 `false`**（源码先判断 `n < 2`，不会因强制转换出错）

**复杂度**
- $O(\log^3 n)$ 量级（固定常数轮）

**流程**
1. `n < 2` 返回 `false`
2. 先用小素数 `{2,3,5,7,11,13,17,19,23,29,31,37}` 试除；若 `n % p == 0` 则返回 `n == p`
3. 再用固定底数集合 `{2, 325, 9375, 28178, 450775, 9780504, 1795265022}` 做 Miller-Rabin，任一底数为见证者则返回 `false`

**注意**：这组底数是覆盖整个 `u64` 范围的**确定性**集合，因此本函数对 64 位整数**不存在误判概率**（不是概率算法）。源码未在注释中说明这一点，但底数集合是硬编码的。

## 使用示例

```cpp
#include "miller_rabin.hpp"
#include <iostream>

int main() {
    std::cout << is_prime_mr(1000000007) << '\n';      // 1
    std::cout << is_prime_mr(1000000008) << '\n';      // 0
    std::cout << is_prime_mr(1) << '\n';               // 0
    std::cout << is_prime_mr(0) << '\n';               // 0
    std::cout << is_prime_mr(-7) << '\n';              // 0（负数恒 false）

    // 大素数
    std::cout << is_prime_mr(9223372036854775783LL) << '\n';   // 1

    return 0;
}
```

### 筛出一段区间内的素数

```cpp
std::vector<i64> primes_in(i64 lo, i64 hi) {
    std::vector<i64> result;
    for (i64 x = std::max<i64>(lo, 2); x <= hi; ++x) {
        if (is_prime_mr(x)) result.push_back(x);
    }
    return result;
}
```

小范围（$10^7$ 以内）仍建议用 [sieve](sieve.md)，快得多。

### 补充说明与使用约定

- 头文件只提供 `is_prime_mr` 一个接口。
- 本文件依赖 `modular_arithmetic.hpp`（间接引入 `types.hpp` 的 `u64`），**不要**单独把 `miller_rabin.hpp` 抄进题面而不带这个依赖。
- 新代码统一使用 `is_prime_mr`，避免与其他素性测试实现重名。

## 相关文档

- [PollardRho](pollard_rho.md) —— 质因数分解，依赖本文件
- [Sieve](sieve.md) —— 小范围（$10^7$ 以内）的筛法，更快
- [Min25](min25.md) —— 大范围素数计数与求和
