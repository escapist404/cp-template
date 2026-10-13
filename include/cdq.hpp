#ifndef CDQ_HPP
#define CDQ_HPP

#include <cassert>
#include <vector>

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


// Ordinary CDQ: solve both halves, then cross-contribute/merge.
// merge(items, l, m, r) handles [l,m) -> [m,r) and restores temporary state.
// leaf(items, i) initializes/finalizes a singleton. Empty ranges do nothing.
template <typename T, typename Merge, typename Leaf>
void cdq_divide(std::vector<T>& items, Merge merge, Leaf leaf) {
	cdq_solve(items, merge, leaf, true);
}

// CDQ for DP dependencies: left -> cross contribution -> right.
// cross must preserve the original index order of both halves.
template <typename T, typename Cross, typename Leaf>
void cdq_dp(std::vector<T>& items, Cross cross, Leaf leaf) {
	cdq_solve(items, cross, leaf, false);
}

#endif
