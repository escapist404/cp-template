#define PROBLEM "https://judge.yosupo.jp/problem/assignment"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/km.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<std::vector<long long>> weight(n, std::vector<long long>(n));
	for (auto& row : weight)
		for (auto& x : row) {
			io >> x;
			x = -x;
		}
	auto result = kuhn_munkres(weight);
	std::cout << -result.weight << '\n';
	for (int x : result.left) std::cout << x << ' ';
	std::cout << '\n';
}
