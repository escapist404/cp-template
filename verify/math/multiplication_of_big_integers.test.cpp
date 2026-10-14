#define PROBLEM "https://judge.yosupo.jp/problem/multiplication_of_big_integers"

#include <iostream>

#include "../../include/bigint.hpp"

int main() {
	int n;
	std::cin >> n;
	while (n--) {
		BigInt a, b;
		std::cin >> a >> b;
		std::cout << a * b << '\n';
	}
}
