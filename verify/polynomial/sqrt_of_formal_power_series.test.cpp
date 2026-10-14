#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_of_formal_power_series"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/formal_power_series.hpp"

using Poly = Polynomial<long long, 998244353, 3>;
using Z = Poly::Z;

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<Z> a(n);
	for (auto& x : a) io >> x;
	int shift = 0;
	while (shift < n && a[shift] == Z()) ++shift;
	if (shift == n) {
		for (int i = 0; i < n; ++i) std::cout << 0 << ' ';
		std::cout << '\n';
		return 0;
	}
	if ((shift & 1) || power(Z(1), a[shift], (Z::mod() - 1) / 2) != Z(1)) {
		std::cout << -1 << '\n';
		return 0;
	}
	Poly input(a);
	Poly result = input.square_root(n);
	for (auto x : result.data()) std::cout << x << ' ';
	std::cout << '\n';
}
