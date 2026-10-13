# exgcd

给定整数 $a,b$，求 $ax+by=\gcd(a,b)$ 的一组解，并据此求一般模数下的乘法逆元。

## 接口

```cpp
inline i64 extended_gcd(const i64 a, const i64 b, i64 &x, i64 &y);
inline i64 mod_inverse(const i64 a, const i64 mod);
```

两个自由函数，非模板。依赖 `types.hpp`（`i64`）。

## extended_gcd

```cpp
inline i64 extended_gcd(const i64 a, const i64 b, i64 &x, i64 &y);
```

求 $\gcd(a, b)$ 以及一组 $(x, y)$ 使

$$a x + b y = \gcd(a, b)$$

**返回值**
- $\gcd(|a|, |b|)$，**恒为非负**

**输出参数**
- `x`、`y` 通过引用返回，满足上面的等式
- 内部先对 $|a|, |b|$ 做迭代扩展欧几里得，最后按 `a`、`b` 的符号分别翻转 $x$、$y$，因此**负数输入也能正确工作**

**边界**
- `extended_gcd(0, 0, x, y)` 时循环不执行，返回 0，得到 `x = 1, y = 0`

**复杂度**
- $O(\log \min(|a|, |b|))$

## mod_inverse

```cpp
inline i64 mod_inverse(const i64 a, const i64 mod);
```

求 $a$ 在模 `mod` 下的乘法逆元。

**返回值**
- 成功时返回 $[0, \mathrm{mod})$ 内的逆元
- `mod <= 0` 时返回 `-1`
- $\gcd(a, \mathrm{mod}) \neq 1$（无逆元）时返回 `-1`

**约束**
- **`mod` 不必是质数**，只需 $\gcd(a, \mathrm{mod}) = 1$。这正是本函数相对 `ModInt::operator/=`（费马小定理，要求质数）的价值所在。
- `a` 可以是任意 `i64`（含负数、含超过 `mod` 的值），内部会处理，**无需预先取模**

**判定技巧**
- `-1` 兼作「无逆元 / 非法模数」的哨兵
- 由于合法逆元一定被规范化到 $[0, \mathrm{mod})$，**返回 `-1` 必然是失败**，不会与真逆元混淆

**复杂度**
- $O(\log \mathrm{mod})$

## 使用示例

### 解二元一次不定方程

```cpp
#include "exgcd.hpp"
#include <iostream>

int main() {
    i64 x, y;
    i64 g = extended_gcd(12, 18, x, y);
    std::cout << g << ' ' << x << ' ' << y << '\n';
    // 6 和一组满足 12x + 18y = 6 的解

    return 0;
}
```

### 合数模数下求逆元

```cpp
#include "exgcd.hpp"
#include <iostream>

int main() {
    // 模数是合数，ModInt 的除法不可用，但 exgcd 可以
    i64 inv = mod_inverse(3, 10);
    std::cout << inv << '\n';   // 7，因为 3 * 7 = 21 ≡ 1 (mod 10)

    // 无逆元
    std::cout << mod_inverse(2, 10) << '\n';   // -1

    return 0;
}
```

### 配合扩展欧几里得解同余方程

```cpp
// 解 a*x ≡ b (mod m)
i64 solve_congruence(i64 a, i64 b, i64 m) {
    i64 x, y;
    i64 g = extended_gcd(a, m, x, y);
    if (b % g != 0) return -1;            // 无解
    i64 solution = (x % (m / g) + (m / g)) % (m / g);
    return solution * (b / g) % (m / g);
}
```

## 前置条件与实现约定

- 内部用 `std::llabs` 取绝对值，对 `LLONG_MIN` 的行为源码未说明（标准上 `llabs(LLONG_MIN)` 是未定义的）。
- 返回的 $x, y$ 是**通解中的一组特解**，数值可能很大；若需要最小非负解，要自己按模数归约。
- `extended_gcd` 不检查 `a == b == 0`，此时返回 0 并给出 `x = 1, y = 0`，调用方自行判断。

## 相关文档

- [CRT](crt.md) —— 中国剩余定理，内部依赖本文件
- [ModInt](modint.md) —— 质数模数下的除法（费马小定理）
- [BSGS](bsgs.md) —— 离散对数
- [Lucas](lucas.md) —— `exlucas` 依赖 `mod_inverse`
