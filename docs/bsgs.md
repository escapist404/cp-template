# BSGS

给定整数 $a,b,m$，求最小的非负整数 $x$ 使 $a^x \equiv b \pmod m$。

## 接口

```cpp
inline i64 bsgs(i64 a, i64 b, i64 mod);
```

依赖 `types.hpp` 和 `bit.hpp`。

## 语义

求**最小的非负整数** $x$ 使

$$a^x \equiv b \pmod{\mathrm{mod}}$$

**返回值**
- 最小的非负解 $x$
- 无解返回 `-1`

**提前返回**
- `mod == 1` 时返回 `0`
- `b == 1` 时返回 `0`（因为 $a^0 = 1$）

**参数处理**
- `a`、`b` 会先取模并规范化到 $[0, \mathrm{mod})$

**约束**
- 源码有 `assert(mod > 0)`
- 参数顺序是 `(a, b, mod)`

**复杂度**
- $O(\sqrt{\mathrm{mod}})$ 时间与空间

## 使用示例

```cpp
#include "bsgs.hpp"
#include <iostream>

int main() {
    // 求 2^x ≡ 8 (mod 11)   → x = 3
    std::cout << bsgs(2, 8, 11) << '\n';

    // 无解时返回 -1
    std::cout << bsgs(2, 7, 11) << '\n';

    // mod == 1 或 b == 1
    std::cout << bsgs(2, 3, 1) << '\n';   // 0
    std::cout << bsgs(2, 1, 11) << '\n';  // 0

    return 0;
}
```

### 求原根的最小指数（离散对数表）

```cpp
// 若 g 是 mod 的原根，则 g^x 遍历所有非零剩余
i64 discrete_log_of(i64 value, i64 g, i64 mod) {
    return bsgs(g, value, mod);
}
```

## 算法说明

设块长 $m = \lceil\sqrt{\mathrm{mod}}\rceil$，把 $x$ 写成 $x = im - j$：

$$a^{im - j} \equiv b \implies a^{im} \equiv b \cdot a^{j}$$

于是枚举 $j \in [0, m)$ 把 $b \cdot a^j$ 存表（baby step），再枚举 $i$ 查表（giant step）。

**注意块长被内部修正**为真正的 $\lceil\sqrt{\mathrm{mod}}\rceil$，使用公共的 `ceil_sqrt` 整数工具。

## 非互质的处理

本实现**对 $\gcd(a, \mathrm{mod}) \neq 1$ 也正确**（源码注释明确说明这一点，这是相对教科书 BSGS 的增强）。内部先做 gcd 剥离：

1. 反复取 $g = \gcd(a, \mathrm{mod})$
2. 若 $b \bmod g \neq 0$ —— 无解，返回 `-1`
3. 否则同步缩小 $b$、`mod`，并把已剥离的因子累乘到 `multiplier`、步数累加到 `removed`
4. 若中途 `multiplier == b`，直接返回 `removed`
5. 最终答案折算为 `i * block - j + removed`

## 前置条件与实现约定

- **乘法用 `(i128)x * y % m` 显式防溢出**，这部分是安全的。
- baby-step 表用 `std::unordered_map<i64, i64>`，同值冲突时**后写覆盖前写**（源码 `baby[value] = j;` 直接赋值）。源码未说明这是否影响「最小 $x$」的保证；如果对最小性有严格要求，建议自行改写为保留较小 $j$。
- 参数顺序容易记错：是 `(底数, 目标值, 模数)`，不是 `(mod, a, b)`。
- 复杂度是 $O(\sqrt{\mathrm{mod}})$ 的**时间与内存**，`mod` 到 $10^{12}$ 量级时表会占约 $\sqrt{mod}$ 项，内存需留意。
- `mod` 为 `i64`，源码未说明超过 $2^{63}$ 平方根上限时的行为。

## 相关文档

- [ExGCD](exgcd.md) —— 线性同余方程（$ax \equiv b$），与离散对数不同
- [ModularRoots](modular_roots.md) —— 模意义下的平方根与原根
- [MillerRabin](miller_rabin.md) / [PollardRho](pollard_rho.md) —— 大数素性与分解
