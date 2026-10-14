#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "../../include/comb.hpp"
#include "../../include/fastio.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int t;
	long long modulus;
	io >> t >> modulus;
	using Z = ModInt<long long, 0>;
	Z::set_mod(modulus);
	std::vector<std::pair<int, int>> queries(t);
	int maximum = 0;
	for (auto& [n, k] : queries) {
		io >> n >> k;
		maximum = std::max(maximum, n);
	}
	Combinatorics<Z> combinatorics(maximum);
	for (auto [n, k] : queries) std::cout << combinatorics.choose(n, k) << '\n';
}
