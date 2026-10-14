#define PROBLEM "https://judge.yosupo.jp/problem/counting_primes"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/min25.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	long long n;
	io >> n;
	std::cout << Min25(n).prime_count() << '\n';
}
