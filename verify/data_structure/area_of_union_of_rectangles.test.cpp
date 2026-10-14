#define PROBLEM "https://judge.yosupo.jp/problem/area_of_union_of_rectangles"

#include "../../include/fastio.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/coordinate_compression.hpp"
#include "../../include/lazy_segtree.hpp"

struct CoverValue {
	long long minimum, zero_length, total_length;
};
struct CoverLazy {
	long long add;
};

CoverValue operation(CoverValue left, CoverValue right) {
	CoverValue result{std::min(left.minimum, right.minimum), 0,
			left.total_length + right.total_length};
	if (left.minimum == result.minimum) result.zero_length += left.zero_length;
	if (right.minimum == result.minimum) result.zero_length += right.zero_length;
	return result;
}
CoverValue identity_value() { return {4'000'000'000'000'000'000LL, 0, 0}; }
CoverValue mapping(CoverLazy lazy, CoverValue value) {
	if (value.minimum < identity_value().minimum) value.minimum += lazy.add;
	return value;
}
CoverLazy composition(CoverLazy newer, CoverLazy older) {
	return {newer.add + older.add};
}
CoverLazy identity_lazy() { return {0}; }

struct Event {
	long long x;
	int low, high;
	int delta;
};

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<Event> events;
	std::vector<long long> raw_y;
	for (int i = 0; i < n; ++i) {
		long long l, d, r, u;
		io >> l >> d >> r >> u;
		int low = raw_y.size();
		raw_y.push_back(d);
		int high = raw_y.size();
		raw_y.push_back(u);
		events.push_back({l, low, high, 1});
		events.push_back({r, low, high, -1});
	}
	auto [ys, y_index] = coordinate_compress(raw_y);
	std::sort(events.begin(), events.end(), [](Event a, Event b) { return a.x < b.x; });
	std::vector<CoverValue> initial(ys.size() - 1);
	for (int i = 0; i + 1 < (int)ys.size(); ++i)
		initial[i] = {0, ys[i + 1] - ys[i], ys[i + 1] - ys[i]};
	LazySegmentTree<CoverValue, operation, identity_value, CoverLazy,
			mapping, composition, identity_lazy> tree(initial);
	long long answer = 0, previous_x = events[0].x;
	for (const auto& event : events) {
		const auto current = tree.all_prod();
		const long long covered = current.total_length -
			(current.minimum == 0 ? current.zero_length : 0);
		answer += (event.x - previous_x) * covered;
		tree.apply(y_index[event.low], y_index[event.high], {event.delta});
		previous_x = event.x;
	}
	std::cout << answer << '\n';
}
