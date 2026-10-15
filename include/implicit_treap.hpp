#ifndef IMPLICIT_TREAP_HPP
#define IMPLICIT_TREAP_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <random>
#include <utility>
#include <vector>

// ============================================================================
// 【文艺平衡树 / 无旋 Treap (implicit_treap.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T = int, typename Merge = std::plus<T>> class ImplicitTreap
//   按下标分裂的无旋 Treap（FHQ-Treap），维护 0-based 动态序列，所有区间操作均采用【左闭右开 [l, r)】。
//   每个节点同时维护正向聚合值 forward 与反向聚合值 backward，即便 Merge 不满足交换律（如矩阵乘法、字符串哈希），区间翻转后查询依然完全正确。
// 构造函数与成员函数签名详解：
//   - ImplicitTreap(std::vector<T> values = {}, Merge merge = Merge(), T identity = T(), std::uint32_t seed = 123456789u)
//     功能：用初始序列 values、二元结合律合并算子 merge、单位元 identity 及随机种子构造平衡树。O(n log n)。
//   - int size()
//     功能：返回当前序列长度 n。时间复杂度 O(1)。
//   - void insert(int position, T value)
//     功能：在 0-based 下标 position (0 <= position <= size()) 之前插入单个元素 value。期望 O(log n)。
//   - void erase(int l, int r)
//     功能：删除左闭右开区间 [l, r) (0 <= l <= r <= size()) 内的所有元素。期望 O(log n)。
//   - void reverse(int l, int r)
//     功能：翻转左闭右开区间 [l, r) (0 <= l <= r <= size()) 内的元素顺序。期望 O(log n)。
//   - void move(int l, int r, int p)
//     功能：将子区间 [l, r) 剪下，并插入到【移除 [l, r) 之后的剩余序列】的下标 p 处（要求 0 <= p <= size() - (r - l)）。期望 O(log n)。
//   - void set(int position, T value)
//     功能：将下标 position (0 <= position < size()) 处的元素修改为 value。期望 O(log n)。
//   - T get(int position)
//     功能：返回下标 position (0 <= position < size()) 处的元素值。期望 O(log n)。
//   - T query(int l, int r)
//     功能：查询区间 [l, r) (0 <= l <= r <= size()) 从左到右按 merge 聚合的结果（空区间返回 identity）。期望 O(log n)。
//   - std::vector<T> values()
//     功能：中序遍历导出当前序列的全部元素（会下推懒标记）。时间复杂度 O(n)。
// ============================================================================

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
	// ImplicitTreap(std::vector<T> values = {}, Merge merge = Merge(), T identity = T(), std::uint32_t seed = 123456789u) — 功能：用初始序列 values、二元结合律合并算子 merge、单位元 identity 及随机种子构造平衡树。O(n log n)。
	ImplicitTreap(std::vector<T> values = {}, Merge merge = Merge(), T identity = T(),
		unsigned seed = 712367821)
		: rng(seed), merge(merge), identity(identity) {
		tree.push_back({identity, identity, identity, 0, 0, 0, false, 0});
		for (T value : values) insert(size(), value);
	}

	int size() { return size(root); }

	// void insert(int position, T value) — 功能：在 0-based 下标 position (0 <= position <= size()) 之前插入单个元素 value。期望 O(log n)。
	void insert(int position, T value) {
		assert(0 <= position && position <= size());
		auto parts = split(root, position);
		root = merge_nodes(merge_nodes(parts.first, new_node(value)), parts.second);
	}

	// void erase(int l, int r) — 功能：删除左闭右开区间 [l, r) (0 <= l <= r <= size()) 内的所有元素。期望 O(log n)。
	void erase(int l, int r) {
		assert(0 <= l && l <= r && r <= size());
		auto first = split(root, l);
		auto second = split(first.second, r - l);
		root = merge_nodes(first.first, second.second);
	}

	// void reverse(int l, int r) — 功能：翻转左闭右开区间 [l, r) (0 <= l <= r <= size()) 内的元素顺序。期望 O(log n)。
	void reverse(int l, int r) {
		assert(0 <= l && l <= r && r <= size());
		auto first = split(root, l);
		auto second = split(first.second, r - l);
		reverse_node(second.first);
		root = merge_nodes(first.first, merge_nodes(second.first, second.second));
	}

	// 将区间 [l, r) 剪下并插入到移除 [l, r) 后的剩余序列下标 p 处
	void move(int l, int r, int p) {
		assert(0 <= l && l <= r && r <= size());
		assert(0 <= p && p <= size() - (r - l));
		auto first = split(root, l);
		auto second = split(first.second, r - l);
		auto parts = split(merge_nodes(first.first, second.second), p);
		root = merge_nodes(parts.first, merge_nodes(second.first, parts.second));
	}

	// void set(int position, T value) — 功能：将下标 position (0 <= position < size()) 处的元素修改为 value。期望 O(log n)。
	void set(int position, T value) {
		assert(0 <= position && position < size());
		auto first = split(root, position);
		auto second = split(first.second, 1);
		tree[second.first].value = value;
		pull(second.first);
		root = merge_nodes(first.first, merge_nodes(second.first, second.second));
	}

	// T query(int l, int r) — 功能：查询区间 [l, r) (0 <= l <= r <= size()) 从左到右按 merge 聚合的结果（空区间返回 identity）。期望 O(log n)。
	T query(int l, int r) {
		assert(0 <= l && l <= r && r <= size());
		auto first = split(root, l);
		auto second = split(first.second, r - l);
		T result = forward(second.first);
		root = merge_nodes(first.first, merge_nodes(second.first, second.second));
		return result;
	}

	// T get(int position) — 功能：返回下标 position (0 <= position < size()) 处的元素值。期望 O(log n)。
	T get(int position) {
		assert(0 <= position && position < size());
		return query(position, position + 1);
	}
};

#endif
