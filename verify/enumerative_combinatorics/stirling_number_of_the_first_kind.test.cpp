#define PROBLEM "https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/fastio.hpp"
#include "../../include/modint.hpp"
#include "../../include/polynomial.hpp"

using Z = ModInt<long long, 998244353>;

std::vector<Z> product(int l, int r) {
	if (r - l == 1) return {Z(-l), Z(1)};
	int m = (l + r) / 2;
	std::vector<Z> left = product(l, m), right = product(m, r);
	return convolution<long long, 998244353, 3>(left, right);
}

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<Z> answer = n ? product(0, n) : std::vector<Z>{Z(1)};
	for (int i = 0; i <= n; ++i) std::cout << answer[i] << (i == n ? '\n' : ' ');
}
