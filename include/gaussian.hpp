#ifndef GAUSSIAN_HPP
#define GAUSSIAN_HPP

#include <cassert>
#include <cmath>
#include <type_traits>
#include <vector>

#include "matrix.hpp"

// ============================================================================
// 【域上高斯消元求解线性方程组与矩阵求秩 Gaussian Elimination (gaussian.hpp)】
// ----------------------------------------------------------------------------
// 1. 判零仿函数与求解结果结构体签名：
//    - template <typename T> struct GaussianZero { bool operator()(T value) const; };
//      浮点类型按 |value| <= 1e-10 判零，精确类型（如 ModInt）按 value == T{} 判零。
//    - template <typename T> struct GaussianResult {
//          bool consistent = true;                // 方程组是否相容（有解为 true，矛盾无解为 false）
//          int rank = 0;                          // 系数矩阵的秩
//          std::vector<T> particular;             // 当有解时，返回长度为 variables 的一组特解 x_0（自由变量取 0）
//          std::vector<std::vector<T>> nullspace; // 齐次方程组 A*x = 0 的零空间基向量列表（共 variables - rank 个长度为 variables 的基向量）
//      };
//      【通解表示】方程组的全部解集可表示为 x = particular + sum_k c_k * nullspace[k]。
// 2. 函数签名与接口详解（要求 T 为域类型，如 double/long double 或素数模 ModInt）：
//    - template <typename T, typename IsZero = GaussianZero<T>>
//      int matrix_rank(std::vector<T> matrix, int n, int m, IsZero is_zero = IsZero{})
//      功能：传入长度为 n * m 的行优先一维数组 matrix，仅计算 n 行 m 列矩阵的秩（支持 n=0 或 m=0）。O(n * m * min(n, m))。
//    - template <typename T, size_t R, size_t C, typename IsZero = GaussianZero<T>>
//      GaussianResult<T> gaussian_elimination(Matrix<T, R, C> augmented, int variables, IsZero is_zero = IsZero{})
//    - template <typename T, typename IsZero = GaussianZero<T>>
//      GaussianResult<T> gaussian_elimination(std::vector<std::vector<T>> augmented, int variables, IsZero is_zero = IsZero{})
//      功能：输入 n 行 (variables + 1) 列的增广矩阵 [A | b]（前 variables 列为未知数系数，最后一列为常数项 b），
//            执行高斯-约旦列主元消元并返回 GaussianResult<T>{consistent, rank, particular, nullspace}。
//      复杂度：时间复杂度 O(n * variables * min(n, variables))，空间复杂度 O(variables * (n + variables))。
// ============================================================================

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

// 计算域上 n x m 矩阵的秩（行主序存储），避免构造特解与零空间向量以节省开销。
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

// 求解域（浮点数或素数模数 ModInt）上的线性方程组 Ax = b：增广矩阵每行含 variables + 1 列，通解 = particular + span(nullspace)。
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
