#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_quotients"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/min25.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	long long n;
	io >> n;
	auto quotients = Min25(n).quotients();
	std::cout << quotients.size() << '\n';
	for (auto x : quotients) std::cout << x << ' ';
	std::cout << '\n';
}
