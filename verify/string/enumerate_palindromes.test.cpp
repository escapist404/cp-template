#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_palindromes"

#include "../../include/fastio.hpp"
#include <iostream>
#include <string>

#include "../../include/manacher.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	std::string s;
	io >> s;
	auto radii = manacher(s);
	for (int i = 0; i < 2 * (int)s.size() - 1; ++i)
		std::cout << radii[i + 1] - 1 << ' ';
	std::cout << '\n';
}
