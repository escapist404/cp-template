#ifndef WQS_BINARY_SEARCH_HPP
#define WQS_BINARY_SEARCH_HPP

#include <cassert>
#include <utility>
#include "types.hpp"

// solve(p) -> {count, min(original_cost + p * count)}; prefer MORE items on ties.
// Requires discrete convex cost(count), a feasible required count, and bounds
// bracketing it: solve(low).first >= required >= solve(high).first.
template <typename T, typename Solve>
T wqs_binary_search(i64 low, i64 high, int required, Solve solve) {
	assert(low <= high && required >= 0);
	assert(solve(low).first >= required && solve(high).first <= required);
	while (low < high) {
		i64 mid = (i64)((i128)low + ((i128)high - low + 1) / 2);
		if (solve(mid).first >= required) low = mid;
		else high = mid - 1;
	}
	auto result = solve(low);
	return result.second - (T)low * required;
}

#endif
