#ifndef LGV_HPP
#define LGV_HPP

#include <cassert>

#include "determinant.hpp"

// 【Lindstrom-Gessel-Viennot (LGV) 引理不相交路径计数 (lgv.hpp)】
// - 在有向无环图 (DAG) 中，给定 k 个起点与 k 个终点，统计顶点不相交路径组的带符号权值和
// - lgv_lemma(k, count_paths): 调用方传入回调 count_paths(i, j) 返回从第 i 个起点到第 j 个终点的路径数，O(k^3) 返回 k x k 路径矩阵的行列式

template <typename T, typename PathCount>
T lgv(int k, PathCount count_paths) {
	assert(k >= 0);
	if (k == 0) return T(1); // One empty path family.
	Matrix<T, 0, 0> paths(k, k);
	for (int i = 0; i < k; ++i)
		for (int j = 0; j < k; ++j)
			paths[i][j] = count_paths(i, j);
	return determinant(paths);
}

#endif // LGV_HPP
