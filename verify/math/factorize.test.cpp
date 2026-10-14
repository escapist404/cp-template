#define PROBLEM "https://judge.yosupo.jp/problem/factorize"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/pollard_rho.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int q;
	io >> q;
	while (q--) {
		long long n;
		io >> n;
		auto factors = factorize(n);
		std::cout << factors.size();
		for (long long factor : factors) std::cout << ' ' << factor;
		std::cout << '\n';
	}
}
