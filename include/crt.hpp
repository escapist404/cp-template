#ifndef CRT_HPP
#define CRT_HPP

#include <utility>
#include <vector>

#include "exgcd.hpp"
#include "types.hpp"

// 【中国剩余定理与扩展中国剩余定理 exCRT (crt.hpp)】
// - 求解同余方程组 x === remainders[i] (mod moduli[i])，支持模数两两【不互质】的一般情况
// - crt(remainders, moduli): 返回 pair{r, m}，表示通解为 x === r (mod m)，其中 0 <= r < m = lcm(moduli)
// - 无解判定: 若同余方程组存在矛盾无解，返回 {0, 0}（即通过检查返回值 second == 0 判定无解）

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
