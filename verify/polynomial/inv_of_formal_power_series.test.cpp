#define PROBLEM "https://judge.yosupo.jp/problem/inv_of_formal_power_series"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/formal_power_series.hpp"

using Poly = Polynomial<long long, 998244353, 3>;

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<Poly::Z> a(n);
	for (auto& x : a) io >> x;
	Poly input(a);
	Poly result = input.inverse(n);
	for (auto x : result.data()) std::cout << x << ' ';
	std::cout << '\n';
}
