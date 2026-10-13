#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include <iostream>
#include <vector>

#include "../../include/polynomial.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	int n, m;
	std::cin >> n >> m;
	std::vector<Z> a(n), b(m);
	for (auto& x : a) std::cin >> x;
	for (auto& x : b) std::cin >> x;
	for (const auto& x : convolution<long long, 998244353, 3>(a, b))
		std::cout << x << ' ';
	std::cout << '\n';
}
