# Sieve

在上界内枚举质数，并可在同一次遍历中计算常见积性函数或最小质因子。

## 通用接口

```cpp
template <typename OnPrime, typename OnComposite>
std::vector<int> sieve(int n, OnPrime on_prime, OnComposite on_composite);
std::vector<int> sieve_prime(int n);
```

筛法在发现质数时调用 `on_prime(p)`，在生成合数 `i * p` 时调用
`on_composite(i, p, i * p, divisible)`，其中 `divisible` 表示 `p | i`。
`sieve` 的返回值是筛出的质数列表；`sieve_prime` 是不需要回调时的简化接口。

两个回调可以捕获目标数组和辅助数组，因此同一个遍历可以实现不同的积性函数。

**复杂度**：`O(n)` 时间，`O(n)` 空间。

## 欧拉函数与莫比乌斯函数

```cpp
std::pair<std::vector<int>, std::vector<int>> sieve_phi(int n);
std::pair<std::vector<int>, std::vector<int>> sieve_mu(int n);
std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
sieve_arithmetic(int n);
```

- `sieve_phi` 返回 `{primes, phi}`；
- `sieve_mu` 返回 `{primes, mu}`；
- `sieve_arithmetic` 返回 `{primes, phi, mu}`。

三个接口要求 `n >= 1`，函数表下标范围为 `[0, n]`，其中 `phi[1] = 1`、`mu[1] = 1`。

## 因子个数与因子和

```cpp
std::tuple<std::vector<int>, std::vector<int>, std::vector<long long>>
sieve_divisor(int n);
```

返回 `{primes, tau, sigma}`，其中 `tau[x]` 是正因子个数，`sigma[x]` 是正因子之和。
例如 `tau[12] == 6`、`sigma[12] == 28`。

## 最小质因子

```cpp
std::pair<std::vector<int>, std::vector<int>> sieve_factor(int n);
```

返回 `{primes, min_factor}`，其中 `min_factor[x]` 是 `x` 的最小质因子。
对 `min_factor` 的反复查询可以在 `O(log n)` 内分解不超过预处理上界的数。

## 示例

```cpp
#include "sieve.hpp"

auto [phi_primes, phi] = sieve_phi(100);
auto [primes, phi_table, mu] = sieve_arithmetic(100);
auto [factor_primes, min_factor] = sieve_factor(100);
```
