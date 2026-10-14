#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/modular_roots.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int q;
	io >> q;
	while (q--) {
		long long p;
		io >> p;
		std::cout << primitive_root_prime(p) << '\n';
	}
}
