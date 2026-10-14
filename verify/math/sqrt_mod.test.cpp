#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/modular_roots.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int t;
	io >> t;
	while (t--) {
		long long y, p;
		io >> y >> p;
		std::cout << modular_sqrt(y, p) << '\n';
	}
}
