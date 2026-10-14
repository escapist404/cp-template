#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"

#include "../../include/fastio.hpp"
#include <iostream>
#include <string>

#include "../../include/z_algo.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	std::string s;
	io >> s;
	for (int value : z_algorithm(s)) std::cout << value << ' ';
	std::cout << '\n';
}
