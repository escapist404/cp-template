#ifndef CDQ_HPP
#define CDQ_HPP

#include <cassert>
#include <vector>

// ============================================================================
// 【CDQ 分治框架：三维偏序与带依赖分治 DP (cdq.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解（所有区间均为 0-based 左闭右开 [l, r)）：
//   1. template <typename T, typename Merge, typename Leaf>
//      void cdq_divide(std::vector<T>& items, Merge merge, Leaf leaf)
//      标准后序 CDQ（三维偏序）：先递归 [l, m) -> 再递归 [m, r) -> 最后调用 `merge(items, l, m, r)`。
//   2. template <typename T, typename Cross, typename Leaf>
//      void cdq_dp(std::vector<T>& items, Cross cross, Leaf leaf)
//      中序 CDQ（1D/1D 分治 DP）：先递归 [l, m) -> 立即调用 `cross(items, l, m, r)` 转移给右半 -> 最后递归 [m, r)。
//   3. template <typename T, typename Cross, typename Leaf>
//      void cdq_solve(std::vector<T>& items, Cross cross, Leaf leaf, bool cross_after_right)
//      底层分治引擎：单点叶子调用 `leaf(items, i)`，跨区间调用 `cross(items, l, m, r)`。
// ============================================================================

template <typename T, typename Cross, typename Leaf>
void cdq_solve(std::vector<T>& items, Cross cross, Leaf leaf, bool cross_after_right) {
	const auto divide = [&](int l, int r, const auto& self) -> void {
		if (l == r) return;
		if (r - l == 1) {
			leaf(items, l);
			return;
		}
		const int m = l + (r - l) / 2;
		self(l, m, self);
		if (!cross_after_right) cross(items, l, m, r);
		self(m, r, self);
		if (cross_after_right) cross(items, l, m, r);
	};
	divide(0, items.size(), divide);
}


// 标准归并型 CDQ 分治：先递归求解左半 [l, m) 与右半 [m, r)，再调用 merge(items, l, m, r) 统计跨区间贡献并归并。
template <typename T, typename Merge, typename Leaf>
void cdq_divide(std::vector<T>& items, Merge merge, Leaf leaf) {
	cdq_solve(items, merge, leaf, true);
}

// DP 转移型 CDQ 分治（中序分治）：递归左半 [l, m) -> 调用 cross(items, l, m, r) 用左半转移右半 -> 递归右半 [m, r)。
template <typename T, typename Cross, typename Leaf>
void cdq_dp(std::vector<T>& items, Cross cross, Leaf leaf) {
	cdq_solve(items, cross, leaf, false);
}

#endif
