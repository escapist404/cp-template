#ifndef SIMPLEX_HPP
#define SIMPLEX_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <utility>
#include <vector>

#include "matrix.hpp"

// ============================================================================
// 【两阶段单纯形法求解线性规划 Two-Phase Simplex (simplex.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename T, size_t R, size_t C>
//   std::pair<T, std::vector<T>> simplex(const Matrix<T, R, C>& A,
//                                        const std::vector<T>& b,
//                                        const std::vector<T>& c)
// 标准型定义与参数说明：
//   在约束 A * x <= b 且 x >= 0 下，最大化目标函数 c^T * x = sum_{j=0}^{n-1} c[j] * x[j]。
//   - A：m 行 n 列约束矩阵（要求 A.rows() == b.size() 且 A.columns() == c.size()）；
//   - b：长度为 m 的右端常数向量（允许含负数，自动通过第一阶段辅助规划找初始可行基）；
//   - c：长度为 n 的目标函数系数向量。
// 返回值 std::pair{optimal_value, solution}（设 infinity = numeric_limits<T>::max() / 4）：
//   1. 存在有限最优解：返回 {最优目标值, 长度为 n 的最优解向量 x}；
//   2. 无可行解 (Infeasible)：返回 {-infinity, 全 0 向量}；
//   3. 目标无界 (Unbounded)：返回 {+infinity, 全 0 向量}。采用 Bland 规则防退化循环。
// ============================================================================

// template <typename T, size_t R, size_t C> std::pair<T, std::vector<T>> simplex(const Matrix<T, R, C>& A, — 功能：const std::vector<T>& b,
template <typename T, size_t R, size_t C>
std::pair<T, std::vector<T>> simplex(const Matrix<T, R, C>& A,
		const std::vector<T>& b, const std::vector<T>& c) {
	const T epsilon = (T)1e-9;
	const T infinity = std::numeric_limits<T>::max() / 4;
	const int m = b.size(), n = c.size();
	assert((int)A.rows() == m && (int)A.columns() == n);
	std::vector<int> basis(m), nonbasis(n + 1);
	Matrix<T, 0, 0> table(m + 2, n + 2);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) table[i][j] = A[i][j];
		basis[i] = n + i;
		table[i][n] = -1;
		table[i][n + 1] = b[i];
	}
	for (int j = 0; j < n; ++j) nonbasis[j] = j;
	nonbasis[n] = -1;
	for (int j = 0; j < n; ++j) table[m][j] = -c[j];
	table[m + 1][n] = 1;

	auto pivot = [&](int row, int column) {
		const T inverse = 1 / table[row][column];
		for (int i = 0; i < m + 2; ++i) if (i != row)
			for (int j = 0; j < n + 2; ++j) if (j != column)
				table[i][j] -= table[row][j] * table[i][column] * inverse;
		for (int j = 0; j < n + 2; ++j) if (j != column) table[row][j] *= inverse;
		for (int i = 0; i < m + 2; ++i) if (i != row) table[i][column] *= -inverse;
		table[row][column] = inverse;
		std::swap(basis[row], nonbasis[column]);
	};
	auto run = [&](int phase) {
		const int objective = phase == 1 ? m + 1 : m;
		while (true) {
			int column = -1;
			for (int j = 0; j <= n; ++j) {
				if (phase == 2 && nonbasis[j] == -1) continue;
				if (column == -1 || table[objective][j] < table[objective][column] - epsilon ||
					(std::fabs(table[objective][j] - table[objective][column]) <= epsilon &&
					 nonbasis[j] < nonbasis[column])) column = j;
			}
			if (column == -1 || table[objective][column] >= -epsilon) return true;
			int row = -1;
			for (int i = 0; i < m; ++i) if (table[i][column] > epsilon) {
				if (row == -1) row = i;
				else {
					const T lhs = table[i][n + 1] / table[i][column];
					const T rhs = table[row][n + 1] / table[row][column];
					if (lhs < rhs - epsilon ||
						(std::fabs(lhs - rhs) <= epsilon && basis[i] < basis[row])) row = i;
				}
			}
			if (row == -1) return false;
			pivot(row, column);
		}
	};

	std::vector<T> x(n);
	if (!m) {
		for (int j = 0; j < n; ++j) if (table[m][j] < -epsilon)
			return {infinity, std::move(x)};
		return {T(), std::move(x)};
	}
	int row = 0;
	for (int i = 1; i < m; ++i) if (table[i][n + 1] < table[row][n + 1]) row = i;
	if (table[row][n + 1] < -epsilon) {
		pivot(row, n);
		if (!run(1) || table[m + 1][n + 1] < -epsilon ||
			std::fabs(table[m + 1][n + 1]) > epsilon) return {-infinity, std::move(x)};
		for (int i = 0; i < m; ++i) if (basis[i] == -1) {
			int column = 0;
			for (int j = 1; j <= n; ++j)
				if (table[i][j] < table[i][column] - epsilon) column = j;
			pivot(i, column);
		}
	}
	if (!run(2)) return {infinity, std::move(x)};
	for (int i = 0; i < m; ++i) if (basis[i] < n) x[basis[i]] = table[i][n + 1];
	return {table[m][n + 1], std::move(x)};
}

#endif
