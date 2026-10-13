# FormalPowerSeries

`formal_power_series.hpp` 提供基于 NTT 的形式幂级数，底层类型为 `ModInt`。支持多项式逆、除法、求导、积分、对数、指数、非负整数幂和平方根。

## 接口

```cpp
template <typename T, T mod, T root = primitive_root_constexpr(mod)>
class Polynomial;

Self inverse(int n) const;
Self logarithm(int n) const;
Self exponential(int n) const;
Self power(i64 exponent, int n) const;
Self square_root(int n) const;
Self derivative() const;
Self integral() const;
std::pair<Self, Self> divmod(Self divisor) const;
```

返回结果统一截取到前 `n` 项。系数下标为次数，所有操作都在模 `mod` 下进行。

## 前置条件

- 模数通常为支持 NTT 的奇素数，例如 `998244353`；`root` 是原根。
- `inverse(n)` 要求常数项非零。
- `logarithm(n)` 要求常数项为 `1`。
- `exponential(n)` 要求常数项为 `0`。
- `square_root(n)` 要求首个非零项次数为偶数，且对应系数在模数下有平方根。
- `power(e,n)` 要求 `e >= 0`；`e=0` 返回前 `n` 项的常数 1。
- 对数、指数和平方根内部需要除以 `1,2,...`，因此必须保证这些数在模数下可逆。

## 复杂度

底层卷积为 $O(n\log n)$。`inverse`、`logarithm`、`exponential`、`power`、`square_root` 均按倍增实现，复杂度为 $O(n\log n)$，空间为 $O(n)$（不计中间多项式）。

## 使用示例

```cpp
#include "formal_power_series.hpp"
#include <cassert>

int main() {
    using P = Polynomial<int, 998244353>;
    using Z = P::Z;
    P one_plus_x({Z(1), Z(1)});
    P log_f = one_plus_x.logarithm(8);
    P recovered = log_f.exponential(8);
    assert(recovered[0] == Z(1) && recovered[1] == Z(1));

    P square({Z(1), Z(2), Z(1)});
    P root = square.square_root(8);
    P check = (root * root).truncated(3);
    assert(check[0] == Z(1) && check[1] == Z(2) && check[2] == Z(1));
}
```

## 前置条件与实现约定

- 多项式会自动去掉末尾零；需要固定长度时，内部实现会直接补零，调用者读取时不要假设 `size()` 一定等于截断长度。
- `square_root` 使用 Tonelli-Shanks 求常数项平方根；平方根不存在时触发断言。
- 形式幂级数不是实函数，所有“对数、指数、平方根”都按形式级数恒等式定义。
- 输入规模较小时，暴力乘法可能比 NTT 更快；本模板统一复用 `polynomial.hpp` 的卷积。

## 相关文档

- [Polynomial](polynomial.md) —— NTT、FFT 和卷积
- [ModInt](modint.md) —— 模运算系数
