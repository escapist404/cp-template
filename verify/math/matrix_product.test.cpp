#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"

#include <iostream>

#include "../../include/matrix.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	int n, m, k;
	std::cin >> n >> m >> k;
	Matrix<Z, 0, 0> a(n, m), b(m, k);
	std::cin >> a >> b;
	std::cout << a * b;
}
