#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"

#include "../../include/fastio.hpp"
#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

#include "../../include/coordinate_compression.hpp"
#include "../../include/fenwick.hpp"

struct Point { long long x, y, w; int y_index; };
struct Event { long long x, d, u; int id, sign; };

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<Point> points(n);
	std::vector<long long> raw_y;
	for (auto& p : points) {
		io >> p.x >> p.y >> p.w;
		raw_y.push_back(p.y);
	}
	auto [ys, y_index] = coordinate_compress(raw_y);
	for (int i = 0; i < n; ++i) points[i].y_index = y_index[i];
	std::vector<Event> events;
	for (int i = 0; i < q; ++i) {
		long long l, d, r, u;
		io >> l >> d >> r >> u;
		events.push_back({r, d, u, i, 1});
		events.push_back({l, d, u, i, -1});
	}
	std::sort(points.begin(), points.end(), [](Point a, Point b) { return a.x < b.x; });
	std::sort(events.begin(), events.end(), [](Event a, Event b) { return a.x < b.x; });
	Fenwick<long long> tree(ys.size());
	std::vector<long long> answer(q);
	int p = 0;
	for (auto event : events) {
		while (p < n && points[p].x < event.x) {
			tree.add(points[p].y_index, points[p].w);
			++p;
		}
		int low = std::lower_bound(ys.begin(), ys.end(), event.d) - ys.begin();
		int high = std::lower_bound(ys.begin(), ys.end(), event.u) - ys.begin();
		answer[event.id] += event.sign * tree.get(low, high);
	}
	for (auto x : answer) std::cout << x << '\n';
}
