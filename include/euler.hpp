#ifndef EULER_HPP
#define EULER_HPP

#include <cassert>
#include <numeric>
#include <string>

#include "bigint.hpp"
#include "modular_arithmetic.hpp"

// Euler totient phi(n) in O(sqrt(n)) and linear sieve phi table on [0, n].

inline i64 euler_phi(i64 n) {
	assert(n > 0);
	i64 result = n;
	for (i64 p = 2; p <= n / p; ++p) {
		if (n % p != 0) continue;
		result = result / p * (p - 1);
		while (n % p == 0) n /= p;
	}
	if (n > 1) result = result / n * (n - 1);
	return result;
}

inline i64 euler_power(i64 a, i64 exponent, i64 mod) {
	assert(mod > 0 && exponent >= 0 && std::gcd(a, mod) == 1);
	return power_mod(a, exponent % euler_phi(mod), mod);
}

inline i64 decimal_mod(const std::string& number, i64 mod) {
	i64 result = 0;
	for (const char digit : number) {
		result = (i64)(((i128)(result) * 10 + digit - '0') % mod);
	}
	return result;
}

inline bool decimal_at_least(const std::string& number, i64 value) {
	const std::string bound = std::to_string(value);
	if (number.size() != bound.size()) return number.size() > bound.size();
	return number >= bound;
}

inline i64 extended_euler_power(i64 a, const BigInt& exponent,
		i64 mod) {
	assert(mod > 0 && !exponent.zero());
	if (mod == 1) return 0;
	const BigInt phi = euler_phi(mod);
	BigInt reduced = exponent % phi;
	if (exponent >= phi) reduced += phi;
	return power_mod(a, reduced.to_i64(), mod);
}

inline i64 extended_euler_power(i64 a, const std::string& exponent,
		i64 mod) {
	assert(mod > 0 && !exponent.empty());
	return extended_euler_power(a, BigInt(exponent), mod);
}

#endif
