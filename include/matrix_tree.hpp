#ifndef MATRIX_TREE_HPP
#define MATRIX_TREE_HPP

#include <cassert>
#include <vector>

#include "determinant.hpp"

// 【Kirchhoff 矩阵树定理生成树计数 (matrix_tree.hpp)】
// - 0-based 顶点编号 [0, n)，时间复杂度 O(n^3)
// - count_spanning_trees_undirected(n, edges)       : 统计无向图所有生成树的边权乘积之和（无权图传 weight = 1）
// - count_spanning_trees_directed(n, edges, root, out): 统计以 root 为根的有向生成树数量（out=true 为外向树，false 为内向树）

template <typename T>
struct MatrixTreeEdge {
	int u, v;
	T weight;

	MatrixTreeEdge(int u, int v, T weight = T(1))
		: u(u), v(v), weight(weight) {}
};

// Weighted undirected graph. Parallel edges are allowed; self-loops are ignored.
// T must support +, -, *, /, unary -, equality, and construction from 0 and 1.
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

#endif // MATRIX_TREE_HPP
