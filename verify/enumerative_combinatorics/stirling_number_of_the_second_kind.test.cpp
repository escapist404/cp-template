#define PROBLEM "https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/modint.hpp"
#include "../../include/polynomial.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<Z> fact(n + 1), ifact(n + 1), a(n + 1), b(n + 1);
	fact[0] = ifact[0] = Z(1);
	for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * Z(i);
	ifact[n] = Z(1) / fact[n];
	for (int i = n; i; --i) ifact[i - 1] = ifact[i] * Z(i);
	for (int i = 0; i <= n; ++i) {
		a[i] = power(Z(1), Z(i), n) * ifact[i];
		b[i] = (i & 1) ? -ifact[i] : ifact[i];
	}
	a = convolution<long long, 998244353, 3>(a, b);
	for (int k = 0; k <= n; ++k) std::cout << a[k] << ' ';
	std::cout << '\n';
}
