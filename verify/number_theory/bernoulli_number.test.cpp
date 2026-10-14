#define PROBLEM "https://judge.yosupo.jp/problem/bernoulli_number"

#include <iostream>
#include <vector>

#include "../../include/comb.hpp"
#include "../../include/fastio.hpp"
#include "../../include/formal_power_series.hpp"

using Z = ModInt<long long, 998244353>;
using Poly = Polynomial<long long, 998244353, 3>;

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	Combinatorics<Z> combinatorics(n + 1);
	std::vector<Z> denominator(n + 1);
	for (int i = 0; i <= n; ++i) denominator[i] = combinatorics.inverse_factorial(i + 1);
	Poly inverse = Poly(denominator).inverse(n + 1);
	for (int i = 0; i <= n; ++i)
		std::cout << inverse[i] * combinatorics.factorial(i) << ' ';
	std::cout << '\n';
}
