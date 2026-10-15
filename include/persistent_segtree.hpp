#ifndef PERSISTENT_SEGTREE_HPP
#define PERSISTENT_SEGTREE_HPP

#include <cassert>
#include <utility>
#include <vector>

#include "types.hpp"

// ============================================================================
// 【可持久化线段树 / 主席树 Persistent Segment Tree (persistent_segtree.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <class V, auto op, auto e> class PersistentSegmentTree
//   维护 0-based 左闭右开区间 [0, n)（要求 n > 0），每次修改仅复制路径上 O(log n) 个节点并返回新版本根编号。
//   根编号 root == 0 始终代表全为 e() 的隐式初始空树版本。
// 构造函数与成员函数签名详解：
//   - PersistentSegmentTree(int n)
//     功能：指定值域/下标范围 [0, n)（要求 n > 0）。O(1)。
//   - int build(const std::vector<V>& init)
//     功能：使用长度为 n 的初始数组 init 建出完整初始版本树，返回新版本根编号。时空复杂度 O(n)。
//   - int set(int root, int p, V value)
//     功能：基于版本 root 将位置 p (0 <= p < n) 修改为 value，原版本不变，返回新版本根编号。时空 O(log n)。
//   - int add(int root, int p, V value)
//     功能：基于版本 root 将位置 p 修改为 op(get(root, p), value)，返回新版本根编号。时空 O(log n)。
//   - V prod(int root, int l, int r)
//     功能：在版本 root 上查询半开区间 [l, r) (0 <= l <= r <= n) 的聚合值。时间复杂度 O(log n)。
//   - V get(int root, int p)
//     功能：在版本 root 上查询单点 p (0 <= p < n) 的值。时间复杂度 O(log n)。
//   - V all_prod(int root)
//     功能：返回版本 root 在全区间 [0, n) 上的聚合值。时间复杂度 O(1)。
//   - template <class F> int max_right(int root, int l, F f)
//     功能：在版本 root 上从 l 向右树上二分（要求 f(e()) == true），返回最大的 r in [l, n]。O(log n)。
//   - template <class F> int min_left(int root, int r, F f)
//     功能：在版本 root 上从 r 向左树上二分（要求 f(e()) == true），返回最小的 l in [0, r]。O(log n)。
//   - int kth(int left_root, int right_root, i64 k)
//     功能：专用于权值线段树前缀版本作差（要求 V 为算术计数类型且每个位置 right - left >= 0），
//           返回差版本 (right_root - left_root) 中 0-based 第 k 小（0 <= k < 总频数差）的值域下标 in [0, n)。O(log n)。
// ============================================================================

template <class V, auto op, auto e>
class PersistentSegmentTree {
	struct Node {
		V value;
		int left, right;

		Node(V value = e(), int left = 0, int right = 0)
			: value(value), left(left), right(right) {}
	};

	int n;
	std::vector<Node> tree{{}};

	V value(int root) {
		return root ? tree[root].value : e();
	}

	template <class F>
	std::pair<int, bool> max_right_node(int root, int l, int r, int q, V& sum, F f) {
		if (r <= q) return {r, true};
		if (q <= l) {
			V next = op(sum, value(root));
			if (f(next)) {
				sum = next;
				return {r, true};
			}
			if (l + 1 == r) return {l, false};
		}
		int m = (l + r) / 2;
		if (q < m) {
			auto left = max_right_node(root ? tree[root].left : 0, l, m, q, sum, f);
			if (!left.second) return left;
		}
		return max_right_node(root ? tree[root].right : 0, m, r, q, sum, f);
	}

	template <class F>
	std::pair<int, bool> min_left_node(int root, int l, int r, int q, V& sum, F f) {
		if (q <= l) return {l, true};
		if (r <= q) {
			V next = op(value(root), sum);
			if (f(next)) {
				sum = next;
				return {l, true};
			}
			if (l + 1 == r) return {r, false};
		}
		int m = (l + r) / 2;
		if (m < q) {
			auto right = min_left_node(root ? tree[root].right : 0, m, r, q, sum, f);
			if (!right.second) return right;
		}
		return min_left_node(root ? tree[root].left : 0, l, m, q, sum, f);
	}

	int clone(int root) {
		tree.push_back(tree[root]);
		return (int)tree.size() - 1;
	}

