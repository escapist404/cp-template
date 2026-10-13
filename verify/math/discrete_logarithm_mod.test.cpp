#define PROBLEM "https://judge.yosupo.jp/problem/discrete_logarithm_mod"

#include <iostream>

#include "../../include/bsgs.hpp"

int main() {
	int t;
	std::cin >> t;
	while (t--) {
		long long x, y, mod;
		std::cin >> x >> y >> mod;
		std::cout << bsgs(x, y, mod) << '\n';
	}
}
