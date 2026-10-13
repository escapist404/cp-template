#ifndef PARALLEL_BINARY_SEARCH_HPP
#define PARALLEL_BINARY_SEARCH_HPP

#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>
#include "binary_search.hpp"

// Queries may carry any payload. Search [low, high); high means no solution.
// Per round: reset(); advance(mid) moves a shared state monotonically to mid;
// check(query, mid) reads that state. No storage proportional to the domain.
template <typename Query, typename Integer, typename Reset, typename Advance, typename Check>
std::vector<Integer> parallel_binary_search(const std::vector<Query>& queries,
	Integer low, Integer high, Reset reset, Advance advance, Check check) {
	assert(low <= high);
	const int n = queries.size();
	std::vector<Integer> left(n, low), right(n, high), mid(n);
	std::vector<int> active;
	while (true) {
		active.clear();
		for (int i = 0; i < n; ++i) {
			if (left[i] < right[i]) {
				mid[i] = binary_midpoint(left[i], right[i]);
				active.push_back(i);
			}
		}
		if (active.empty()) {
			return left;
		}
		std::sort(active.begin(), active.end(), [&](int a, int b) {
			return mid[a] < mid[b];
		});
		reset();
		for (const int id : active) {
			advance(mid[id]);
			if (check(queries[id], mid[id])) {
				right[id] = mid[id];
			} else {
				left[id] = mid[id] + 1;
			}
		}
	}
}

#endif
