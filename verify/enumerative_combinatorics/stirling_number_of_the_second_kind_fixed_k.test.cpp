#define PROBLEM "https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind_fixed_k"

#include <iostream>
#include <vector>

#include "../../include/comb.hpp"
#include "../../include/fastio.hpp"
#include "../../include/formal_power_series.hpp"

using Z = ModInt<long long, 998244353>;
using Poly = Polynomial<long long, 998244353, 3>;

int main() {
	OptimizedIO<1 << 20> io;
	int n, k;
	io >> n >> k;
	Combinatorics<Z> combinatorics(n);
	std::vector<Z> exponential(n + 1);
	for (int i = 1; i <= n; ++i) exponential[i] = combinatorics.inverse_factorial(i);
	Poly result = Poly(exponential).power(k, n + 1);
	Z scale = combinatorics.inverse_factorial(k);
	for (int i = k; i <= n; ++i)
		std::cout << result[i] * scale * combinatorics.factorial(i)
			<< (i == n ? '\n' : ' ');
}
