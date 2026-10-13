# ModularRoots

模平方根与原根（Tonelli-Shanks 与原根试探）。

依赖 `pollard_rho.hpp`（进而依赖 `miller_rabin.hpp`、`modular_arithmetic.hpp`）与 `types.hpp`。

## 接口

```cpp
inline i64 modular_sqrt(i64 value, i64 prime);
inline i64 primitive_root_prime(i64 prime);
```

两个接口都要求 `prime` 是素数。调试构建中的 `assert(is_prime_mr(prime))` 可帮助发现误用；竞赛代码应直接满足此前置条件，不能把它当作运行时校验。

## modular_sqrt

```cpp
inline i64 modular_sqrt(i64 value, i64 prime);
```

返回 `x` 使 `x * x ≡ value (mod prime)`。

- 有解时返回最小的非负解 `x`，范围为 `[0, prime)`。
- 无解时返回 `-1`。
- 输入 `value` 会先规范到 `[0, prime)`。

实现使用 Tonelli-Shanks 算法：先用 Legendre 符号判断二次剩余，再在 `prime - 1` 的 2-adic 分解上迭代求根。复杂度为 $O(\log^2 prime)$。

## primitive_root_prime

```cpp
inline i64 primitive_root_prime(i64 prime);
```

返回素数 `prime` 的最小原根。实现先分解 `prime - 1` 的不同素因子，再从 `2` 起试探候选值。

## 使用示例

```cpp
#include "modular_roots.hpp"
#include <iostream>

int main() {
    i64 p = 1000000007;
    std::cout << modular_sqrt(4, p) << '\n';
    std::cout << primitive_root_prime(p) << '\n';
}
```

## 相关文档

- [PollardRho](pollard_rho.md) —— 质因数分解
- [BSGS](bsgs.md) —— 离散对数（与原根配合使用）
- [ModInt](modint.md) —— 模运算
- [ExGCD](exgcd.md) —— 求逆元
