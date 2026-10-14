#define PROBLEM "https://judge.yosupo.jp/problem/lcm_convolution"

#include <iostream>
#include <vector>

#include "../../include/fastio.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<Z> a(n + 1), b(n + 1);
	for (int i = 1; i <= n; ++i) io >> a[i];
	for (int i = 1; i <= n; ++i) io >> b[i];
	for (int d = n; d; --d) {
		for (int i = d + d; i <= n; i += d) {
			a[i] += a[d];
			b[i] += b[d];
		}
	}
	for (int i = 1; i <= n; ++i) a[i] *= b[i];
	for (int d = 1; d <= n; ++d) {
		for (int i = d + d; i <= n; i += d) a[i] -= a[d];
	}
	for (int i = 1; i <= n; ++i) std::cout << a[i] << (i == n ? '\n' : ' ');
}
