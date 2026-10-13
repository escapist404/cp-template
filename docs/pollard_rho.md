# PollardRho

给定一个大整数，求出它的质因数分解，适用于接近 $10^{18}$ 的范围。

## 接口

```cpp
inline std::vector<i64> factorize(const i64 n);
```

依赖 `miller_rabin.hpp`（进而 `modular_arithmetic.hpp`）。

## 返回值

- **升序排序**的质因数列表
- **带重数**（重复因子重复出现），例如 `factorize(12)` 得到 `{2, 2, 3}`
- `n < 1` 时返回**空 `vector`**（`if (n < 1) return {};`），因此 `n = 0` 与负数都返回空
- `n == 1` 返回空
- `n` 为素数时返回 `{n}`

**注意**：返回的是「质因数列表」而非「因子个数」或「不同素因子」。要得到不同素因子需自行 `std::unique`（[modular_roots.hpp](modular_roots.md) 就是这样用的）。

**复杂度**
- 期望 $O(n^{1/4})$ 量级；源码未注明

## 使用示例

```cpp
#include "pollard_rho.hpp"
#include <algorithm>
#include <iostream>

int main() {
    auto factors = factorize(1234567890123456LL);
    for (i64 p : factors) std::cout << p << ' ';
    std::cout << '\n';

    // 结果是带重数的升序列表
    for (i64 p : factorize(12)) std::cout << p << ' ';
    std::cout << '\n';   // 2 2 3

    return 0;
}
```

### 分解为「质因子 × 指数」

```cpp
std::vector<std::pair<i64, int>> factor_exponents(i64 n) {
    auto factors = factorize(n);
    std::vector<std::pair<i64, int>> result;
    for (size_t i = 0; i < factors.size(); ) {
        size_t j = i;
        while (j < factors.size() && factors[j] == factors[i]) ++j;
        result.push_back({factors[i], (int)(j - i)});
        i = j;
    }
    return result;
}
```

### 枚举因数个数（用除数函数公式）

```cpp
long long divisor_count(i64 n) {
    long long result = 1;
    for (auto [p, e] : factor_exponents(n)) result *= (e + 1);
    return result;
}
```

## 底层结构

```cpp
inline u64 pollard_rho_split(u64 n);                         // 找一个非平凡因子
inline void pollard_rho_factor(u64 n, std::vector<i64>& factors); // 递归分解
```

- `pollard_rho_split`：偶数直接返回 2；否则用 Floyd 龟兔随机迭代（$f(x) = x^2 + c \bmod n$）寻找非平凡因子，`divisor != n` 时返回，等于 `n` 时重试
- 迭代中的乘法用 `multiply_mod` 防溢出

## 前置条件与实现约定

- **结果是随机化的**：内部用 `std::mt19937_64`，种子取自 `std::chrono::steady_clock::now()`，每次运行的迭代路径不同——但**因子集合是确定的**。
- 依赖 `<random>` 与 `<chrono>`，不是纯确定性算法。多测环境下若要求完全可复现需自行固定种子。
- 随机引擎是 `pollard_rho_split` 内的 `static` 局部变量，**跨调用共享状态**。
- 对 0 与负数返回空列表（而非报错），调用时注意区分「n = 0」与「n 无质因子」。
- `pollard_rho_split` 隐含要求 `n >= 3`；实际只由 `pollard_rho_factor` 在 `!is_prime_mr(n)` 的奇合数上调用，最小为 9，因此是安全的——不要直接对小于 3 的数调用它。
- 分解大素数时 `miller_rabin.hpp` 会先判定并直接返回，不做无用功。

## 相关文档

- [MillerRabin](miller_rabin.md) —— 素性测试，本文件的依赖
- [Sieve](sieve.md) —— $10^7$ 以内的高效筛法替代
- [Min25](min25.md) —— 大范围素数计数与求和
- [ModularRoots](modular_roots.md) —— 使用本文件做模平方根与原根
