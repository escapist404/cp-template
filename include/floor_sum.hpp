#ifndef FLOOR_SUM_HPP
#define FLOOR_SUM_HPP

#include <cassert>
#include <limits>
#include <utility>

#include "types.hpp"

// 【类欧几里得算法 Floor Sum (floor_sum.hpp)】
// - floor_sum(n, m, a, b): 在 O(log m) 时间内计算 sum_{i = 0}^{n - 1} floor((a * i + b) / m)
// - 支持任意有符号整数 a, b（内部自动对负数系数规范化取模），要求 n >= 0, m > 0

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
