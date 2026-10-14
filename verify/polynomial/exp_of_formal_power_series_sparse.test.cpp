#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series_sparse"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/formal_power_series.hpp"

using Poly = Polynomial<long long, 998244353, 3>;

int main() {
	OptimizedIO<1 << 20> io;
	int n, k;
	io >> n >> k;
	std::vector<Poly::Z> a(n);
	for (int i, value; k--;) {
		io >> i >> value;
		a[i] = value;
	}
	Poly input(a), result = input.exponential(n);
	for (auto x : result.data()) std::cout << x << ' ';
	std::cout << '\n';
}
