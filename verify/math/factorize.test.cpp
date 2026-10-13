#define PROBLEM "https://judge.yosupo.jp/problem/factorize"

#include <iostream>

#include "../../include/pollard_rho.hpp"

int main() {
	int q;
	std::cin >> q;
	while (q--) {
		long long n;
		std::cin >> n;
		auto factors = factorize(n);
		std::cout << factors.size();
		for (long long factor : factors) std::cout << ' ' << factor;
		std::cout << '\n';
	}
}
