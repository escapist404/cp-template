#ifndef DETERMINANT_HPP
#define DETERMINANT_HPP

#include <cassert>
#include <utility>

#include "matrix.hpp"

// ============================================================================
// 【Bareiss 无分数消元求方阵行列式 Determinant (determinant.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename T, size_t R, size_t C>
//   T determinant(const Matrix<T, R, C>& input)
// 参数与返回值详解：
//   - input：n x n 方阵（要求 input.rows() == input.columns()，n == 0 时返回单位元 T(1)）。
//   - 算法原理：采用 Bareiss 无分数高斯消元（a[i][j] = (a[i][j] * a[k][k] - a[i][k] * a[k][j]) / previous），
//     在素数模 ModInt、有理数或无溢出的整环 T 上均可直接精确计算方阵行列式 det(input)。
//   - 复杂度：时间复杂度 O(n^3)，空间复杂度 O(n^2)。
// ============================================================================

// template <typename T, size_t R, size_t C> T determinant(const Matrix<T, R, C>& input) — 功能：T determinant(const Matrix<T, R, C>& input)
template <typename T, size_t R, size_t C>
T determinant(const Matrix<T, R, C>& input) {
	assert(input.rows() == input.columns());
	const size_t n = input.rows();
	if (n == 0) return T(1);

	// Bareiss 无分数高斯消元：每步除法均保证整除，中间过程全部保持精确整数。
	auto a = input;
	T previous = T(1);
	bool negated = false;
	for (size_t k = 0; k + 1 < n; ++k) {
		size_t pivot = k;
		while (pivot < n && a[pivot][k] == T(0)) ++pivot;
		if (pivot == n) return T(0);
		if (pivot != k) {
			for (size_t j = k; j < n; ++j) std::swap(a[pivot][j], a[k][j]);
			negated = !negated;
		}
		for (size_t i = k + 1; i < n; ++i) {
			for (size_t j = k + 1; j < n; ++j)
				a[i][j] = (a[i][j] * a[k][k] - a[i][k] * a[k][j]) / previous;
			a[i][k] = T(0);
		}
		previous = a[k][k];
	}
	return negated ? -a[n - 1][n - 1] : a[n - 1][n - 1];
}

#endif
