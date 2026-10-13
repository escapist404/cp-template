#ifndef DETERMINANT_HPP
#define DETERMINANT_HPP

#include <cassert>
#include <utility>

#include "matrix.hpp"

// T must support +, -, *, /, unary -, equality, and construction from 0 and 1.
template <typename T, size_t R, size_t C>
T determinant(const Matrix<T, R, C>& input) {
	assert(input.rows() == input.columns());
	const size_t n = input.rows();
	if (n == 0) return T(1);

	// Bareiss elimination keeps every intermediate value integral.
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

#endif // DETERMINANT_HPP
