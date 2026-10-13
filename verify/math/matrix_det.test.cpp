#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"

#include <iostream>

#include "../../include/determinant.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	int n;
	std::cin >> n;
	Matrix<Z, 0, 0> matrix(n, n);
	std::cin >> matrix;
	std::cout << determinant(matrix) << '\n';
}
