#ifndef CRT_HPP
#define CRT_HPP

#include <utility>
#include <vector>

#include "exgcd.hpp"
#include "types.hpp"

// Solve x = remainder (mod modulus); modulus == 0 means the equations conflict.

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
