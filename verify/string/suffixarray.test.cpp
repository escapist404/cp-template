#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"

#include <iostream>
#include <string>

#include "../../include/suffix_array.hpp"

int main() {
	std::string s;
	std::cin >> s;
	SuffixArray suffix_array(s);
	for (int position : suffix_array.array()) std::cout << position << ' ';
	std::cout << '\n';
}
