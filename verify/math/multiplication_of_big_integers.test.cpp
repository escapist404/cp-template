#define PROBLEM "https://judge.yosupo.jp/problem/multiplication_of_big_integers"

#include "../../include/fastio.hpp"
#include <iostream>
#include <string>

#include "../../include/bigint.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	while (n--) {
		std::string sa, sb;
		io >> sa >> sb;
		BigInt a(sa), b(sb);
		std::cout << a * b << '\n';
	}
}
