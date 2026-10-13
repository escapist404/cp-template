#ifndef MO_HPP
#define MO_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <utility>
#include <vector>

#include "bit.hpp"

struct MoQuery {
	int l, r, id;
};
struct ModifiedMoQuery {
	int l, r, time, id;
};

// All ranges are [l,r). Query may be any type with l,r,id members;
// id must be a permutation of [0, queries.size()).
// State may be any type; answer(state, query) returns the result type.
// add/remove(index, state) maintain an order-independent window aggregate.
// Callbacks may capture the underlying array; the supplied State is initially
// the aggregate of the empty window. It is passed by reference, without copies.
template <typename Query, typename State, typename Add, typename Remove, typename Answer>
auto mo_algorithm(std::vector<Query> queries, int n, State& state,
	Add add, Remove remove, Answer answer, int block = 0) {
	using Result = std::decay_t<decltype(answer(state, std::declval<const Query&>()))>;
	assert(n >= 0 && block >= 0);
	if (!block) {
		block = std::max(1, floor_sqrt(std::max(1, n)));
	}
	std::sort(queries.begin(), queries.end(), [block](const Query& a, const Query& b) {
		const int ab = a.l / block, bb = b.l / block;
		return ab != bb ? ab < bb : ((ab & 1) ? a.r > b.r : a.r < b.r);
	});
	std::vector<Result> result(queries.size());
	int l = 0, r = 0;
	for (const auto& q : queries) {
		assert(0 <= q.l && q.l <= q.r && q.r <= n && 0 <= q.id && q.id < (int)queries.size());
		while (l > q.l) {
			add(--l, state);
		}
		while (r < q.r) {
			add(r++, state);
		}
		while (l < q.l) {
			remove(l++, state);
		}
		while (r > q.r) {
			remove(--r, state);
		}
		result[q.id] = answer(state, q);
	}
	return result;
}

// Query also has time = number of preceding updates.
// apply(update,l,r,state), rollback(update,l,r,state) must update the array
// AND the window aggregate if update.pos is in [l,r). They are exact inverses.
// Array and state must start at time zero. Result type is inferred from answer.
// Array/state are left at the final processed window/time when this returns.
template <typename Query, typename Update, typename State, typename Add, typename Remove,
	typename Apply, typename Rollback, typename Answer>
auto mo_with_modify(std::vector<Query> queries, int n, const std::vector<Update>& updates,
	State& state, Add add, Remove remove, Apply apply, Rollback rollback,
	Answer answer, int block = 0) {
	using Result = std::decay_t<decltype(answer(state, std::declval<const Query&>()))>;
	assert(n >= 0 && block >= 0);
	if (!block) {
		block = std::max(1, (int)std::pow(std::max(1, n), 2.0 / 3.0));
	}
	std::sort(queries.begin(), queries.end(), [block](const Query& a, const Query& b) {
		const int ab = a.l / block, bb = b.l / block;
		if (ab != bb) {
			return ab < bb;
		}
		const int ar = a.r / block, br = b.r / block;
		if (ar != br) {
			return (ab & 1) ? ar > br : ar < br;
		}
		return (ar & 1) ? a.time > b.time : a.time < b.time;
	});
	std::vector<Result> result(queries.size());
	int l = 0, r = 0, time = 0;
	for (const auto& q : queries) {
		assert(0 <= q.l && q.l <= q.r && q.r <= n && 0 <= q.id && q.id < (int)queries.size());
		assert(0 <= q.time && q.time <= (int)updates.size());
		while (time < q.time) {
			apply(updates[time++], l, r, state);
		}
		while (time > q.time) {
			rollback(updates[--time], l, r, state);
		}
		while (l > q.l) {
			add(--l, state);
		}
		while (r < q.r) {
			add(r++, state);
		}
		while (l < q.l) {
			remove(l++, state);
		}
		while (r > q.r) {
			remove(--r, state);
		}
		result[q.id] = answer(state, q);
	}
	return result;
}

#endif
