# Bit Utilities

常用的整数位运算小函数，使用 C++17 实现。

```cpp
#include "bit.hpp"
```

## 接口

```cpp
lowbit(x);       // 最低位的 1，x = 0 时返回 0
highbit(x);      // 最高位的 1，x = 0 时返回 0
floor_pow2(x);   // 不大于 x 的最大二次幂
ceil_pow2(x);    // 不小于 x 的最小二次幂，ceil_pow2(0) == 1
floor_log2(x);   // x > 0 时，最高位 1 的位置
bit_width(x);    // x 的二进制位数，bit_width(0) == 0
ceil_log2(x);    // 满足 2^k >= x 的最小 k
is_pow2(x);      // 判断 x 是否为正二次幂
popcount(x);     // 统计二进制 1 的个数
bit_reverse(x, width); // 反转低 width 位
floor_sqrt(x);         // floor(sqrt(x))
ceil_sqrt(x);          // ceil(sqrt(x))
```

`floor_log2` 要求 `x > 0`；`highbit`、`floor_pow2`、`ceil_pow2`、`bit_width` 和 `ceil_log2` 要求 `x >= 0`。位反转建议使用无符号整数，`width` 不能超过整数类型的位数。

这些函数主要用于线段树大小、树状数组跳跃、倍增表层数、整数对数和位逆序排列。
