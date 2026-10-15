#ifndef LAGRANGE_HPP
#define LAGRANGE_HPP

#include <cassert>
#include <vector>

#include "comb.hpp"

// ============================================================================
// 【拉格朗日多项式插值（一般横坐标 O(n^2) 与连续整数横坐标 O(n)）(lagrange.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. template <typename T, T mod>
//      ModInt<T, mod> lagrange_evaluate(
//          const std::vector<ModInt<T, mod>>& x,
//          const std::vector<ModInt<T, mod>>& y,
//          ModInt<T, mod> at)
//      功能：给定 n 个横坐标两两不同（模 mod 下互不同余）的采样点 (x[i], y[i])，
//            在 O(n^2) 时间内直接计算唯一确定的 < n 次多项式 f 在横坐标 at 处的函数值 f(at)。
//   2. template <typename T, T mod>
//      ModInt<T, mod> lagrange_evaluate_consecutive(
//          const std::vector<ModInt<T, mod>>& y,
//          ModInt<T, mod> x)
//      功能：专用于采样横坐标固定为连续整数 0, 1, ..., n - 1（即 y[i] = f(i)）的情形，
//            通过预处理前后缀积与阶乘逆元在【严格线性 O(n) 时间】内求 f(x)。
//      【前置条件】要求采样点数 n < mod 且 mod 为素数（以保证 0! .. (n-1)! 均可逆）。
// ============================================================================

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

// 取值点为连续整数 x = 0, 1, ..., n-1 时的 O(n) 拉格朗日插值，要求 n < mod 以保证阶乘可逆。
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
