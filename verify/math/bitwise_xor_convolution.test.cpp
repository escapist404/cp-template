#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_xor_convolution"

#include <iostream>
#include <vector>

#include "../../include/fwt.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	int n;
	std::cin >> n;
	int size = 1 << n;
	std::vector<Z> a(size), b(size);
	for (auto& x : a) std::cin >> x;
	for (auto& x : b) std::cin >> x;
	auto result = fwt_convolution<Z, fwt_xor<Z>>(a, b);
	for (const auto& x : result) std::cout << x << ' ';
	std::cout << '\n';
}
