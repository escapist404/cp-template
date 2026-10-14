#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"

#include <iostream>
#include <vector>

#include "../../include/exgcd.hpp"
#include "../../include/fastio.hpp"
#include "../../include/modint.hpp"
#include "../../include/polynomial.hpp"

using Z1 = ModInt<long long, 998244353>;
using Z2 = ModInt<long long, 1004535809>;
using Z3 = ModInt<long long, 469762049>;

int main() {
	OptimizedIO<1 << 20> io;
	int n, m;
	io >> n >> m;
	std::vector<Z1> a1(n), b1(m);
	std::vector<Z2> a2(n), b2(m);
	std::vector<Z3> a3(n), b3(m);
	for (int i = 0; i < n; ++i) {
		long long x;
		io >> x;
		a1[i] = Z1(x);
		a2[i] = Z2(x);
		a3[i] = Z3(x);
	}
	for (int i = 0; i < m; ++i) {
		long long x;
		io >> x;
		b1[i] = Z1(x);
		b2[i] = Z2(x);
		b3[i] = Z3(x);
	}
	a1 = convolution<long long, 998244353, 3>(a1, b1);
	a2 = convolution<long long, 1004535809, 3>(a2, b2);
	a3 = convolution<long long, 469762049, 3>(a3, b3);
	const i64 p1 = 998244353, p2 = 1004535809, p3 = 469762049, mod = 1000000007;
	const i64 inv12 = mod_inverse(p1 % p2, p2);
	const i64 inv123 = mod_inverse((i64)((i128)p1 * p2 % p3), p3);
	for (int i = 0; i < n + m - 1; ++i) {
		i64 r1 = a1[i].data(), r2 = a2[i].data(), r3 = a3[i].data();
		i64 t1 = (i64)((i128)((r2 - r1 + p2) % p2) * inv12 % p2);
		i64 x12 = (i64)((i128)p1 * t1 + r1);
		i64 delta = (r3 - x12 % p3 + p3) % p3;
		i64 t2 = (i64)((i128)delta * inv123 % p3);
		i128 result = (i128)x12 + (i128)p1 * p2 * t2;
		std::cout << (i64)(result % mod) << (i == n + m - 2 ? '\n' : ' ');
	}
}
