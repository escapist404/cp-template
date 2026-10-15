#ifndef CRT_HPP
#define CRT_HPP

#include <utility>
#include <vector>

#include "exgcd.hpp"
#include "types.hpp"

// ============================================================================
// 【中国剩余定理与扩展中国剩余定理 CRT / exCRT (crt.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   inline std::pair<i64, i64> crt(const std::vector<std::pair<i64, i64>>& equations)
// 参数与返回值详解：
//   - equations：同余方程组列表，每项 {a_i, m_i} 代表一条同余方程：
//         x === a_i (mod m_i)
//     【不要求模数 m_i 两两互质】（内部使用扩展欧几里得算法逐条合并相容性检验），a_i 可为任意正负整数。
//   - 返回值 std::pair{remainder, modulus}：
//     - 若方程组有解（且所有 m_i > 0），返回 {r, M}，其中 M = lcm(m_1, ..., m_k)，r 为 [0, M) 内的唯一最小非负整数解（空方程组返回 {0, 1}）；
//     - 若存在 m_i <= 0 或方程组互相矛盾无解，返回【{0, 0}】（可通过返回值 second == 0 判定无解）。
//   - 【防溢出注意】合并过程中最小公倍数 M = lcm(m_i) 必须不超过 i64 上限。
// 复杂度：时间复杂度 O(k log(lcm))，空间复杂度 O(1)。
// ============================================================================

inline std::pair<i64, i64> crt(
    const std::vector<std::pair<i64, i64>>& equations) {
    i64 remainder = 0;
    i64 modulus = 1;
    for (const auto& [a, m] : equations) {
        if (m <= 0) return {0, 0};
        i64 x, y;
        const i64 g = extended_gcd(modulus, m, x, y);
        const i64 difference = a - remainder;
        if (difference % g != 0) return {0, 0};
        const i64 next_modulus = modulus / g * m;
        const i64 reduced_modulus = m / g;
        i64 step =
            reduced_modulus == 1
                ? 0
                : (difference / g) % reduced_modulus * x % reduced_modulus;
        if (step < 0) step += reduced_modulus;
        remainder = (remainder + modulus * step) % next_modulus;
        if (remainder < 0) remainder += next_modulus;
        modulus = next_modulus;
    }
    return {remainder, modulus};
}

#endif
