#ifndef MATRIX_TREE_HPP
#define MATRIX_TREE_HPP

#include <cassert>
#include <vector>

#include "determinant.hpp"

// ============================================================================
// 【Kirchhoff 矩阵树定理：无向图生成树计数 (matrix_tree.hpp)】
// ----------------------------------------------------------------------------
// 1. 带权边结构体签名：
//    template <typename T> struct MatrixTreeEdge {
//        int u, v; T weight;
//        MatrixTreeEdge(int u, int v, T weight = T(1));
//    };
// 2. 函数签名与接口详解（0-based 顶点编号 [0, n)，自动忽略自环 u == v，支持重边累加）：
//    - template <typename T>
//      T matrix_tree(int n, const std::vector<MatrixTreeEdge<T>>& edges)
//      功能：构造无向图拉普拉斯矩阵 L 并求其 (n - 1) 阶主子式行列式，返回所有生成树的边权乘积之和（n <= 1 返回 T(1)）。O(n^3 + m)。
//    - template <typename T = long long>
//      T matrix_tree(int n, const std::vector<std::pair<int, int>>& edges)
//      功能：无权无向图重载（边权视为 1），返回无向图的生成树总棵数。时间复杂度 O(n^3 + m)。
// ============================================================================

template <typename T>
struct MatrixTreeEdge {
	int u, v;
	T weight;

	MatrixTreeEdge(int u, int v, T weight = T(1))
		: u(u), v(v), weight(weight) {}
};

// 无向加权图的 Matrix-Tree 定理：支持重边，自动忽略自环，返回所有生成树边权乘积之和。
template <typename T>
T matrix_tree(int n, const std::vector<MatrixTreeEdge<T>>& edges) {
	assert(n >= 1);
	if (n == 1) return T(1);
	Matrix<T, 0, 0> laplacian(n, n);
	for (const auto& edge : edges) {
		assert(0 <= edge.u && edge.u < n);
		assert(0 <= edge.v && edge.v < n);
		if (edge.u == edge.v) continue;
		laplacian[edge.u][edge.u] += edge.weight;
		laplacian[edge.v][edge.v] += edge.weight;
		laplacian[edge.u][edge.v] -= edge.weight;
		laplacian[edge.v][edge.u] -= edge.weight;
	}

	Matrix<T, 0, 0> minor(n - 1, n - 1);
	for (int i = 0; i + 1 < n; ++i)
		for (int j = 0; j + 1 < n; ++j)
			minor[i][j] = laplacian[i][j];
	return determinant(minor);
}

template <typename T = long long>
T matrix_tree(int n, const std::vector<std::pair<int, int>>& edges) {
	std::vector<MatrixTreeEdge<T>> weighted;
	weighted.reserve(edges.size());
	for (const auto& [u, v] : edges) weighted.emplace_back(u, v);
	return matrix_tree(n, weighted);
}

#endif
