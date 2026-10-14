#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"

#include "../../include/fastio.hpp"
#include <iostream>
#include <string>

#include "../../include/suffix_array.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	std::string s;
	io >> s;
	SuffixArray suffix_array(s);
	for (int position : suffix_array.array()) std::cout << position << ' ';
	std::cout << '\n';
}
