#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/determinant.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	Matrix<Z, 0, 0> matrix(n, n);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) io >> matrix[i][j];
	std::cout << determinant(matrix) << '\n';
}
