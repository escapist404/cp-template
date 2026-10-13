# BigInt

大整数类，支持任意精度的有符号整数运算。

依赖 `types.hpp`（`i64`、`u64`）。

## 类型与构造

```cpp
class BigInt;

BigInt(i64 value = 0);
BigInt(const std::string& value);
```

字符串可以带前缀 `+` 或 `-`。

## 运算

```cpp
BigInt operator+(BigInt lhs, const BigInt& rhs);
BigInt operator-(BigInt lhs, const BigInt& rhs);
BigInt operator*(BigInt lhs, const BigInt& rhs);
BigInt operator*(BigInt lhs, i64 rhs);
BigInt operator/(const BigInt& lhs, const BigInt& rhs);
BigInt operator%(const BigInt& lhs, const BigInt& rhs);
BigInt& operator+=(const BigInt& rhs);
BigInt& operator-=(const BigInt& rhs);
BigInt& operator*=(const BigInt& rhs);
BigInt& operator*=(i64 rhs);
```

除法和取模的结果遵循整数除法规则；除数不能为 0。

## 比较与辅助函数

```cpp
bool zero() const;
BigInt abs() const;
i64 to_i64() const;
std::pair<BigInt, BigInt> divmod(const BigInt& dividend,
                                 const BigInt& divisor);
```

`to_i64` 在结果超出 `i64` 范围时触发断言。

## IO

```cpp
std::ostream& operator<<(std::ostream&, const BigInt&);
std::istream& operator>>(std::istream&, BigInt&);
```

## 使用示例

```cpp
#include "bigint.hpp"
#include <iostream>

int main() {
    BigInt a("-123456789012345678901234567890");
    BigInt b("987654321098765432109876543210");

    std::cout << a + b << '\n';
    std::cout << a * b << '\n';

    BigInt x;
    std::cin >> x;
}
```

## 相关文档

- [ModInt](modint.md) —— 模意义整数
- [Euler](euler.md) —— 用到 `BigInt` 的欧拉降幂
- [FastIO](fastio.md) —— 快速 IO
