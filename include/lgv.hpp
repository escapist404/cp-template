#ifndef LGV_HPP
#define LGV_HPP

#include <cassert>

#include "determinant.hpp"

// Lindstrom-Gessel-Viennot lemma on a DAG with k distinct sources and k distinct sinks.
// count_paths(i, j) returns the sum of path weights from source i to sink j;
// a path's weight is the product of its edge weights (use 1 for unweighted edges).
// Returns the signed sum over vertex-disjoint path families, with the sign of
// the permutation matching sources to sinks. It counts source i -> sink i
// families only when no nonidentity permutation admits a vertex-disjoint family.
// The caller must establish these graph/endpoint conditions.
// T must satisfy determinant's arithmetic requirements: use a field (e.g. prime
// ModInt) or exact integral arithmetic without overflow.
// O(k^2) callback calls, O(k^3) arithmetic operations, O(k^2) space.
// Example: lgv<Z>(k, [&](int i, int j) { return paths(sources[i], sinks[j]); });
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
