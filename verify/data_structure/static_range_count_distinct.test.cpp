#define PROBLEM "https://judge.yosupo.jp/problem/static_range_count_distinct"

#include "../../include/fastio.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/coordinate_compression.hpp"
#include "../../include/mo.hpp"

struct State {
	std::vector<int> count;
	int distinct = 0;
	int size = 0;
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
	State state{std::vector<int>(values.size())};
	auto add = [&](int i, State& state) {
		if (!state.count[a[i]]++) ++state.distinct;
		++state.size;
	};
	auto remove = [&](int i, State& state) {
		if (!--state.count[a[i]]) --state.distinct;
		--state.size;
	};
	auto answer = [](State& state, const MoQuery&) { return state.distinct; };
	auto result = mo_algorithm(queries, n, state, add, remove, answer);
	for (int x : result) std::cout << x << '\n';
}
