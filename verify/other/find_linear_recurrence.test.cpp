#define PROBLEM "https://judge.yosupo.jp/problem/find_linear_recurrence"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/linear_recurrence.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<Z> sequence(n);
	for (auto& x : sequence) io >> x;
	auto recurrence = berlekamp_massey(sequence);
	std::cout << recurrence.size() << '\n';
	for (auto x : recurrence) std::cout << x << ' ';
	std::cout << '\n';
}
