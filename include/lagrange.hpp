#ifndef LAGRANGE_HPP
#define LAGRANGE_HPP

#include <cassert>
#include <vector>

#include "comb.hpp"

// 【拉格朗日插值 (lagrange.hpp)】
// - lagrange_interpolate(xs, ys, k): 给定 n 个横坐标两两不同的采样点 (xs[i], ys[i])，O(n^2) 计算 n-1 次多项式在 k 处的取值
// - lagrange_interpolate_iota(y, k): 专用于横坐标为连续整数 0, 1, ..., n-1 的情况，通过前后缀积在严格 O(n) 时间内求 f(k)

template <typename T, T mod>
ModInt<T, mod> lagrange_evaluate(
		const std::vector<ModInt<T, mod>>& x,
		const std::vector<ModInt<T, mod>>& y,
		ModInt<T, mod> at) {
	assert(x.size() == y.size());
	const int n = (int)x.size();
	ModInt<T, mod> result;
	for (int i = 0; i < n; ++i) {
		ModInt<T, mod> term = y[i];
		for (int j = 0; j < n; ++j) {
			if (i == j) continue;
			assert(x[i] != x[j]);
			term *= (at - x[j]) / (x[i] - x[j]);
		}
		result += term;
	}
	return result;
}

// Evaluate the polynomial with values y[i] at x = i in O(n).
// This requires n < mod so that all factorials are invertible.
template <typename T, T mod>
ModInt<T, mod> lagrange_evaluate_consecutive(
		const std::vector<ModInt<T, mod>>& y,
		ModInt<T, mod> x) {
	using Z = ModInt<T, mod>;
	const int n = (int)y.size();
	if (n == 0) return Z(0);

	std::vector<Z> prefix(n + 1, Z(1)), suffix(n + 1, Z(1));
	for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] * (x - Z(i));
	for (int i = n - 1; i >= 0; --i) suffix[i] = suffix[i + 1] * (x - Z(i));

	Combinatorics<Z> combinatorics(n);

	Z result;
	for (int i = 0; i < n; ++i) {
		Z basis = prefix[i] * suffix[i + 1] * combinatorics.inverse_factorial(i);
		if ((n - 1 - i) & 1) basis = -basis;
		basis *= combinatorics.inverse_factorial(n - 1 - i);
		result += y[i] * basis;
	}
	return result;
}

#endif
