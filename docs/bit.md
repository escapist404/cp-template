# Bit Utilities (`bit.hpp`)

`bit.hpp` 提供常用的整数位运算、二次幂上下取整、对数上下取整、精确整数平方根以及位翻转函数。所有函数均通过 `static_assert` 限制参数 `T` 必须为非 `bool` 的整数类型。

## 接口说明

### 类型辅助与二次幂运算

```cpp
template <typename T>
using unsigned_integer_t = std::make_unsigned_t<T>;

template <typename T> constexpr T lowbit(T value);
template <typename T> constexpr T highbit(T value);
template <typename T> constexpr T floor_pow2(T value);
template <typename T> constexpr T ceil_pow2(T value);
template <typename T> constexpr bool is_pow2(T value);
```

- `lowbit(value)`：返回 `value` 二进制表示中最低位的 $1$ 所对应的值（即 `x & -x`，在无符号视角下计算以避免有符号溢出）；当 `value == 0` 时返回 `0`。
- `highbit(value)` / `floor_pow2(value)`：要求 `value >= 0`。返回不超过 `value` 的最大二次幂；当 `value == 0` 时返回 `0`。
- `ceil_pow2(value)`：要求 `value >= 0`。返回不小于 `value` 的最小二次幂；注意当 `value == 0` 或 `value == 1` 时均返回 `1`。调用时需确保结果不超过类型 `T` 的最大值，否则左移会发生溢出。
- `is_pow2(value)`：判断 `value` 在无符号表示下是否为严格正的二次幂（`value == 0` 返回 `false`；有符号负数会被转换为对应的无符号数判定，因此一般仅对非负整数调用）。

### 对数与位宽

```cpp
template <typename T> constexpr int floor_log2(T value);
template <typename T> constexpr int ceil_log2(T value);
template <typename T> constexpr int bit_width(T value);
```

- `floor_log2(value)`：**要求 `value > 0`**（传入 `0` 会触发 `assert`）。返回 $\lfloor \log_2(\text{value}) \rfloor$。
- `ceil_log2(value)`：要求 `value >= 0`。返回满足 $2^k \ge \text{value}$ 的最小非负整数 $k$；当 `value <= 1` 时返回 `0`。
- `bit_width(value)`：要求 `value >= 0`。返回表示 `value` 所需的二进制位数：`value == 0` 时返回 `0`，`value > 0` 时返回 `floor_log2(value) + 1`。

### 整数平方根与位统计

```cpp
template <typename T> T floor_sqrt(T value);
template <typename T> T ceil_sqrt(T value);
template <typename T> constexpr int popcount(T value);
template <typename T> constexpr T bit_reverse(T value, int width);
```

- `floor_sqrt(value)` / `ceil_sqrt(value)`：要求 `value >= 0`。先用 `std::sqrt((long double)value)` 估计初值，再用整数除法修正误差，保证对 64 位整数（`i64` / `u64`）也完全精确无浮点舍入误差。注意 `ceil_sqrt` 内部执行 `result * result == value` 判定，因此 `value` 不应超过 $\lfloor\sqrt{\max(T)}\rfloor^2$。
- `popcount(value)`：要求 `sizeof(T) <= sizeof(unsigned long long)`（不支持 128 位整数）。返回无符号二进制表示中 $1$ 的个数。
- `bit_reverse(value, width)`：要求 `0 <= width` 且不超过无符号类型 `U` 的总位数。将 `value` 的低 `width` 位按二进制前后翻转，高出 `width` 的位会被丢弃。

## 复杂度

- `lowbit`、`is_pow2`、`popcount`：$O(1)$。
- `highbit`、`floor_pow2`、`ceil_pow2`、`floor_log2`、`ceil_log2`、`bit_width`、`bit_reverse`：$O(w)$，其中 $w$ 为数值的有效二进制位宽（至多 64 或 128）。
- `floor_sqrt`、`ceil_sqrt`：$O(1)$。

## 使用示例

```cpp
#include "bit.hpp"
#include <cassert>

int main() {
    assert(lowbit(12) == 4);
    assert(highbit(12) == 8);
    assert(ceil_pow2(12) == 16);
    assert(floor_log2(12) == 3);
    assert(ceil_log2(12) == 4);
    assert(bit_width(12) == 4);
    assert(is_pow2(8) && !is_pow2(12));
    assert(floor_sqrt(15LL) == 3 && ceil_sqrt(15LL) == 4);
    assert(popcount(0b101101) == 4);
    assert(bit_reverse(0b0011, 4) == 0b1100);
    return 0;
}
```

## 相关文档

- [SegmentTree](segtree.md) —— 依赖 `ceil_pow2` 与 `floor_log2` 建树
- [SparseTable](sparse_table.md) —— 依赖 `floor_log2` 与 `ceil_pow2` 预处理区间
