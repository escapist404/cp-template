#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/polynomial.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n, m;
	io >> n >> m;
	std::vector<Z> a(n), b(m);
	for (auto& x : a) io >> x;
	for (auto& x : b) io >> x;
	for (const auto& x : convolution<long long, 998244353, 3>(a, b))
		std::cout << x << ' ';
	std::cout << '\n';
}
