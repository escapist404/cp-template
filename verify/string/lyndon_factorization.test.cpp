#define PROBLEM "https://judge.yosupo.jp/problem/lyndon_factorization"

#include "../../include/fastio.hpp"
#include <iostream>
#include <string>

#include "../../include/lyndon.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	std::string s;
	io >> s;
	std::cout << 0 << ' ';
	for (auto [l, r] : lyndon_factorization(s)) std::cout << r << ' ';
	std::cout << '\n';
}
