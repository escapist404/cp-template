#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"

#include <iostream>

#include "../../include/miller_rabin.hpp"

int main() {
	int q;
	std::cin >> q;
	while (q--) {
		long long n;
		std::cin >> n;
		std::cout << (is_prime_mr(n) ? "Yes" : "No") << '\n';
	}
}
