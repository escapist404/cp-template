#define PROBLEM "https://judge.yosupo.jp/problem/montmort_number_mod"

#include <iostream>

#include "../../include/fastio.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	long long mod;
	io >> n >> mod;
	long long previous = 1 % mod, current = 0;
	for (int k = 1; k <= n; ++k) {
		if (k > 1) {
			long long next = (long long)((__int128)(k - 1) * (previous + current) % mod);
			previous = current;
			current = next;
		}
		std::cout << current << (k == n ? '\n' : ' ');
	}
}
