# ModInt

模运算整数类，自动处理模运算。

## 模板参数

```cpp
template <typename T, T modulo = 0>
struct ModInt;
```

- `T`：整数类型（如 `int`、`long long`）
- `modulo`：模数
  - 若 `modulo > 0`，为编译期常量模数
  - 若 `modulo = 0`，为运行时动态模数

## 类型别名

```cpp
using mint = ModInt<int, 998244353>;  // 常用模数
using mint107 = ModInt<int, 1000000007>;
using dynamic_mint = ModInt<int, 0>;  // 动态模数
```

## 构造函数

```cpp
ModInt<T, mod> x;        // 默认为 0
ModInt<T, mod> x(value); // 从整数构造
```

会自动取模并调整到 $[0, \mathrm{mod})$ 范围。

## 静态成员函数

### mod

```cpp
static T ModInt::mod()
```

返回当前模数。

### set_mod

```cpp
static void ModInt::set_mod(T value)
```

设置动态模数（仅 `modulo = 0` 时可用）。

**约束**
- `value > 0`
- 仅在 `modulo = 0` 时可调用

### norm

```cpp
static T ModInt::norm(T x)
```

将 $x$ 规范化到 $[0, \mathrm{mod})$ 范围。

## 成员函数

### data

```cpp
T x.data()
```

返回内部存储的值（范围 $[0, \mathrm{mod})$）。

## 运算符

### 算术运算

```cpp
ModInt operator+(ModInt rhs)
ModInt operator-(ModInt rhs)
ModInt operator*(ModInt rhs)
ModInt operator/(ModInt rhs)
ModInt operator-()  // 取负
```

所有运算结果自动取模。

**注意**：除法使用费马小定理，**要求模数为质数**。对于合数模数，需使用扩展欧几里得算法。

### 复合赋值

```cpp
ModInt& operator+=(ModInt rhs)
ModInt& operator-=(ModInt rhs)
ModInt& operator*=(ModInt rhs)
ModInt& operator/=(ModInt rhs)
```

### 比较运算

```cpp
bool operator==(ModInt rhs)
bool operator!=(ModInt rhs)
```

### 输入输出

```cpp
std::istream& operator>>(std::istream& in, ModInt& x)
std::ostream& operator<<(std::ostream& out, const ModInt& x)
```

## 辅助函数

### power

```cpp
template <typename S, typename T>
S power(S begin, T base, i64 exp)
```

计算 $\mathrm{base}^{\mathrm{exp}}$，使用快速幂。

- `begin` 是单位元（通常是 `ModInt(1)`）
- 返回 `begin * base^exp`

## 使用示例

### 基本用法

```cpp
#include "modint.hpp"
#include <iostream>

using mint = ModInt<int, 998244353>;

int main() {
    mint a(10);
    mint b(20);
    
    mint c = a + b;
    std::cout << c << std::endl;  // 30
    
    mint d = a * b;
    std::cout << d << std::endl;  // 200
    
    mint e = b / a;
    std::cout << e << std::endl;  // 2
    
    return 0;
}
```

### 动态模数

```cpp
#include "modint.hpp"
#include <iostream>

using mint = ModInt<int, 0>;

int main() {
    int mod;
    std::cin >> mod;
    mint::set_mod(mod);
    
    mint a(10);
    mint b(20);
    std::cout << a + b << std::endl;
    
    return 0;
}
```

### 快速幂

```cpp
#include "modint.hpp"
#include <iostream>

using mint = ModInt<int, 1000000007>;

int main() {
    mint base(2);
    mint result = power(mint(1), base, 10);
    std::cout << result << std::endl;  // 1024
    
    return 0;
}
```

### 组合数计算

```cpp
#include "modint.hpp"
#include <vector>

using mint = ModInt<int, 998244353>;

std::vector<mint> fact, inv_fact;

void precompute(int n) {
    fact.resize(n + 1);
    inv_fact.resize(n + 1);
    
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * mint(i);
    }
    
    inv_fact[n] = power(mint(1), fact[n], mint::mod() - 2);
    for (int i = n - 1; i >= 0; --i) {
        inv_fact[i] = inv_fact[i + 1] * mint(i + 1);
    }
}

mint C(int n, int r) {
    if (r < 0 || r > n) return mint(0);
    return fact[n] * inv_fact[r] * inv_fact[n - r];
}
```

## 常用模数

- **998244353** = $119 \times 2^{23} + 1$（NTT 友好，原根为 3）
- **1000000007** = $10^9 + 7$（常用质数）
- **1000000009** = $10^9 + 9$
- **10007**、**10009**（小模数）

## 注意事项

1. **除法要求模数为质数**：使用费马小定理计算逆元
2. **避免溢出**：中间计算使用 `Wide` 类型（`long long` 或 `__int128_t`）
3. **性能**：编译期常量模数性能更好，因为可以优化取模运算
4. **类型转换**：使用 `x.data()` 获取整数值，或用 `explicit` 转换

## 实现说明

- 使用 `static_assert` 检查类型约束
- `Wide` 类型自动选择以避免溢出
- 编译期 `constexpr` 优化
- 规范化函数确保结果始终非负
