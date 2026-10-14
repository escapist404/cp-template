#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <algorithm>
#include <cmath>
#include <complex>
#include <type_traits>
#include <vector>

#include "modint.hpp"
#include "modular_arithmetic.hpp"
#include "types.hpp"
#include "bit.hpp"

// NTT (for ModInt) and FFT (for Complex/i64) polynomial convolutions in O(n log n).

template <typename T>
constexpr T polynomial_primitive_root(T mod) {
	if (mod == 2) {
		return 1;
	}

	T factors[32] = {};
	int count = 0;
	T phi = mod - 1;
	T x = phi / 2;
	factors[count++] = 2;
	for (T p = 3; p <= x / p; p += 2) {
		if (x % p != 0) {
			continue;
		}
		factors[count++] = p;
		while (x % p == 0) {
			x /= p;
		}
	}
	if (x > 1) {
		factors[count++] = x;
	}

	for (T g = 2;; ++g) {
		bool ok = true;
		for (int i = 0; i < count; ++i) {
			if (power_mod(g, phi / factors[i], mod) == 1) {
				ok = false;
			}
		}
		if (ok) {
			return g;
		}
	}
}

template <typename T, T mod, T root>
void polynomial_transform(std::vector<ModInt<T, mod>>& a, bool inverse) {
	using Z = ModInt<T, mod>;
	const int n = (int)(a.size());

	for (int i = 1, j = 0; i < n; ++i) {
		int bit = n >> 1;
		for (; j & bit; bit >>= 1) {
			j ^= bit;
		}
		j ^= bit;
		if (i < j) {
			std::swap(a[i], a[j]);
		}
	}

	for (int len = 2; len <= n; len <<= 1) {
		Z wroot = power(Z(1), Z(root), (mod - 1) / len);
		if (inverse) {
			wroot = power(Z(1), wroot, mod - 2);
		}
		for (int i = 0; i < n; i += len) {
			Z w(1);
			for (int j = 0; j < len / 2; ++j) {
				const Z u = a[i + j];
				const Z v = a[i + j + len / 2] * w;
				a[i + j] = u + v;
				a[i + j + len / 2] = u - v;
				w *= wroot;
			}
		}
	}

	if (inverse) {
		const Z inv_n = power(Z(1), Z(n), mod - 2);
		for (auto& x : a) {
			x *= inv_n;
		}
	}
}


template <typename T>
constexpr T primitive_root_constexpr(T mod) {
	return polynomial_primitive_root(mod);
}

template <typename T, T mod, T root = primitive_root_constexpr(mod)>
void ntt(std::vector<ModInt<T, mod>>& a, bool inverse = false) {
	polynomial_transform<T, mod, root>(a, inverse);
}

template <typename T, T mod, T root = primitive_root_constexpr(mod)>
std::vector<ModInt<T, mod>> convolution(
		const std::vector<ModInt<T, mod>>& a,
		const std::vector<ModInt<T, mod>>& b) {
	if (a.empty() || b.empty()) {
		return {};
	}

	int n = ceil_pow2((int)(a.size() + b.size() - 1));
	std::vector<ModInt<T, mod>> x(a.begin(), a.end()), y(b.begin(), b.end());
	x.resize(n);
	y.resize(n);

	polynomial_transform<T, mod, root>(x, false);
	polynomial_transform<T, mod, root>(y, false);

	for (int i = 0; i < n; ++i) {
		x[i] *= y[i];
	}
	polynomial_transform<T, mod, root>(x, true);
	x.resize(a.size() + b.size() - 1);
	return x;
}

inline void fft(std::vector<Complex>& a, bool inverse = false) {
	const int n = (int)(a.size());

	for (int i = 1, j = 0; i < n; ++i) {
		int bit = n >> 1;
		for (; j & bit; bit >>= 1) {
			j ^= bit;
		}
		j ^= bit;
		if (i < j) {
			std::swap(a[i], a[j]);
		}
	}

	const Real PI = std::acos(-1.0);
	for (int len = 2; len <= n; len <<= 1) {
		const Real angle = (inverse ? -2 : 2) * PI / len;
		const Complex root(std::cos(angle), std::sin(angle));
		for (int i = 0; i < n; i += len) {
			Complex w(1);
			for (int j = 0; j < len / 2; ++j) {
				const Complex u = a[i + j];
				const Complex v = a[i + j + len / 2] * w;
				a[i + j] = u + v;
				a[i + j + len / 2] = u - v;
				w *= root;
			}
		}
	}

	if (inverse) {
		for (auto& x : a) {
			x /= n;
		}
	}
}

inline std::vector<i64> convolution(
		const std::vector<i64>& a, const std::vector<i64>& b) {
	if (a.empty() || b.empty()) {
		return {};
	}

	int n = ceil_pow2((int)(a.size() + b.size() - 1));
	std::vector<Complex> x(a.begin(), a.end()), y(b.begin(), b.end());
	x.resize(n);
	y.resize(n);

	fft(x);
	fft(y);
	for (int i = 0; i < n; ++i) {
		x[i] *= y[i];
	}
	fft(x, true);

	std::vector<i64> result(a.size() + b.size() - 1);
	for (int i = 0; i < (int)(result.size()); ++i) {
		result[i] = std::llround(x[i].real());
	}
	return result;
}

template <typename Int,
		std::enable_if_t<std::is_integral<Int>::value && !std::is_same<Int, i64>::value, int> = 0>
std::vector<i64> convolution(const std::vector<Int>& a, const std::vector<Int>& b) {
	return convolution(std::vector<i64>(a.begin(), a.end()),
			std::vector<i64>(b.begin(), b.end()));
}


#endif
