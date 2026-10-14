#define PROBLEM "https://judge.yosupo.jp/problem/discrete_logarithm_mod"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/bsgs.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int t;
	io >> t;
	while (t--) {
		long long x, y, mod;
		io >> x >> y >> mod;
		std::cout << bsgs(x, y, mod) << '\n';
	}
}
