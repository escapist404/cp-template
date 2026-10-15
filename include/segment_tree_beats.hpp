#ifndef SEGMENT_TREE_BEATS_HPP
#define SEGMENT_TREE_BEATS_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

// 【吉司机线段树 Segment Tree Beats (segment_tree_beats.hpp)】
// - 0-based 左闭右开区间 [l, r)，同时支持区间取 min、取 max、区间加与区间和/最值查询，均摊复杂度 O(log^2 n)
// - 模板参数: SegmentTreeBeats<T, Sum>，所有元素与标记中间值必须严格处于 (numeric_limits<T>::lowest(), max()) 内部
// - add(l, r, v)   : 区间 [l, r) 每个元素加 v
// - chmin(l, r, v) : 区间 [l, r) 每个元素变为 min(a[i], v)
// - chmax(l, r, v) : 区间 [l, r) 每个元素变为 max(a[i], v)
// - sum(l, r)      : 查询 [l, r) 元素和 (0 <= l <= r <= n)
// - min / max(l, r): 查询 [l, r) 最值（要求区间非空 0 <= l < r <= n）

template <typename T = long long, typename Sum = T>
class SegmentTreeBeats {
	struct Node {
		Sum sum;
		T max1, max2, min1, min2, add;
		int max_count, min_count;
	};
	int n;
	std::vector<Node> tree;

	static T neg_inf() { return std::numeric_limits<T>::lowest(); }
	static T pos_inf() { return std::numeric_limits<T>::max(); }

	void pull(int p) {
		Node& a = tree[p << 1], &b = tree[p << 1 | 1];
		Node& x = tree[p];
		x.sum = a.sum + b.sum;
		x.max1 = std::max(a.max1, b.max1);
		x.max_count = 0;
		x.max2 = neg_inf();
		if (a.max1 == x.max1) x.max_count += a.max_count, x.max2 = std::max(x.max2, a.max2);
		else x.max2 = std::max(x.max2, a.max1);
		if (b.max1 == x.max1) x.max_count += b.max_count, x.max2 = std::max(x.max2, b.max2);
		else x.max2 = std::max(x.max2, b.max1);
		x.min1 = std::min(a.min1, b.min1);
		x.min_count = 0;
		x.min2 = pos_inf();
		if (a.min1 == x.min1) x.min_count += a.min_count, x.min2 = std::min(x.min2, a.min2);
		else x.min2 = std::min(x.min2, a.min1);
		if (b.min1 == x.min1) x.min_count += b.min_count, x.min2 = std::min(x.min2, b.min2);
		else x.min2 = std::min(x.min2, b.min1);
		x.add = T();
	}

	void apply_add(int p, T value, int length) {
		Node& x = tree[p];
		x.sum += (Sum)value * length;
		x.max1 += value;
		if (x.max2 != neg_inf()) x.max2 += value;
		x.min1 += value;
		if (x.min2 != pos_inf()) x.min2 += value;
		x.add += value;
	}

	void apply_max(int p, T value) {
		Node& x = tree[p];
		x.sum += ((Sum)value - x.max1) * x.max_count;
		if (x.min1 == x.max1) x.min1 = value;
		else if (x.min2 == x.max1) x.min2 = value;
		x.max1 = value;
	}

	void apply_min(int p, T value) {
		Node& x = tree[p];
		x.sum += ((Sum)value - x.min1) * x.min_count;
		if (x.max1 == x.min1) x.max1 = value;
		else if (x.max2 == x.min1) x.max2 = value;
		x.min1 = value;
	}

	void push(int p, int length) {
		if (tree[p].add != T()) {
			apply_add(p << 1, tree[p].add, length / 2);
			apply_add(p << 1 | 1, tree[p].add, length - length / 2);
			tree[p].add = T();
		}
		if (tree[p << 1].max1 > tree[p].max1) apply_max(p << 1, tree[p].max1);
		if (tree[p << 1 | 1].max1 > tree[p].max1) apply_max(p << 1 | 1, tree[p].max1);
		if (tree[p << 1].min1 < tree[p].min1) apply_min(p << 1, tree[p].min1);
		if (tree[p << 1 | 1].min1 < tree[p].min1) apply_min(p << 1 | 1, tree[p].min1);
	}

