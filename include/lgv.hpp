#ifndef LGV_HPP
#define LGV_HPP

#include <cassert>

#include "determinant.hpp"

// ============================================================================
// 【Lindström–Gessel–Viennot (LGV) 引理：DAG 不相交路径组计数 (lgv.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename T, typename PathCount>
//   T lgv(int k, PathCount count_paths)
// 参数与返回值详解：
//   - k：起点集合 A = {a_0, ..., a_{k-1}} 与终点集合 B = {b_0, ..., b_{k-1}} 的大小（k >= 0，k == 0 返回 T(1)）。
//   - count_paths：回调函数 `T count_paths(int i, int j)`（0 <= i, j < k），返回 DAG 中从 a_i 到 b_j 的路径数（或权值和）。
//   - 返回值 T：构造 k x k 路径矩阵 M[i][j] = count_paths(i, j) 并返回 det(M)，即不相交路径组的带符号权值和。O(k^3)。
// ============================================================================

// template <typename T, typename PathCount> T lgv(int k, PathCount count_paths) — 功能：T lgv(int k, PathCount count_paths)
template <typename T, typename PathCount>
T lgv(int k, PathCount count_paths) {
	assert(k >= 0);
	if (k == 0) return T(1); // 0 个起点与终点时，存在唯一的一组空路径族，行列式为 1。
	Matrix<T, 0, 0> paths(k, k);
	for (int i = 0; i < k; ++i)
		for (int j = 0; j < k; ++j)
			paths[i][j] = count_paths(i, j);
	return determinant(paths);
}

#endif
