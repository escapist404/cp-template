#define PROBLEM "https://judge.yosupo.jp/problem/two_sat"

#include <cstdlib>
#include <iostream>
#include <string>

#include "../../include/two_sat.hpp"

int main() {
	int n, m;
	std::string header;
	std::cin >> header >> header >> n >> m;
	TwoSat sat(n);
	while (m--) {
		int x, y;
		std::cin >> x >> y >> header;
		int vx = x > 0, vy = y > 0;
		sat.add_or(std::abs(x) - 1, vx, std::abs(y) - 1, vy);
	}
	if (!sat.solve()) {
		std::cout << "s UNSATISFIABLE\n";
		return 0;
	}
	std::cout << "s SATISFIABLE\nv";
	for (int i = 0; i < n; ++i)
		std::cout << ' ' << (sat.assignment()[i] ? i + 1 : -i - 1);
	std::cout << " 0\n";
}
