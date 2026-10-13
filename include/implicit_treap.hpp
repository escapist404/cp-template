#ifndef IMPLICIT_TREAP_HPP
#define IMPLICIT_TREAP_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <random>
#include <utility>
#include <vector>

// 0-based sequence, [l, r). Merge is associative, identity is a two-sided unit.
// Both directions are stored so reverse also supports noncommutative Merge.
template <typename T = int, typename Merge = std::plus<T>>
class ImplicitTreap {
	struct Node {
		T value, forward, backward;
		int left = 0, right = 0, size = 1;
		bool reversed = false;
		unsigned priority;
	};
	std::vector<Node> tree;
	std::mt19937 rng;
	Merge merge;
	T identity;
	int root = 0;

	int size(int p) { return p ? tree[p].size : 0; }
	T forward(int p) { return p ? tree[p].forward : identity; }
	T backward(int p) { return p ? tree[p].backward : identity; }

	void pull(int p) {
		Node& x = tree[p];
		x.size = 1 + size(x.left) + size(x.right);
		x.forward = merge(merge(forward(x.left), x.value), forward(x.right));
		x.backward = merge(merge(backward(x.right), x.value), backward(x.left));
	}

	void reverse_node(int p) {
		if (!p) return;
		std::swap(tree[p].left, tree[p].right);
		std::swap(tree[p].forward, tree[p].backward);
		tree[p].reversed ^= 1;
	}

	void push(int p) {
		if (!tree[p].reversed) return;
		reverse_node(tree[p].left);
		reverse_node(tree[p].right);
		tree[p].reversed = false;
	}

	int new_node(T value) {
		tree.push_back({value, value, value, 0, 0, 1, false, rng()});
		return tree.size() - 1;
	}

	std::pair<int, int> split(int p, int count) {
		if (!p) return {0, 0};
		push(p);
		if (size(tree[p].left) >= count) {
			auto parts = split(tree[p].left, count);
			tree[p].left = parts.second;
			pull(p);
			return {parts.first, p};
		}
		auto parts = split(tree[p].right, count - size(tree[p].left) - 1);
		tree[p].right = parts.first;
		pull(p);
		return {p, parts.second};
	}

	int merge_nodes(int a, int b) {
		if (!a || !b) return a ? a : b;
		if (tree[a].priority > tree[b].priority) {
			push(a);
			tree[a].right = merge_nodes(tree[a].right, b);
			pull(a);
			return a;
		}
		push(b);
		tree[b].left = merge_nodes(a, tree[b].left);
		pull(b);
		return b;
	}

public:
	ImplicitTreap(std::vector<T> values = {}, Merge merge = Merge(), T identity = T(),
		unsigned seed = 712367821)
		: rng(seed), merge(merge), identity(identity) {
		tree.push_back({identity, identity, identity, 0, 0, 0, false, 0});
		for (T value : values) insert(size(), value);
	}

	int size() { return size(root); }

	void insert(int position, T value) {
		assert(0 <= position && position <= size());
		auto parts = split(root, position);
		root = merge_nodes(merge_nodes(parts.first, new_node(value)), parts.second);
	}

	void erase(int l, int r) {
		assert(0 <= l && l <= r && r <= size());
		auto first = split(root, l);
		auto second = split(first.second, r - l);
		root = merge_nodes(first.first, second.second);
	}

	void reverse(int l, int r) {
		assert(0 <= l && l <= r && r <= size());
		auto first = split(root, l);
		auto second = split(first.second, r - l);
		reverse_node(second.first);
		root = merge_nodes(first.first, merge_nodes(second.first, second.second));
	}

	// Insert [l, r) at position p in the sequence AFTER removing [l, r).
	void move(int l, int r, int p) {
		assert(0 <= l && l <= r && r <= size());
		assert(0 <= p && p <= size() - (r - l));
		auto first = split(root, l);
		auto second = split(first.second, r - l);
		auto parts = split(merge_nodes(first.first, second.second), p);
		root = merge_nodes(parts.first, merge_nodes(second.first, parts.second));
	}

	void set(int position, T value) {
		assert(0 <= position && position < size());
		auto first = split(root, position);
		auto second = split(first.second, 1);
		tree[second.first].value = value;
		pull(second.first);
		root = merge_nodes(first.first, merge_nodes(second.first, second.second));
	}

	T query(int l, int r) {
		assert(0 <= l && l <= r && r <= size());
		auto first = split(root, l);
		auto second = split(first.second, r - l);
		T result = forward(second.first);
		root = merge_nodes(first.first, merge_nodes(second.first, second.second));
		return result;
	}

	T get(int position) {
		assert(0 <= position && position < size());
		return query(position, position + 1);
	}
};

#endif
