# CRT

给定若干条同余方程，求它们是否有公共解；有解时返回规范化后的解和解的模数。

## 接口

```cpp
inline std::pair<i64, i64> crt(const std::vector<std::pair<i64, i64>> &equations);
```

依赖 `exgcd.hpp`、`types.hpp`。

## 参数

每个元素是一个 `{remainder, modulus}` 序对，表示

$$x \equiv \mathrm{remainder} \pmod{\mathrm{modulus}}$$

**注意顺序**：余数在前，模数在后。

## 返回值

返回 `{remainder, modulus}`。

- 有解时：`modulus > 0`，`remainder` 已规范化到 $[0, \mathrm{modulus})$，即 $x \equiv \mathrm{remainder} \pmod{\mathrm{modulus}}$
- **无解时：`modulus == 0`**（第二个分量作为失败哨兵）
- 空输入返回 `{0, 1}`

## 使用示例

```cpp
#include "crt.hpp"
#include <iostream>

int main() {
    // x ≡ 2 (mod 3), x ≡ 3 (mod 5), x ≡ 2 (mod 7)
    std::vector<std::pair<i64, i64>> eq = {{2, 3}, {3, 5}, {2, 7}};
    auto [remainder, modulus] = crt(eq);

    if (modulus == 0) {
        std::cout << "no solution\n";
    } else {
        std::cout << remainder << " mod " << modulus << '\n';  // 23 mod 105
    }

    return 0;
}
```

### 模数不互质的情形

```cpp
// x ≡ 1 (mod 4), x ≡ 3 (mod 6)
std::vector<std::pair<i64, i64>> eq = {{1, 4}, {3, 6}};
auto [r, m] = crt(eq);   // 都满足 x ≡ 7 (mod 12)
```

互质与不互质都走同一条逐个合并的路径。

### 无解判定

```cpp
std::vector<std::pair<i64, i64>> eq = {{0, 4}, {1, 6}};
// x ≡ 0 (mod 4) 说明 x 是偶数，x ≡ 1 (mod 6) 说明 x 是奇数 → 矛盾
auto [r, m] = crt(eq);
// m == 0
```

## 前置条件与实现约定

- **必须检查返回的 `modulus` 是否为 0** 才能判断有解，光看 `remainder` 无法区分「解是 0」与「无解」。
- 任一 `modulus <= 0` 会**立即整体返回 `{0, 0}`**（源码 `if (m <= 0) return {0, 0};`），不是跳过该方程。
- 结果被规范化到 $[0, \mathrm{modulus})$，若题目要求最小正整数解，需自行处理 `remainder == 0` 的边界（此时最小正解是 `modulus`）。
- **中间乘法全部用 `i64`，没有 `i128` 保护**。当各模数乘积很大（例如接近或超过 $2^{63}$）时存在溢出风险。模数量级较大时，建议自己用 `__int128` 重写合并步骤。
- 合并后的模数是各模数的 LCM，可能很大；即使每个模数都不溢出，LCM 也可能溢出。

## 相关文档

- [ExGCD](exgcd.md) —— 本文件的基础，求逆元与判定可解性
- [Lucas](lucas.md) —— `exlucas` 用 CRT 合并各素因子幂的结果
- [ModInt](modint.md) —— 单一模数下的模运算
