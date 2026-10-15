#ifndef PERSISTENT_SEGTREE_HPP
#define PERSISTENT_SEGTREE_HPP

#include <cassert>
#include <utility>
#include <vector>

#include "types.hpp"

// 【可持久化线段树 / 主席树 (persistent_segtree.hpp)】
// - 0-based 左闭右开值域 [0, n)（要求 n > 0），每次修改仅复制 O(log n) 个节点并返回新版本根编号
// - 节点 0 为全 e() 的隐式空树版本；build(init) 可用长度为 n 的数组建初始版本并返回根编号
// - set(root, p, v) / add(root, p, v): 基于版本 root 修改位置 p，返回新版本根编号（原版本不变）
// - prod(root, l, r) / get(root, p) / all_prod(root) / max_right / min_left: 在指定版本 root 上查询或二分
// - kth(left_root, right_root, k): 专用于权值线段树两版本作差，返回 0-based 第 k 小的值域下标

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

	V prod(int root, int l, int r, int L, int R) {
		if (!root || R <= l || r <= L) return e();
		if (L <= l && r <= R) return tree[root].value;
		int m = (l + r) / 2;
		return op(prod(tree[root].left, l, m, L, R), prod(tree[root].right, m, r, L, R));
	}

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

	template <class F>
	int max_right(int root, int l, F f) {
		assert(0 <= l && l <= n && f(e()));
		if (l == n) return n;
		V sum = e();
		return max_right_node(root, 0, n, l, sum, f).first;
	}

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
