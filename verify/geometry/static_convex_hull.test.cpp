#define PROBLEM "https://judge.yosupo.jp/problem/static_convex_hull"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/hull.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int cases;
	io >> cases;
	while (cases--) {
		int n;
		io >> n;
		std::vector<Point> points(n);
		for (auto& point : points) io >> point.x >> point.y;
		auto hull = convex_hull(points);
		std::cout << hull.size() << '\n';
		for (const auto& point : hull)
			std::cout << (long long)point.x << ' ' << (long long)point.y << '\n';
	}
}
