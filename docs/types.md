# Types (`types.hpp`)

`types.hpp` 定义了全库通用的基础整数、浮点数、复数类型别名，并提供 `i128`（`__int128_t`）的标准流式输出重载 `operator<<`。

## 接口说明

### 类型别名

```cpp
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;
using Real = double;
using Complex = std::complex<Real>;
```

- `i64` / `u64`：64 位有符号与无符号整数，值域分别为 $[-2^{63}, 2^{63}-1]$ 和 $[0, 2^{64}-1]$。
- `i128` / `u128`：GCC / Clang 扩展的 128 位有符号与无符号整数，仅在 64 位编译环境下可用，常用于大模数乘法防溢出（如 `modular_arithmetic.hpp` 中的 `multiply_mod`）以及高精度中间累加。由于 C++ 标准不支持 128 位整数字面量，超过 64 位的常数需通过计算或类型转换 `(i128)x * y` 构造。
- `Real` / `Complex`：双精度浮点数与复数类型，供计算几何（`geometry_base.hpp`）和快速傅里叶变换（`polynomial.hpp`）统一使用。

### 流式输出

```cpp
inline std::ostream& operator<<(std::ostream& out, const i128 value);
```

- 支持直接通过 `std::cout << value` 输出任意范围内的 `i128` 整数（包含负数与最小值 `-2^127`）。
- 本头文件仅提供 `i128` 的输出重载；若需读入 `i128`，可使用 [`fastio.hpp`](fastio.md) 中的 `OptimizedIO`（其整型模板重载原生支持 `i128` 读入与输出），或先以 `i64` / `std::string` 读入后再转换。

## 复杂度

- 类型转换与算术运算：$O(1)$。
- `operator<<(std::ostream&, i128)`：时间复杂度 $O(\log_{10} |x|)$（最多提取 39 位十进制数字），额外空间复杂度 $O(1)$。

## 使用示例

```cpp
#include "types.hpp"
#include <iostream>

int main() {
    i64 a = 1000000000000000000LL;
    i128 prod = (i128)a * a;
    std::cout << prod << '\n'; // 1000000000000000000000000000000000000

    Complex z(3.0, 4.0);
    std::cout << std::abs(z) << '\n'; // 5
    return 0;
}
```

## 相关文档

- [FastIO](fastio.md) —— 支持 `i128` 直接读入与输出的快速 I/O
- [ModularArithmetic](modular_arithmetic.md) —— 基于 `i128` / `u128` 的安全模乘与快速幂
- [ModInt](modint.md) —— 模意义整数类
