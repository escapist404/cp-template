#ifndef MODULAR_ROOTS_HPP
#define MODULAR_ROOTS_HPP

#include <algorithm>
#include <cassert>
#include "pollard_rho.hpp"

// ============================================================================
// 【素数模原根与 Tonelli-Shanks 模平方根（二次剩余）(modular_roots.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. inline i64 primitive_root_prime(i64 prime)
//      功能：求素数 prime（要求 prime >= 2 且为素数）的最小正原根 g。
//      实现：调用 Pollard-Rho 分解 prime - 1 的全部质因数 p_i，从小到大枚举 g 检验是否对所有 p_i 均有 g^((prime-1)/p_i) != 1 (mod prime)。
//      返回：最小正原根（如 prime == 2 返回 1，prime == 998244353 返回 3）。复杂度 O(prime^(1/4) + g * log^2(prime))。
//   2. inline i64 modular_sqrt(i64 value, i64 prime)
//      功能：使用 Tonelli-Shanks 算法求解二次同余方程 x^2 === value (mod prime)（要求 prime >= 2 且为素数）。
//      返回值约定：
//        - 若存在解，返回满足 x^2 === value (mod prime) 的一个解 x in [0, prime)（另一解为 (prime - x) % prime）；
//        - 若 value 为模 prime 的二次非剩余（无解），返回【-1】。
//      复杂度：时间复杂度 O(log^2(prime))，空间复杂度 O(1)。
// ============================================================================

// inline i64 primitive_root_prime(i64 prime) — 功能：求素数 prime（要求 prime >= 2 且为素数）的最小正原根 g。
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

// Tonelli-Shanks 模平方根算法：返回较小的非负平方根 x in [0, p)，无解返回 -1。
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
