#ifndef GAUSSIAN_HPP
#define GAUSSIAN_HPP

#include <cassert>
#include <cmath>
#include <type_traits>
#include <vector>

#include "matrix.hpp"

// 【高斯消元求解线性方程组与矩阵求秩 (gaussian.hpp)】
// - gaussian_elimination(A, b, is_zero): O(n^2 m) 求解线性方程组 A * x = b，返回 GaussianResult<T>{consistent, rank, solution, basis}
//   - consistent: 方程组是否有解
//   - rank      : 系数矩阵的秩
//   - solution  : 一组特解 x_0（自由变量取 0）
//   - basis     : 齐次方程组 A * x = 0 的零空间基向量列表（维数 = m - rank，通解为 solution + sum(c_i * basis[i])）
// - matrix_rank(vec, n, m, is_zero): 仅求 n x m 行优先矩阵的秩（支持 n=0 或 m=0，不分配解空间）

template <typename T>
struct GaussianZero {
    bool operator()(T value) const {
        if constexpr (std::is_floating_point<T>::value) {
            return std::abs(value) <= T(1E-10);
        } else {
            return value == T{};
        }
    }
};

// Rank of an n by m matrix over a field. The matrix is row-major. This
// rank-only form avoids the solution/nullspace allocations of the solver.
template <typename T, typename IsZero = GaussianZero<T>>
int matrix_rank(std::vector<T> matrix, int n, int m, IsZero is_zero = IsZero{}) {
	assert(n >= 0 && m >= 0 && (long long)n * m == (long long)matrix.size());
	if (n == 0 || m == 0) return 0;
	Matrix<T, 0, 0> result(n, m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			result[i][j] = std::move(matrix[(long long)i * m + j]);
	return result.rank(is_zero);
}

template <typename T>
struct GaussianResult {
    bool consistent = true;
    int rank = 0;
    std::vector<T> particular;
    std::vector<std::vector<T>> nullspace;
};

// Solve Ax=b over a field: float/double or prime-modulus ModInt, not integers.
// Each row of augmented contains variables+1 entries. No equations is allowed
// by supplying variables explicitly. All solutions = particular + span(nullspace).
template <typename T, size_t R, size_t C, typename IsZero = GaussianZero<T>>
GaussianResult<T> gaussian_elimination(Matrix<T, R, C> augmented,
                                      int variables, IsZero is_zero = IsZero{}) {
    assert(variables >= 0);
	assert((int)augmented.columns() == variables + 1);
	int n = augmented.rows();
    std::vector<int> pivot(variables, -1);
    int rank = 0;
    for (int col = 0; col < variables && rank < n; ++col) {
        int selected = -1;
        for (int i = rank; i < n; ++i) {
            if (!is_zero(augmented[i][col])) {
                if (selected == -1) {
                    selected = i;
                } else if constexpr (std::is_floating_point<T>::value) {
                    if (std::abs(augmented[i][col]) > std::abs(augmented[selected][col])) {
                        selected = i;
                    }
                }
            }
        }
        if (selected == -1) {
            continue;
        }
		for (int j = 0; j <= variables; ++j)
			std::swap(augmented[selected][j], augmented[rank][j]);
        T scale = T(1) / augmented[rank][col];
        for (int j = col; j <= variables; ++j) {
            augmented[rank][j] *= scale;
        }
        for (int i = 0; i < n; ++i) {
            if (i != rank && !is_zero(augmented[i][col])) {
                T factor = augmented[i][col];
                for (int j = col; j <= variables; ++j) {
                    augmented[i][j] -= factor * augmented[rank][j];
                }
            }
        }
        pivot[col] = rank++;
    }
    GaussianResult<T> result;
    result.rank = rank;
    for (int i = rank; i < n; ++i) {
        if (!is_zero(augmented[i][variables])) {
            result.consistent = false;
            return result;
        }
    }
    result.particular.resize(variables);
    for (int col = 0; col < variables; ++col) {
        if (pivot[col] != -1) {
            result.particular[col] = augmented[pivot[col]][variables];
        }
    }
    for (int free = 0; free < variables; ++free) {
        if (pivot[free] == -1) {
            std::vector<T> basis(variables);
            basis[free] = T(1);
            for (int col = 0; col < variables; ++col) {
                if (pivot[col] != -1) {
                    basis[col] = -augmented[pivot[col]][free];
                }
            }
            result.nullspace.push_back(std::move(basis));
        }
    }
    return result;
}

template <typename T, typename IsZero = GaussianZero<T>>
GaussianResult<T> gaussian_elimination(std::vector<std::vector<T>> augmented,
                                      int variables, IsZero is_zero = IsZero{}) {
	assert(variables >= 0);
	if (augmented.empty()) {
		GaussianResult<T> result;
		result.particular.resize(variables);
		for (int i = 0; i < variables; ++i) {
			std::vector<T> basis(variables);
			basis[i] = T(1);
			result.nullspace.push_back(std::move(basis));
		}
		return result;
	}
	Matrix<T, 0, 0> matrix(augmented.size(), variables + 1);
	for (int i = 0; i < (int)augmented.size(); ++i) {
		assert((int)augmented[i].size() == variables + 1);
		for (int j = 0; j <= variables; ++j) matrix[i][j] = augmented[i][j];
	}
	return gaussian_elimination(matrix, variables, is_zero);
}
#endif
