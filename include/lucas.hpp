#ifndef LUCAS_HPP
#define LUCAS_HPP

#include <cassert>
#include <utility>
#include <vector>

#include "crt.hpp"
#include "exgcd.hpp"
#include "modular_arithmetic.hpp"

inline i64 lucas_valuation(i64 n, i64 p) {
	i64 result = 0;
	while (n) result += n /= p;
	return result;
}

inline i64 lucas_choose_prime_power(i64 n, i64 k, i64 p,
		i64 q, i64 exponent_limit) {
	if (k < 0 || k > n) return 0;
	std::vector<i64> prefix(q + 1, 1);
	for (i64 i = 1; i <= q; ++i) {
		prefix[i] = prefix[i - 1];
		if (i % p) prefix[i] = (i64)((i128)(prefix[i]) * i % q);
	}
	auto unit_factorial = [&](auto&& self, i64 x) -> i64 {
		if (!x) return 1;
		const i64 result = power_mod(prefix[q], x / q, q) * prefix[x % q] % q;
		return (i64)((i128)(result) * self(self, x / p) % q);
	};
	const i64 exponent = lucas_valuation(n, p) - lucas_valuation(k, p) - lucas_valuation(n - k, p);
	if (exponent >= exponent_limit) return 0;
	i64 result = unit_factorial(unit_factorial, n);
	result = (i64)((i128)(result) * mod_inverse(unit_factorial(unit_factorial, k), q) % q);
	result = (i64)((i128)(result) * mod_inverse(unit_factorial(unit_factorial, n - k), q) % q);
	return (i64)((i128)(result) * power_mod(p, exponent, q) % q);
}


class Lucas {
public:
	Lucas(i64 prime) : p(prime), fact(prime), ifact(prime) {
		assert(prime > 1);
		fact[0] = ifact[0] = 1;
		for (i64 i = 1; i < p; ++i) fact[i] = fact[i - 1] * i % p;
		ifact[p - 1] = mod_inverse(fact[p - 1], p);
		for (i64 i = p - 1; i > 0; --i) ifact[i - 1] = ifact[i] * i % p;
	}

	i64 choose(i64 n, i64 k) const {
		if (k < 0 || k > n) return 0;
		i64 result = 1;
		while (n || k) {
			const i64 ni = n % p, ki = k % p;
			if (ki > ni) return 0;
			result = result * fact[ni] % p * ifact[ki] % p * ifact[ni - ki] % p;
			n /= p;
			k /= p;
		}
		return result;
	}

private:
	i64 p;
	std::vector<i64> fact, ifact;
};

inline i64 exlucas(i64 n, i64 k, i64 modulus) {
	assert(n >= 0 && modulus > 0);
	if (k < 0 || k > n || modulus == 1) return 0;
	i64 remaining = modulus;
	std::vector<std::pair<i64, i64>> equations;
	for (i64 p = 2; p <= remaining / p; ++p) {
		if (remaining % p) continue;
		i64 q = 1, exponent = 0;
		while (remaining % p == 0) {
			remaining /= p;
			q *= p;
			++exponent;
		}
		equations.emplace_back(lucas_choose_prime_power(n, k, p, q, exponent), q);
	}
	if (remaining > 1) equations.emplace_back(
			lucas_choose_prime_power(n, k, remaining, remaining, 1), remaining);
	return crt(equations).first;
}

#endif
