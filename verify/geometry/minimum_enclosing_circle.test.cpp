#define PROBLEM "https://judge.yosupo.jp/problem/minimum_enclosing_circle"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

#include "../../include/minimum_enclosing_circle.hpp"

int main() {
	int n;
	std::cin >> n;
	std::vector<Point> points(n);
	for (auto& point : points) std::cin >> point.x >> point.y;
	Circle circle = minimum_enclosing_circle(points);
	for (const auto& point : points) {
		Real d2 = distance_squared(circle.center, point);
		Real r2 = circle.radius * circle.radius;
		std::cout << (std::fabs(d2 - r2) <= 1e-10 * std::max(1.0, r2));
	}
	std::cout << '\n';
}
