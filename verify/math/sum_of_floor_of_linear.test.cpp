#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_floor_of_linear"

#include <iostream>

#include "../../include/floor_sum.hpp"

int main() {
	int t;
	std::cin >> t;
	while (t--) {
		long long n, m, a, b;
		std::cin >> n >> m >> a >> b;
		std::cout << floor_sum(0, n, m, a, b) << '\n';
	}
}