	void build(int p, int l, int r, const std::vector<T>& a) {
		if (l + 1 == r) {
			T value = a[l];
			tree[p] = {value, value, neg_inf(), value, pos_inf(), T(), 1, 1};
			return;
		}
		int m = (l + r) / 2;
		build(p << 1, l, m, a);
		build(p << 1 | 1, m, r, a);
		pull(p);
	}

	void add(int p, int l, int r, int L, int R, T value) {
		if (R <= l || r <= L) return;
		if (L <= l && r <= R) return apply_add(p, value, r - l);
		push(p, r - l);
		int m = (l + r) / 2;
		add(p << 1, l, m, L, R, value);
		add(p << 1 | 1, m, r, L, R, value);
		pull(p);
	}

	void chmin(int p, int l, int r, int L, int R, T value) {
		if (R <= l || r <= L || tree[p].max1 <= value) return;
		if (L <= l && r <= R && (tree[p].max2 < value || tree[p].max_count == r - l)) return apply_max(p, value);
		push(p, r - l);
		int m = (l + r) / 2;
		chmin(p << 1, l, m, L, R, value);
		chmin(p << 1 | 1, m, r, L, R, value);
		pull(p);
	}

	void chmax(int p, int l, int r, int L, int R, T value) {
		if (R <= l || r <= L || tree[p].min1 >= value) return;
		if (L <= l && r <= R && (tree[p].min2 > value || tree[p].min_count == r - l)) return apply_min(p, value);
		push(p, r - l);
		int m = (l + r) / 2;
		chmax(p << 1, l, m, L, R, value);
		chmax(p << 1 | 1, m, r, L, R, value);
		pull(p);
	}

	Sum sum(int p, int l, int r, int L, int R) {
		if (R <= l || r <= L) return Sum();
		if (L <= l && r <= R) return tree[p].sum;
		push(p, r - l);
		int m = (l + r) / 2;
		return sum(p << 1, l, m, L, R) + sum(p << 1 | 1, m, r, L, R);
	}

	T extreme(int p, int l, int r, int L, int R, bool maximum) {
		if (R <= l || r <= L) return maximum ? neg_inf() : pos_inf();
		if (L <= l && r <= R) return maximum ? tree[p].max1 : tree[p].min1;
		push(p, r - l);
		int m = (l + r) / 2;
		T a = extreme(p << 1, l, m, L, R, maximum);
		T b = extreme(p << 1 | 1, m, r, L, R, maximum);
		return maximum ? std::max(a, b) : std::min(a, b);
	}

	static int assert_size(int n) { assert(n >= 0); return n; }

public:
	SegmentTreeBeats(const std::vector<T>& a) : n(a.size()), tree(std::max(1, n * 4)) {
		if (n) build(1, 0, n, a);
	}

	SegmentTreeBeats(int n, T value = T()) : SegmentTreeBeats(std::vector<T>(assert_size(n), value)) {}

	void add(int l, int r, T value) { assert(0 <= l && l <= r && r <= n); add(1, 0, n, l, r, value); }
	void chmin(int l, int r, T value) { assert(0 <= l && l <= r && r <= n); chmin(1, 0, n, l, r, value); }
	void chmax(int l, int r, T value) { assert(0 <= l && l <= r && r <= n); chmax(1, 0, n, l, r, value); }
	Sum sum(int l, int r) { assert(0 <= l && l <= r && r <= n); return sum(1, 0, n, l, r); }
	T min(int l, int r) { assert(0 <= l && l < r && r <= n); return extreme(1, 0, n, l, r, false); }
	T max(int l, int r) { assert(0 <= l && l < r && r <= n); return extreme(1, 0, n, l, r, true); }
};

#endif
