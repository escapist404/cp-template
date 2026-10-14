#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_and_convolution"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/fwt.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	int size = 1 << n;
	std::vector<Z> a(size), b(size);
	for (auto& x : a) io >> x;
	for (auto& x : b) io >> x;
	for (const auto& x : fwt_convolution<Z, fwt_and<Z>>(a, b))
		std::cout << x << ' ';
	std::cout << '\n';
}
