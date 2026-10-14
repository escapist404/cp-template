#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/matrix.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n, m, k;
	io >> n >> m >> k;
	Matrix<Z, 0, 0> a(n, m), b(m, k);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) io >> a[i][j];
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < k; ++j) io >> b[i][j];
	std::cout << a * b;
}
