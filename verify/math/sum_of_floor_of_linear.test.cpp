#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_floor_of_linear"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/floor_sum.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int t;
	io >> t;
	while (t--) {
		long long n, m, a, b;
		io >> n >> m >> a >> b;
		std::cout << floor_sum(0, n, m, a, b) << '\n';
	}
}
