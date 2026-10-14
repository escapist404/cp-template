#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/miller_rabin.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int q;
	io >> q;
	while (q--) {
		long long n;
		io >> n;
		std::cout << (is_prime_mr(n) ? "Yes" : "No") << '\n';
	}
}
