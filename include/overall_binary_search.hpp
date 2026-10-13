#ifndef OVERALL_BINARY_SEARCH_HPP
#define OVERALL_BINARY_SEARCH_HPP

#include <cassert>
#include <utility>
#include <vector>
#include "binary_search.hpp"

// Stable partition of a chronological event sequence over inclusive [low, high].
// route(event, mid) returns true for the left half. It may apply updates and
// reduce a query's remaining rank when routed right. rollback() restores the
// temporary data structure after EACH partition, before either child runs.
// finish(event, value) records query answers; it may ignore update events.
// Event and value types are generic. Include a sentinel in the domain if needed.
template <typename Event, typename Integer, typename Route, typename Rollback, typename Finish>
void overall_binary_search(std::vector<Event> events, Integer low, Integer high,
	Route route, Rollback rollback, Finish finish) {
	assert(low <= high);
	const auto solve = [&](std::vector<Event>& items, Integer l, Integer r, const auto& self) -> void {
		if (items.empty()) {
			return;
		}
		if (l == r) {
			for (auto& event : items) {
				finish(event, l);
			}
			return;
		}
		const Integer mid = binary_midpoint(l, r);
		std::vector<Event> left, right;
		for (auto& event : items) {
			if (route(event, mid)) {
				left.push_back(std::move(event));
			} else {
				right.push_back(std::move(event));
			}
		}
		rollback();
		std::vector<Event>().swap(items);
		self(left, l, mid, self);
		rollback();
		self(right, mid + 1, r, self);
	};
	solve(events, low, high, solve);
}

#endif
