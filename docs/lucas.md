# Lucas / exLucas

本文件覆盖**两个不同**的接口，不要混用：

- `Lucas` 类 —— 模数为**素数**的 Lucas 定理
- `exlucas` 函数 —— 模数为**任意整数**的扩展 Lucas（分解质因数 + CRT 合并）

依赖 `crt.hpp`、`exgcd.hpp`、`modular_arithmetic.hpp`。

## Lucas 类（模素数）

```cpp
class Lucas {
public:
    Lucas(i64 prime);
    i64 choose(i64 n, i64 k) const;
};
```

### 构造函数

```cpp
Lucas(i64 prime);
```

预处理阶乘与逆阶乘表，表长为 `prime`（索引 $0$ 到 `prime-1`）。

**约束**
- 源码有 `assert(prime > 1)`
- **要求 `prime` 是素数**。实现只断言 `prime > 1`，素性由调用者保证；传入合数不在接口契约内
- 内存 $O(\mathrm{prime})$，因此 `prime` 必须较小（源码未注明上界，但实际限制在 $10^7$ 量级以内）

### choose

```cpp
i64 choose(i64 n, i64 k) const;
```

返回 $\binom{n}{k} \bmod \mathrm{prime}$。

**行为**
- `k < 0 || k > n` 时返回 `0`
- 其余按 Lucas 定理逐位计算（逐位比较，`ki > ni` 时返回 `0`）

**适用**：$n$ 可以很大（任意非负 `i64`），只要模数小就行。

## exlucas（模任意整数）

```cpp
inline i64 exlucas(i64 n, i64 k, i64 modulus);
```

返回 $\binom{n}{k} \bmod \mathrm{modulus}$。

**约束**
- 源码有 `assert(n >= 0 && modulus > 0)`
- `k < 0 || k > n || modulus == 1` 时返回 `0`
- `modulus` 可为**任意正整数**：不必是素数，也不必是无平方因子数

**实现**
1. 对 `modulus` 试除分解为 $p^q$ 的乘积
2. 对每个 $p^q$ 用 `choose_prime_power` 算出 $\binom{n}{k} \bmod p^q$，得到一个 CRT 方程
3. 用 [crt](crt.md) 合并，取 `.first` 作为答案

**关键细节**：若 $v_p\binom{n}{k} \geq q$（即 $p^q$ 整除结果），该分量直接返回 0。这里的指数由
$$v_p = v_p(n!) - v_p(k!) - v_p((n-k)!)$$
给出。

## 使用示例

### 小素数模数，大 n（用 Lucas 类）

```cpp
#include "lucas.hpp"
#include <iostream>

int main() {
    Lucas lucas(1000003);   // 素数模数
    std::cout << lucas.choose(1000000000000LL, 5) << '\n';

    return 0;
}
```

### 任意模数（用 exlucas）

```cpp
#include "lucas.hpp"
#include <iostream>

int main() {
    std::cout << exlucas(10, 3, 12) << '\n';        // C(10,3)=120 → 120 mod 12 = 0
    std::cout << exlucas(10, 3, 1000000007) << '\n'; // 120
    std::cout << exlucas(10, 3, 1) << '\n';          // 0

    return 0;
}
```

## 两条路径的选择

| 场景 | 用哪个 |
|------|--------|
| 模数是小素数（$\leq 10^7$），$n$ 极大 | `Lucas` 类 |
| 模数是合数 | `exlucas` |
| 模数是素数但很大（如 $10^9+7$） | 两者都不理想：`Lucas` 的表开不下；`exlucas` 会分配 $O(\text{modulus})$ 内存 |

模数为大素数时，直接用 [ModInt](modint.md) 预处理阶乘更合适（前提是 $n < \mathrm{mod}$）。

## 相关文档

- [CRT](crt.md) —— `exlucas` 的合并工具
- [ExGCD](exgcd.md) —— 求逆元
- [ModInt](modint.md) —— 大素数模数下的阶乘预处理
- [Comb](comb.md) —— 模板化的组合数类
