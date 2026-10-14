#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

#include "../../include/fastio.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/coordinate_compression.hpp"
#include "../../include/fenwick.hpp"
#include "../../include/mo.hpp"

struct State {
	Fenwick<int> count;
	long long inversions = 0;
	int size = 0;
	State(int n) : count(n) {}
};

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<int> a(n);
	for (auto& x : a) io >> x;
	auto [values, compressed] = coordinate_compress(a);
	a = std::move(compressed);
	std::vector<MoQuery> queries(q);
	for (int i = 0; i < q; ++i) {
		io >> queries[i].l >> queries[i].r;
		queries[i].id = i;
	}
	State state(values.size());
	// Run Mo with a side-aware state encoded by the current window endpoints.
	int left = 0, right = 0;
	auto result = mo_algorithm(queries, n, state,
		[&](int i, State& state) {
			if (i < left) {
				int less = state.count.get(a[i]);
				state.inversions += less;
			} else {
				int greater = state.size - state.count.get(a[i] + 1);
				state.inversions += greater;
			}
			state.count.add(a[i], 1);
			++state.size;
			if (i < left) left = i;
			else if (i >= right) right = i + 1;
		},
		[&](int i, State& state) {
			bool from_left = i == left;
			state.count.add(a[i], -1);
			--state.size;
			if (from_left) {
				state.inversions -= state.count.get(a[i]);
				++left;
			} else {
				state.inversions -= state.size - state.count.get(a[i] + 1);
				--right;
			}
		},
		[](State& state, const MoQuery&) { return state.inversions; });
	for (auto x : result) std::cout << x << '\n';
}
