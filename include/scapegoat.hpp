#ifndef SCAPEGOAT_HPP
#define SCAPEGOAT_HPP

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <utility>
#include <vector>

// ============================================================================
// 【替罪羊树 Scapegoat Tree (scapegoat.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T = int, typename Compare = std::less<T>> class ScapegoatTree
//   基于重量平衡因子 alpha = 2/3 暴力拍平重构的有序多重集合，禁用拷贝构造与拷贝赋值。
// 构造函数与成员函数签名详解（所有修改与查询均摊时间复杂度 O(log n)）：
//   - ScapegoatTree() / ScapegoatTree(Compare compare)
//     功能：构造空替罪羊树。O(1)。
//   - int size() const / bool empty() const
//     功能：返回集合中元素的总个数（含重复元素） / 判断集合是否为空。O(1)。
//   - void insert(T key)
//     功能：插入一个元素 key；若插入后路径深度超过阈值则触发最高失衡祖先的 O(size) 重构。均摊 O(log n)。
//   - void remove(T key)
//     功能：删除一个等于 key 的元素副本（若 key 不存在则无操作）。均摊 O(log n)。
//   - int get_rank(T key) const
//     功能：返回 key 的 1-based 排名（严格小于 key 的元素个数 + 1）。均摊 O(log n)。
//   - T get_value(int rank) const
//     功能：返回 1-based 排名为 rank 的元素值（要求 1 <= rank <= size()）。均摊 O(log n)。
//   - T get_prev(T key) const
//     功能：返回严格小于 key 的最大元素（不存在返回 std::numeric_limits<T>::lowest()）。均摊 O(log n)。
//   - T get_next(T key) const
//     功能：返回严格大于 key 的最小元素（不存在返回 std::numeric_limits<T>::max()）。均摊 O(log n)。
// ============================================================================

template <typename T = int, typename Compare = std::less<T>>
class ScapegoatTree {
	static constexpr double alpha = 2.0 / 3.0;

	struct Node {
		Node* left;
		Node* right;
		T key;
		int count;
		int size;
		int nodes;

		Node(T key)
			: left(nullptr), right(nullptr), key(key), count(1), size(1), nodes(1) {}

		void maintain() {
			size = count;
			nodes = 1;
			if (left) {
				size += left->size;
				nodes += left->nodes;
			}
			if (right) {
				size += right->size;
				nodes += right->nodes;
			}
		}
	};

	Node* root = nullptr;
	int max_nodes = 0;
	Compare compare;

	bool equal(T lhs, T rhs) const {
		return !compare(lhs, rhs) && !compare(rhs, lhs);
	}

	static int node_count(Node* cur) {
		return cur ? cur->nodes : 0;
	}

	static int element_count(Node* cur) {
		return cur ? cur->size : 0;
	}

	// void insert(T key) — 功能：插入一个元素 key；若插入后路径深度超过阈值则触发最高失衡祖先的 O(size) 重构。均摊 O(log n)。
	Node* insert(Node* cur, T key, std::vector<Node*>& path, bool& inserted_node) {
		if (!cur) {
			Node* node = new Node(key);
			path.push_back(node);
			inserted_node = true;
			return node;
		}

		path.push_back(cur);
		if (equal(key, cur->key)) {
			++cur->count;
			cur->maintain();
			return cur;
		}
		if (compare(key, cur->key)) {
			cur->left = insert(cur->left, key, path, inserted_node);
		} else {
			cur->right = insert(cur->right, key, path, inserted_node);
		}
		cur->maintain();
		return cur;
	}

	static void flatten(Node* cur, std::vector<Node*>& nodes) {
		if (!cur) return;
		flatten(cur->left, nodes);
		nodes.push_back(cur);
		flatten(cur->right, nodes);
	}

	static Node* build_balanced(std::vector<Node*>& nodes, int left, int right) {
		if (left >= right) return nullptr;
		int middle = left + (right - left) / 2;
		Node* cur = nodes[middle];
		cur->left = build_balanced(nodes, left, middle);
		cur->right = build_balanced(nodes, middle + 1, right);
		cur->maintain();
		return cur;
	}

	static Node* rebuild(Node* cur) {
		std::vector<Node*> nodes;
		nodes.reserve(cur->nodes);
		flatten(cur, nodes);
		return build_balanced(nodes, 0, (int)nodes.size());
	}

	static Node* detach_max(Node* cur, Node*& maximum) {
		if (!cur->right) {
			maximum = cur;
			return cur->left;
		}
		cur->right = detach_max(cur->right, maximum);
		cur->maintain();
		return cur;
	}

