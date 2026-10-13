# euler

计算整数的互质计数，并在底数与模数互质时把大指数幂化简到较小指数；同时支持十进制字符串取模和大小比较。

> **重要澄清**：这个文件**不是**欧拉路径（那在 [euler_path.hpp](euler_path.md)）。它处理的是**数论中的欧拉函数 $\varphi(n)$**。

依赖 `bigint.hpp` 与 `modular_arithmetic.hpp`（`power_mod`），使用 `types.hpp` 的 `i64`。

## 接口

```cpp
inline i64 euler_phi(i64 n);
inline i64 euler_power(const i64 a, const i64 exponent, const i64 mod);
inline i64 decimal_mod(const std::string &number, const i64 mod);
inline bool decimal_at_least(const std::string &number, const i64 value);
inline i64 extended_euler_power(const i64 a, const BigInt &exponent, const i64 mod);
inline i64 extended_euler_power(const i64 a, const std::string &exponent, const i64 mod);
```

## euler_phi

```cpp
inline i64 euler_phi(i64 n);
```

试除法求 $\varphi(n)$。

**约束**
- 有 `assert(n > 0)`

**复杂度**
- $O(\sqrt n)$（循环条件 `p <= n / p`）

## euler_power

```cpp
inline i64 euler_power(const i64 a, const i64 exponent, const i64 mod);
```

返回 $a^{\mathrm{exponent}} \bmod \mathrm{mod}$，用欧拉定理把指数降为 $\mathrm{exponent} \bmod \varphi(\mathrm{mod})$。

**约束**（源码 `assert` 明确写出）
- `mod > 0`
- `exponent >= 0`
- **`std::gcd(a, mod) == 1`** —— 只处理互素情形，这是本函数与 `extended_euler_power` 的关键区别

## extended_euler_power

```cpp
inline i64 extended_euler_power(const i64 a, const BigInt &exponent, const i64 mod);
inline i64 extended_euler_power(const i64 a, const std::string &exponent, const i64 mod);
```

**扩展**欧拉定理降幂：**不要求 $\gcd(a, \mathrm{mod}) = 1$**。

**行为**
- 有 `assert(mod > 0 && !exponent.zero())`（`BigInt` 重载）/ `assert(mod > 0 && !exponent.empty())`（字符串重载）
- `mod == 1` 时返回 `0`
- 否则取 $\varphi = \varphi(\mathrm{mod})$，计算 $\mathrm{reduced} = \mathrm{exponent} \bmod \varphi$；**若 $\mathrm{exponent} \ge \varphi$ 则 $\mathrm{reduced} \mathrel{+}= \varphi$**（这正是扩展欧拉定理降幂的关键）
- 最终 `power_mod(a, reduced.to_i64(), mod)`

**这是应对「指数巨大且底数与模数不互素」的标准做法**。

## decimal_mod

```cpp
inline i64 decimal_mod(const std::string &number, const i64 mod);
```

逐字符计算十进制字符串所表示的数模 `mod` 的值。内部用 `i128` 累乘，**支持极长的十进制串**（不受 64 位限制）。

**约束**
- **无 `assert`**
- 要求 `mod > 0`
- 字符串只能含 `'0'`–`'9'`：**负号、空白会得到错误结果**（源码未说明）

## decimal_at_least

```cpp
inline bool decimal_at_least(const std::string &number, const i64 value);
```

判断十进制字符串代表的数是否 $\ge$ `value`。先比长度，长度相同再字典序比较。

**约束**
- **无 `assert`**
- 按实现要求 `number` 为**无前导零的非负十进制串**、`value` 非负时，结果才符合数值比较语义

## 使用示例

### 欧拉函数

```cpp
#include "euler.hpp"
#include <iostream>

int main() {
    std::cout << euler_phi(12) << '\n';    // 4
    std::cout << euler_phi(97) << '\n';    // 96

    return 0;
}
```

### 底数与模数互素时降幂

```cpp
#include "euler.hpp"
#include <iostream>

int main() {
    // 2 与 7 互素
    std::cout << euler_power(2, 1000000000, 7) << '\n';

    return 0;
}
```

### 底数与模数不互素（必须用 extended）

```cpp
#include "euler.hpp"
#include <iostream>

int main() {
    // 2 与 4 不互素，euler_power 会断言失败
    std::cout << extended_euler_power(2, std::string("1000000000"), 4) << '\n';

    return 0;
}
```

### 巨大的十进制指数

```cpp
#include "euler.hpp"
#include <iostream>

int main() {
    std::string huge = "1" + std::string(1000, '0');   // 10^1000

    std::cout << decimal_mod(huge, 1000000007) << '\n';
    std::cout << extended_euler_power(3, huge, 1000000007) << '\n';

    return 0;
}
```

### 判断指数是否超过某个阈值

```cpp
// 扩展欧拉定理降幂需要知道「指数是否 >= φ(mod)」
if (decimal_at_least(exponent_string, phi)) {
    // 需要加 φ
}
```

## 前置条件与实现约定

- **文件名叫 `euler` 但内容不是欧拉路径**。找欧拉路径请去 [euler_path.hpp](euler_path.md)。
- **`euler_power` 要求 $\gcd(a, \mathrm{mod}) = 1$**，不满足时断言失败。不互素必须用 `extended_euler_power`。
- **指数必须非负**。三个幂函数都如此；负指数未处理，源码未说明。
- `extended_euler_power` 返回 `i64`，依赖 `BigInt::to_i64()` 的溢出断言——结果必须落在 `i64` 内。
- `decimal_mod` 与 `decimal_at_least` **无断言**，输入不合法（负号、空格、前导零）会静默给出错误结果。
- `extended_euler_power(string)` 不接受空串（断言），且不校验字符串内容的合法性。
- `mod == 1` 时两者行为略有差异：`euler_power` 走 `power_mod` 内部返回 0，`extended_euler_power` 显式返回 0。结果一致。

## 相关文档

- [BigInt](bigint.md) —— 本文件的依赖
- [ModInt](modint.md) —— 常规模运算
- [ExGCD](exgcd.md) —— $\gcd$ 与求逆元
- [EulerPath](euler_path.md) —— **欧拉路径**（名字相近，内容无关）
