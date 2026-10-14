# Linear Basis (`linear_basis.hpp`)

`linear_basis.hpp` 实现基于高斯消元的 $\mathbb{F}_2$ 异或线性基 `LinearBasis`，支持动态插入非负 64 位整数、判断某个整数能否由已插入元素异或表出，以及查询给定初值与基中元素异或能得到的最大值。

## 接口说明

```cpp
class LinearBasis {
public:
    void insert(std::int64_t value);
    bool contains(std::int64_t value) const;
    std::int64_t max_xor(std::int64_t value = 0) const;
};
```

- **位宽与值域约定**：内部维护 `max_bit = 62`（即覆盖二进制第 $0 \sim 62$ 位，`basis[63]`），适用于所有**非负有符号 64 位整数** $0 \le \text{value} < 2^{63}$（即 `[0, LLONG_MAX]`）。请勿传入负数，否则符号位不会参与消元且可能影响 `max_xor` 的大小比较。
- `insert(std::int64_t value)`：尝试将 `value` 插入线性基。从最高位 `bit = 62` 到 `0` 依次扫描：若 `value` 的第 `bit` 位为 $1$，且 `basis[bit]` 已存在则令 `value ^= basis[bit]` 继续向低位消元；若 `basis[bit] == 0` 则令 `basis[bit] = value` 并结束插入。
- `contains(std::int64_t value) const`：判断 `value` 是否位于当前线性基张成的异或子空间中。注意由于空子集异或和为 $0$，**对于 `value == 0`，即使线性基为空，`contains(0)` 也恒返回 `true`**；若题目要求非空子集异或出 $0$，需在外部额外记录是否有元素在 `insert` 时被完全消成 $0$（即发生线性相关）。
- `max_xor(std::int64_t value = 0) const`：以 `value` 为初始值，从高位到低位贪心尝试异或 `basis[bit]`，返回能取得的最大异或结果 $\max_{x \in \text{span}} (\text{value} \oplus x)$。默认参数 `value = 0` 即返回子集最大异或和。

## 复杂度

- 令 $B = 63$ 为位宽：
- 单次 `insert`、`contains`、`max_xor`：时间复杂度均为 $O(B)$。
- 空间复杂度：固定占用 63 个 `int64_t`（$O(B)$）。

## 使用示例

```cpp
#include "linear_basis.hpp"
#include <cassert>

int main() {
    LinearBasis lb;
    lb.insert(3);  // 011_2
    lb.insert(5);  // 101_2
    lb.insert(6);  // 110_2 = 3 ^ 5 (线性相关)

    assert(lb.contains(6));
    assert(!lb.contains(1));
    assert(lb.max_xor() == 6);   // 3 ^ 5 = 6
    assert(lb.max_xor(1) == 7);  // 1 ^ 6 = 7
    return 0;
}
```

## 相关文档

- [BinaryTrie](binary_trie.md) —— 支持单元素两两异或最值（非子集异或）的 01 字典树
- [Gaussian](gaussian.md) —— 一般域上的矩阵高斯消元
