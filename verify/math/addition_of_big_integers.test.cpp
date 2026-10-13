#define PROBLEM "https://judge.yosupo.jp/problem/addition_of_big_integers"

#include <iostream>

#include "../../include/bigint.hpp"

int main() {
	int t;
	std::cin >> t;
	while (t--) {
		BigInt a, b;
		std::cin >> a >> b;
		std::cout << a + b << '\n';
	}
}
