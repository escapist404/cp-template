#ifndef MO_HPP
#define MO_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <utility>
#include <vector>

#include "bit.hpp"

// ============================================================================
// 【普通莫队与带修莫队离线区间查询算法 Mo's Algorithm (mo.hpp)】
// ----------------------------------------------------------------------------
// 1. 询问结构体签名（0-based 左闭右开区间 [l, r)）：
//    - struct MoQuery { int l, r, id; };
//    - struct ModifiedMoQuery { int l, r, time, id; };
// 2. 普通莫队函数签名：
//    template <typename Query, typename State, typename Add, typename Remove, typename Answer>
//    auto mo_algorithm(std::vector<Query> queries, int n, State& state,
//                      Add add, Remove remove, Answer answer, int block = 0)
//    回调签名：`void add(int idx, State& st)`, `void remove(int idx, State& st)`, `Result answer(const State& st, const Query& q)`。
//    复杂度：O((n + q) sqrt(n))。
// 3. 带修莫队函数签名：
//    template <typename Query, typename Update, typename State, typename Add, typename Remove,
//              typename Apply, typename Rollback, typename Answer>
//    auto mo_with_modify(std::vector<Query> queries, int n, const std::vector<Update>& updates,
//                        State& state, Add add, Remove remove, Apply apply, Rollback rollback,
//                        Answer answer, int block = 0)
//    额外回调：`void apply(const Update& u, int l, int r, State& st)` 与逆操作 `rollback`。复杂度 O((n + q) * n^(2/3))。
// ============================================================================

struct MoQuery {
	int l, r, id;
};
struct ModifiedMoQuery {
	int l, r, time, id;
};

// 普通离线莫队：所有询问区间均为 0-based 半开区间 [l, r)，id 为 [0, q) 的排列，State 引用传递。
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

// 带修莫队：Query 额外包含生效修改数 time，apply/rollback 需互为逆操作并在修改位置处于 [l, r) 内时同步更新窗口状态。
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
