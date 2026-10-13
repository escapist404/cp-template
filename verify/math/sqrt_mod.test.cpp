#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"

#include <iostream>

#include "../../include/modular_roots.hpp"

int main() {
	int t;
	std::cin >> t;
	while (t--) {
		long long y, p;
		std::cin >> y >> p;
		std::cout << modular_sqrt(y, p) << '\n';
	}
}
