#define PROBLEM "https://judge.yosupo.jp/problem/static_range_mode_query"

#include <algorithm>
#include <iostream>
#include <utility>
#include <unordered_set>
#include <vector>

#include "../../include/coordinate_compression.hpp"
#include "../../include/fastio.hpp"
#include "../../include/mo.hpp"

struct ModeState {
	std::vector<int> count;
	std::vector<std::unordered_set<int>> bucket;
	int maximum = 0;
	ModeState(int value_count, int max_frequency)
		: count(value_count), bucket(max_frequency + 1) {}
};

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<int> a(n);
	for (auto& x : a) io >> x;
	auto compressed_result = coordinate_compress(a);
	auto values = std::move(compressed_result.first);
	a = std::move(compressed_result.second);
	std::vector<MoQuery> queries(q);
	for (int i = 0; i < q; ++i) {
		io >> queries[i].l >> queries[i].r;
		queries[i].id = i;
	}
	ModeState state(values.size(), n);
	auto add = [&](int i, ModeState& state) {
		int value = a[i], old = state.count[value];
		if (old) state.bucket[old].erase(value);
		int next = ++state.count[value];
		state.bucket[next].insert(value);
		state.maximum = std::max(state.maximum, state.count[value]);
	};
	auto remove = [&](int i, ModeState& state) {
		int value = a[i], old = state.count[value];
		state.bucket[old].erase(value);
		if (--state.count[value]) state.bucket[state.count[value]].insert(value);
		while (state.maximum && state.bucket[state.maximum].empty()) --state.maximum;
	};
	auto answer = [&](ModeState& state, const MoQuery&) {
		int value = *state.bucket[state.maximum].begin();
		return std::pair<int, int>{values[value], state.maximum};
	};
	for (auto [value, count] : mo_algorithm(queries, n, state, add, remove, answer))
		std::cout << value << ' ' << count << '\n';
}
