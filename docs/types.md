# types.hpp

全库共用的类型别名与输出支持。几乎所有其他头文件都依赖它。

## 类型别名

```cpp
using i64  = long long;
using u64  = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;
using Real = double;
using Complex = std::complex<Real>;
```

- `Real` 被计算几何（`geometry_base.hpp`）与模拟退火（`simulated_annealing.hpp`）等模块直接使用，**全库精度都取决于它**。需要 `long double` 时只能改这一个别名，但要注意 `Complex` 也随之改变。
- `i64` / `u64` 出现在 `linear_recurrence.hpp`、`rolling_hash.hpp`、`exlucas` 等处。

## 输入输出

```cpp
inline std::ostream &operator<<(std::ostream &out, const i128 value);
```

`__int128_t` 没有标准库的重载，这里补上了输出支持。

**行为**
- `0` 输出 `0`
- 负数先输出 `'-'`，再用无符号的方式取绝对值，因此 `INT128_MIN`（$-2^{127}$）也能正确输出
- 内部用 `char digits[40]` 缓冲，十进制最多 39 位，足够

**没有提供 `operator>>`**：读入 `i128` 需要自己写，或用 `fastio.hpp`。

## 使用示例

```cpp
#include "types.hpp"
#include <iostream>

int main() {
    i128 big = (i128)1e18 * (i128)1e18 % 1000000007;
    std::cout << big << '\n';

    Complex z(1.0, 2.0);
    std::cout << z.real() << ' ' << z.imag() << '\n';

    return 0;
}
```

## 相关文档

- [ModInt](modint.md) —— 依赖本文件
- [Geometry](geometry.md) —— 使用 `Real`、`Complex`
- [BigInt](bigint.md) —— 任意精度整数，`__int128` 不够用时使用
