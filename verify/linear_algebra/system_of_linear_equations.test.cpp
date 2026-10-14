#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/gaussian.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n, m;
	io >> n >> m;
	Matrix<Z, 0, 0> augmented(n, m + 1);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) io >> augmented[i][j];
	for (int i = 0; i < n; ++i) io >> augmented[i][m];
	auto result = gaussian_elimination(augmented, m);
	if (!result.consistent) {
		std::cout << -1 << '\n';
		return 0;
	}
	std::cout << result.nullspace.size() << '\n';
	for (auto x : result.particular) std::cout << x << ' ';
	std::cout << '\n';
	for (const auto& basis : result.nullspace) {
		for (auto x : basis) std::cout << x << ' ';
		std::cout << '\n';
	}
}
