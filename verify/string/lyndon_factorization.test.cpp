#define PROBLEM "https://judge.yosupo.jp/problem/lyndon_factorization"

#include <iostream>
#include <string>

#include "../../include/lyndon.hpp"

int main() {
	std::string s;
	std::cin >> s;
	std::cout << 0 << ' ';
	for (auto [l, r] : lyndon_factorization(s)) std::cout << r << ' ';
	std::cout << '\n';
}