	Node* erase(Node* cur, T key, bool& removed) {
		if (!cur) return nullptr;
		if (compare(key, cur->key)) {
			cur->left = erase(cur->left, key, removed);
		} else if (compare(cur->key, key)) {
			cur->right = erase(cur->right, key, removed);
		} else if (cur->count > 1) {
			--cur->count;
			removed = true;
		} else {
			removed = true;
			if (!cur->left) {
				Node* next = cur->right;
				delete cur;
				return next;
			}
			if (!cur->right) {
				Node* next = cur->left;
				delete cur;
				return next;
			}
			Node* predecessor = nullptr;
			cur->left = detach_max(cur->left, predecessor);
			predecessor->left = cur->left;
			predecessor->right = cur->right;
			predecessor->maintain();
			delete cur;
			return predecessor;
		}
		cur->maintain();
		return cur;
	}

	static void clear(Node* cur) {
		if (!cur) return;
		clear(cur->left);
		clear(cur->right);
		delete cur;
	}

	static T value_at(Node* cur, int rank) {
		int left_size = element_count(cur->left);
		if (rank <= left_size) {
			return value_at(cur->left, rank);
		}
		if (rank <= left_size + cur->count) {
			return cur->key;
		}
		return value_at(cur->right, rank - left_size - cur->count);
	}

	static int allowed_depth(int nodes) {
		return (int)(std::log((double)nodes) / std::log(1.0 / alpha));
	}

	void rebuild_scapegoat(std::vector<Node*>& path) {
		for (int i = (int)path.size() - 2; i >= 0; --i) {
			Node* parent = path[i];
			Node* child = path[i + 1];
			if (3LL * node_count(child) <= 2LL * node_count(parent)) {
				continue;
			}
			Node* rebuilt = rebuild(parent);
			if (i == 0) {
				root = rebuilt;
			} else if (path[i - 1]->left == parent) {
				path[i - 1]->left = rebuilt;
			} else {
				path[i - 1]->right = rebuilt;
			}
			return;
		}
	}

public:
	// ScapegoatTree() — 功能：构造空替罪羊树。O(1)。
	ScapegoatTree() = default;

	// ScapegoatTree(Compare compare) — 功能：构造空替罪羊树。O(1)。
	ScapegoatTree(Compare compare) : compare(compare) {}

	ScapegoatTree(const ScapegoatTree&) = delete;
	ScapegoatTree& operator=(const ScapegoatTree&) = delete;

	~ScapegoatTree() {
		clear(root);
	}

	void insert(T key) {
		std::vector<Node*> path;
		bool inserted_node = false;
		root = insert(root, key, path, inserted_node);
		if (!inserted_node) return;

		max_nodes = std::max(max_nodes, node_count(root));
		if ((int)path.size() - 1 > allowed_depth(max_nodes)) {
			rebuild_scapegoat(path);
		}
	}

	// void remove(T key) — 功能：删除一个等于 key 的元素副本（若 key 不存在则无操作）。均摊 O(log n)。
	void remove(T key) {
		bool removed = false;
		root = erase(root, key, removed);
		if (!removed) return;
		if (!root) {
			max_nodes = 0;
		} else if (3LL * node_count(root) < 2LL * max_nodes) {
			root = rebuild(root);
			max_nodes = node_count(root);
		}
	}

	// int size() const — 功能：返回集合中元素的总个数（含重复元素） / 判断集合是否为空。O(1)。
	int size() const {
		return element_count(root);
	}

	// bool empty() const — 功能：返回集合中元素的总个数（含重复元素） / 判断集合是否为空。O(1)。
	bool empty() const {
		return root == nullptr;
	}

	// int get_rank(T key) const — 功能：返回 key 的 1-based 排名（严格小于 key 的元素个数 + 1）。均摊 O(log n)。
	int get_rank(T key) const {
		Node* cur = root;
		int result = 1;
		while (cur) {
			if (compare(key, cur->key)) {
				cur = cur->left;
			} else if (equal(key, cur->key)) {
				return result + element_count(cur->left);
			} else {
				result += element_count(cur->left) + cur->count;
				cur = cur->right;
			}
		}
		return result;
	}

	// T get_value(int rank) const — 功能：返回 1-based 排名为 rank 的元素值（要求 1 <= rank <= size()）。均摊 O(log n)。
	T get_value(int rank) const {
		return value_at(root, rank);
	}

	// T get_prev(T key) const — 功能：返回严格小于 key 的最大元素（不存在返回 std::numeric_limits<T>::lowest()）。均摊 O(log n)。
	T get_prev(T key) const {
		T result = std::numeric_limits<T>::lowest();
		Node* cur = root;
		while (cur) {
			if (compare(cur->key, key)) {
				result = cur->key;
				cur = cur->right;
			} else {
				cur = cur->left;
			}
		}
		return result;
	}

	// T get_next(T key) const — 功能：返回严格大于 key 的最小元素（不存在返回 std::numeric_limits<T>::max()）。均摊 O(log n)。
	T get_next(T key) const {
		T result = std::numeric_limits<T>::max();
		Node* cur = root;
		while (cur) {
			if (compare(key, cur->key)) {
				result = cur->key;
				cur = cur->left;
			} else {
				cur = cur->right;
			}
		}
		return result;
	}

	Compare key_comp() const {
		return compare;
	}
};

#endif
