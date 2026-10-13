# Polynomial primitives

`polynomial.hpp` 提供底层的 NTT、FFT 和卷积函数，不定义 `Polynomial` 类。需要多项式对象以及逆、对数、指数、幂和平方根时，请使用 [`formal_power_series.hpp`](formal_power_series.md) 中的 `Polynomial<T, mod, root>`。

## NTT（模整数）

```cpp
template <typename T, T mod, T root = primitive_root_constexpr(mod)>
void ntt(std::vector<ModInt<T, mod>>& a, bool inverse = false);

template <typename T, T mod, T root = primitive_root_constexpr(mod)>
std::vector<ModInt<T, mod>> convolution(
    const std::vector<ModInt<T, mod>>& a,
    const std::vector<ModInt<T, mod>>& b);

```

`ntt` 原地变换；`inverse = true` 时执行逆变换。`convolution` 返回长度为 `a.size() + b.size() - 1` 的系数，并要求模数支持相应的 NTT 长度（例如 `998244353`）。

直接调用 `ntt` 时，向量长度必须是非零的二次幂，并整除素数模数 `mod - 1`；`root` 必须是该模数的原根。卷积接口会自动补齐变换长度，但调用者仍须保证模数支持这个长度。

## FFT（复数）

```cpp
void fft(std::vector<Complex>& a, bool inverse = false);
std::vector<i64> convolution(
    const std::vector<i64>& a, const std::vector<i64>& b);
```

`fft` 是原地变换。整数卷积使用复数 FFT，结果通过四舍五入转换为 `i64`；输入系数和中间结果应留在浮点精度可承受的范围内。其他整数类型可以直接调用同名模板重载，它们会先转换为 `i64`。

直接调用 `fft` 时，向量长度必须是非零的二次幂。

## 使用示例

```cpp
#include "polynomial.hpp"
#include "modint.hpp"

using mint = ModInt<int, 998244353>;

int main() {
    std::vector<mint> a{1, 2, 3};
    std::vector<mint> b{4, 5};
    auto c = convolution(a, b); // 4 + 13x + 22x^2 + 15x^3
}
```

## 复杂度与约定

- NTT、FFT 和卷积的复杂度为 $O(n\log n)$，其中 `n` 是覆盖结果长度的二次幂。
- 空输入的卷积返回空向量。
- 系数下标从 `0` 开始；卷积结果不会自动去除末尾零。
- 形式幂级数的高层接口见 [`formal_power_series.md`](formal_power_series.md)。

## 相关文档

- [ModInt](modint.md) —— 模运算系数
- [FormalPowerSeries](formal_power_series.md) —— 多项式类和高层运算
- [Lagrange](lagrange.md) —— Lagrange 插值
