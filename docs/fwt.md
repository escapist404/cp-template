# FWT

给定两组按位状态的系数，计算 XOR、OR 或 AND 意义下的卷积。

## 接口

```cpp
template <typename T>
void fwt_xor(std::vector<T>& a, bool inverse = false);
template <typename T>
void fwt_or(std::vector<T>& a, bool inverse = false);
template <typename T>
void fwt_and(std::vector<T>& a, bool inverse = false);
template <typename T, void (*Transform)(std::vector<T>&, bool)>
std::vector<T> fwt_convolution(std::vector<T> a, std::vector<T> b);
```

`inverse=false` 为正变换，`true` 为逆变换。原地修改数组。卷积按位运算定义：`c[k] = sum(a[i]*b[j])`，其中 `i XOR/OR/AND j = k`。

## 约束与复杂度

- 数组长度为 2 的幂，或为空；卷积两个输入长度必须相同，不会自动补齐。
- `T` 支持加减法；卷积还要求乘法。XOR 逆变换还要求除以长度 `n`。
- 模意义下 XOR 逆变换要求 `n` 可逆，典型用奇素数模 `ModInt`；整数运算中间量不能溢出，逆变换结果应能被 `n` 整除。
- 单次变换与卷积 $O(n\log n)$；变换额外空间 $O(1)$，卷积空间 $O(n)$。

## 使用示例

```cpp
#include "fwt.hpp"
#include <cassert>

int main() {
    auto c = fwt_convolution<long long, fwt_xor<long long>>({1, 2}, {3, 4});
    assert(c[0] == 11 && c[1] == 10);
    auto d = fwt_convolution<long long, fwt_or<long long>>({1, 2}, {3, 4});
    assert(d[0] == 3 && d[1] == 18);
}
```

## 前置条件与实现约定

- OR/AND 逆变换不需要除法，XOR 逆变换最后除以 `n`。
- 不能用普通有符号整数执行可能溢出的变换；系数需要更宽的类型或模整数。
- 空数组的变换、卷积均返回空数组。

公式参考：[KACTL FastSubsetTransform](https://github.com/kth-competitive-programming/kactl/blob/main/content/numerical/FastSubsetTransform.h)。

## 相关文档

- [Polynomial](polynomial.md) —— 普通卷积
- [ModInt](modint.md) —— 模运算类型
