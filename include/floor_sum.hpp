#ifndef FLOOR_SUM_HPP
#define FLOOR_SUM_HPP

#include <cassert>
#include <limits>
#include <utility>

#include "types.hpp"

// ============================================================================
// 【类欧几里得算法 Floor Sum (floor_sum.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   inline i64 floor_sum(i64 l, i64 r, i64 m, i64 a, i64 b)
// 参数与返回值详解（【注意参数个数与顺序】共 5 个参数：左边界 l, 右边界 r, 除数 m, 一次项系数 a, 常数项 b）：
//   - 功能：计算半开整数区间 i in [l, r) 上的下取整线性函数求和：
//         sum_{i = l}^{r - 1} floor((a * i + b) / m)
//   - 参数约束：要求 0 <= l <= r 且 m > 0；a, b 可为任意正负有符号 64 位整数（内部自动向负无穷方向规范化负余数）。
//   - 若需计算常见前缀形式 sum_{i = 0}^{n - 1} floor((a * i + b) / m)，请调用 `floor_sum(0, n, m, a, b)`。
// 复杂度：时间复杂度 O(log m)，空间复杂度 O(1)。
// ============================================================================

// inline i64 floor_sum(i64 l, i64 r, i64 m, i64 a, i64 b) — 功能：参数与返回值详解（【注意参数个数与顺序】共 5 个参数：左边界 l, 右边界 r, 除数 m, 一次项系数 a, 常数项 b）：
inline i64 floor_sum(i64 l, i64 r, i64 m, i64 a, i64 b) {
    assert(0 <= l && l <= r && m > 0);
    auto prefix = [&](i64 n) {
        i128 count = n, mod = m, slope = a, offset = b, result = 0;
        auto normalize = [&](auto& value, i128 weight) {
            i128 quotient = value / mod;
            if (value % mod < 0) --quotient;
            result += quotient * weight;
            value -= quotient * mod;
        };
        normalize(slope, count * (count - 1) / 2);
        normalize(offset, count);
        while (true) {
            result += (slope / mod) * (count * (count - 1) / 2);
            slope %= mod;
            result += (offset / mod) * count;
            offset %= mod;
            i128 top = slope * count + offset;
            if (top < mod) break;
            count = top / mod;
            offset = top % mod;
            std::swap(slope, mod);
        }
        return result;
    };
    i128 result = prefix(r) - prefix(l);
    assert(result >= std::numeric_limits<i64>::min() &&
            result <= std::numeric_limits<i64>::max());
    return (i64)result;
}

#endif
