#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"

#include <iostream>
#include <string>

#include "../../include/z_algo.hpp"

int main() {
	std::string s;
	std::cin >> s;
	for (int value : z_algorithm(s)) std::cout << value << ' ';
	std::cout << '\n';
}
