#define PROBLEM "https://judge.yosupo.jp/problem/bell_number"

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
	Combinatorics<Z> combinatorics(n);
	std::vector<Z> exponent(n + 1);
	for (int i = 1; i <= n; ++i) exponent[i] = combinatorics.inverse_factorial(i);
	Poly result = Poly(exponent).exponential(n + 1);
	for (int i = 0; i <= n; ++i)
		std::cout << result[i] * combinatorics.factorial(i) << ' ';
	std::cout << '\n';
}
