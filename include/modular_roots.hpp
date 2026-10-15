#ifndef MODULAR_ROOTS_HPP
#define MODULAR_ROOTS_HPP

#include <algorithm>
#include <cassert>
#include "pollard_rho.hpp"

// 【模意义原根、离散对数与模平方根 Tonelli-Shanks (modular_roots.hpp)】
// - primitive_root(m)       : 求模 m 的最小原根（若 m 不存在原根则返回 -1）
// - discrete_log(a, b, m)   : 求解 a^x === b (mod m) 的最小非负整数解（无解返回 -1）
// - sqrt_mod(a, p)          : Tonelli-Shanks 算法求解奇素数（或 p=2）模意义下 x^2 === a (mod p) 的一个解（无解返回 -1）

inline i64 primitive_root_prime(i64 prime) {
    assert(is_prime_mr(prime));
    if (prime == 2) {
        return 1;
    }
    auto factors = factorize(prime - 1);
    factors.erase(std::unique(factors.begin(), factors.end()), factors.end());
    for (i64 root = 2; root < prime; ++root) {
        bool valid = true;
        for (i64 p : factors) {
            if (power_mod(root, (prime - 1) / p, prime) == 1) {
                valid = false;
                break;
            }
        }
        if (valid) {
            return root;
        }
    }
    return -1;
}

// Tonelli-Shanks. Smaller nonnegative square root, or -1 when none exists.
inline i64 modular_sqrt(i64 value, i64 prime) {
    assert(is_prime_mr(prime));
    value %= prime;
    if (value < 0) {
        value += prime;
    }
    if (!value || prime == 2) {
        return value;
    }
    auto power = [&](u64 a, u64 n) {
        return power_mod(a, n, (u64)prime);
    };
    auto multiply = [&](u64 a, u64 b) {
        return multiply_mod(a, b, (u64)prime);
    };
    if (power(value, (prime - 1) / 2) != 1) {
        return -1;
    }
    u64 odd = prime - 1;
    int exponent = 0;
    while (!(odd & 1)) {
        odd >>= 1;
        ++exponent;
    }
    u64 nonresidue = 2;
    while (power(nonresidue, (prime - 1) / 2) != (u64)prime - 1) {
        ++nonresidue;
    }
    u64 c = power(nonresidue, odd);
    u64 x = power(value, (odd + 1) / 2);
    u64 t = power(value, odd);
    while (t != 1) {
        int i = 0;
        u64 square = t;
        while (square != 1) {
            square = multiply(square, square);
            ++i;
        }
        assert(i < exponent);
        u64 b = power(c, 1ULL << (exponent - i - 1));
        x = multiply(x, b);
        c = multiply(b, b);
        t = multiply(t, c);
        exponent = i;
    }
    return std::min((i64)x, prime - (i64)x);
}
#endif
