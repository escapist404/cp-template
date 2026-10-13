#define PROBLEM "https://judge.yosupo.jp/problem/static_convex_hull"

#include <iostream>
#include <vector>

#include "../../include/hull.hpp"

int main() {
	int cases;
	std::cin >> cases;
	while (cases--) {
		int n;
		std::cin >> n;
		std::vector<Point> points(n);
		for (auto& point : points) std::cin >> point.x >> point.y;
		auto hull = convex_hull(points);
		std::cout << hull.size() << '\n';
		for (const auto& point : hull)
			std::cout << (long long)point.x << ' ' << (long long)point.y << '\n';
	}
}
