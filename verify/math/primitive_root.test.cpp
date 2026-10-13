#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"

#include <iostream>

#include "../../include/modular_roots.hpp"

int main() {
	int q;
	std::cin >> q;
	while (q--) {
		long long p;
		std::cin >> p;
		std::cout << primitive_root_prime(p) << '\n';
	}
}