	// int build(const std::vector<V>& init) — 功能：使用长度为 n 的初始数组 init 建出完整初始版本树，返回新版本根编号。时空复杂度 O(n)。
	int build(const std::vector<V>& init, int l, int r) {
		if (l + 1 == r) {
			tree.emplace_back(init[l]);
			return (int)tree.size() - 1;
		}
		int m = (l + r) / 2;
		int left = build(init, l, m);
		int right = build(init, m, r);
		tree.emplace_back(op(tree[left].value, tree[right].value), left, right);
		return (int)tree.size() - 1;
	}

	// int set(int root, int p, V value) — 功能：基于版本 root 将位置 p (0 <= p < n) 修改为 value，原版本不变，返回新版本根编号。时空 O(log n)。
	int set(int root, int l, int r, int p, V value) {
		int result = clone(root);
		if (l + 1 == r) {
			tree[result].value = value;
			return result;
		}
		int m = (l + r) / 2;
		if (p < m)
			tree[result].left = set(tree[root].left, l, m, p, value);
		else
			tree[result].right = set(tree[root].right, m, r, p, value);
		tree[result].value = op(tree[tree[result].left].value, tree[tree[result].right].value);
		return result;
	}

	// V prod(int root, int l, int r) — 功能：在版本 root 上查询半开区间 [l, r) (0 <= l <= r <= n) 的聚合值。时间复杂度 O(log n)。
	V prod(int root, int l, int r, int L, int R) {
		if (!root || R <= l || r <= L) return e();
		if (L <= l && r <= R) return tree[root].value;
		int m = (l + r) / 2;
		return op(prod(tree[root].left, l, m, L, R), prod(tree[root].right, m, r, L, R));
	}

	// int kth(int left_root, int right_root, i64 k) — 功能：专用于权值线段树前缀版本作差（要求 V 为算术计数类型且每个位置 right - left >= 0），
	int kth(int left_root, int right_root, int l, int r, i64 k) {
		if (l + 1 == r) return l;
		int m = (l + r) / 2;
		int left_before = tree[left_root].left;
		int left_after = tree[right_root].left;
		i64 count = (i64)(tree[left_after].value - tree[left_before].value);
		if (k < count) return kth(left_before, left_after, l, m, k);
		return kth(tree[left_root].right, tree[right_root].right, m, r, k - count);
	}

public:
	PersistentSegmentTree(int n) : n(n) { assert(n > 0); }

	int build(const std::vector<V>& init) {
		assert((int)init.size() == n);
		return build(init, 0, n);
	}

	int set(int root, int p, V value) {
		assert(0 <= p && p < n);
		return set(root, 0, n, p, value);
	}

	// int add(int root, int p, V value) — 功能：基于版本 root 将位置 p 修改为 op(get(root, p), value)，返回新版本根编号。时空 O(log n)。
	int add(int root, int p, V value) {
		assert(0 <= p && p < n);
		return set(root, p, op(get(root, p), value));
	}

	V prod(int root, int l, int r) {
		assert(0 <= l && l <= r && r <= n);
		return prod(root, 0, n, l, r);
	}

	V get(int root, int p) { return prod(root, p, p + 1); }

	V all_prod(int root) { return tree[root].value; }

	// template <class F> int max_right(int root, int l, F f) — 功能：在版本 root 上从 l 向右树上二分（要求 f(e()) == true），返回最大的 r in [l, n]。O(log n)。
	template <class F>
	int max_right(int root, int l, F f) {
		assert(0 <= l && l <= n && f(e()));
		if (l == n) return n;
		V sum = e();
		return max_right_node(root, 0, n, l, sum, f).first;
	}

	// template <class F> int min_left(int root, int r, F f) — 功能：在版本 root 上从 r 向左树上二分（要求 f(e()) == true），返回最小的 l in [0, r]。O(log n)。
	template <class F>
	int min_left(int root, int r, F f) {
		assert(0 <= r && r <= n && f(e()));
		if (r == 0) return 0;
		V sum = e();
		return min_left_node(root, 0, n, r, sum, f).first;
	}

	// 仅用于权值线段树非负频数差分：
	// 返回 right_root - left_root 中 0-based 第 k 小的值域下标
	int kth(int left_root, int right_root, i64 k) {
		assert(0 <= left_root && left_root < (int)tree.size());
		assert(0 <= right_root && right_root < (int)tree.size());
		assert(0 <= k && k < (i64)(tree[right_root].value - tree[left_root].value));
		return kth(left_root, right_root, 0, n, k);
	}
};

#endif
