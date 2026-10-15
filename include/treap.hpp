#ifndef TREAP_HPP
#define TREAP_HPP

#include <functional>
#include <limits>
#include <random>
#include <utility>

// ============================================================================
// 【分裂合并平衡树 Treap (treap.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T = int, typename Compare = std::less<T>> class Treap
//   基于随机优先级维护有序多重集合（相同键值合并计数 count），默认 std::less<T> 按升序排列。
//   内部含裸指针节点，已禁用拷贝构造与拷贝赋值，析构时自动释放全部节点。
// 构造函数与成员函数签名详解（所有操作期望时间复杂度均为 O(log n)）：
//   - Treap() / Treap(const Compare& compare)
//     功能：构造空平衡树。O(1)。
//   - void insert(const T& key)
//     功能：向多重集合中插入一个 key（若已存在则对应节点 count 加 1）。期望 O(log n)。
//   - void remove(const T& key)
//     功能：从多重集合中删除一个 key 副本（count 减 1，归零时移除节点；若 key 不存在则无操作）。期望 O(log n)。
//   - int get_rank(const T& key) const
//     功能：查询 key 的 1-based 排名，定义为【严格小于 key 的元素总个数（含重复）+ 1】。期望 O(log n)。
//   - T get_value(int rank) const
//     功能：查询 1-based 排名为 rank 的元素值（要求 1 <= rank <= 总元素数，越界触发 assert）。期望 O(log n)。
//   - T get_prev(const T& key) const
//     功能：查询严格小于 key 的最大元素（严格前驱）；若不存在则返回 std::numeric_limits<T>::lowest()。期望 O(log n)。
//   - T get_next(const T& key) const
//     功能：查询严格大于 key 的最小元素（严格后继）；若不存在则返回 std::numeric_limits<T>::max()。期望 O(log n)。
// ============================================================================

template <typename T = int, typename Compare = std::less<T>>
class Treap {
	struct Node {
		Node* left;
		Node* right;
		T key;
		int priority;
		int count;
		int size;

		Node(const T& key, int priority)
			: left(nullptr),
			  right(nullptr),
			  key(key),
			  priority(priority),
			  count(1),
			  size(1) {}

		void maintain() {
			size = count;
			if (left) {
				size += left->size;
			}
			if (right) {
				size += right->size;
			}
		}
	};

	std::mt19937 rng{std::random_device{}()};
	std::uniform_int_distribution<int> priorities;
	Node* root = nullptr;
	Compare compare;

	int random_priority() {
		return priorities(rng);
	}

	bool equal(const T& lhs, const T& rhs) const {
		return !compare(lhs, rhs) && !compare(rhs, lhs);
	}

	std::pair<Node*, Node*> split_less(Node* cur, const T& key) {
		if (!cur) {
			return {nullptr, nullptr};
		}

		if (compare(cur->key, key)) {
			auto parts = split_less(cur->right, key);
			cur->right = parts.first;
			cur->maintain();
			return {cur, parts.second};
		}

		auto parts = split_less(cur->left, key);
		cur->left = parts.second;
		cur->maintain();
		return {parts.first, cur};
	}

	// void insert(const T& key) — 功能：向多重集合中插入一个 key（若已存在则对应节点 count 加 1）。期望 O(log n)。
	Node* insert(Node* cur, const T& key, int priority) {
		if (!cur) {
			return new Node(key, priority);
		}

		if (equal(key, cur->key)) {
			++cur->count;
			cur->maintain();
			return cur;
		}

		if (priority > cur->priority) {
			auto parts = split_less(cur, key);
			Node* node = new Node(key, priority);
			node->left = parts.first;
			node->right = parts.second;
			node->maintain();
			return node;
		}

		if (compare(key, cur->key)) {
			cur->left = insert(cur->left, key, priority);
		} else {
			cur->right = insert(cur->right, key, priority);
		}
		cur->maintain();
		return cur;
	}

	Node* erase(Node* cur, const T& key) {
		if (!cur) {
			return nullptr;
		}

		if (compare(key, cur->key)) {
			cur->left = erase(cur->left, key);
		} else if (compare(cur->key, key)) {
			cur->right = erase(cur->right, key);
		} else if (cur->count > 1) {
			--cur->count;
		} else {
			Node* next = merge(cur->left, cur->right);
			delete cur;
			return next;
		}

		cur->maintain();
		return cur;
	}

	Node* merge(Node* left, Node* right) {
		if (!left) {
			return right;
		}
		if (!right) {
			return left;
		}

		if (left->priority > right->priority) {
			left->right = merge(left->right, right);
			left->maintain();
			return left;
		}

		right->left = merge(left, right->left);
		right->maintain();
		return right;
	}

	void clear(Node* cur) {
		if (!cur) {
			return;
		}

		clear(cur->left);
		clear(cur->right);
		delete cur;
	}

	T value_at(const Node* cur, int rank) const {
		const int left_size = cur->left ? cur->left->size : 0;
		if (rank <= left_size) {
			return value_at(cur->left, rank);
		}
		if (rank <= left_size + cur->count) {
			return cur->key;
		}
		return value_at(cur->right, rank - left_size - cur->count);
	}

public:
	// Treap() — 功能：构造空平衡树。O(1)。
	Treap() = default;

	// Treap(const Compare& compare) — 功能：构造空平衡树。O(1)。
	Treap(const Compare& compare)
		: compare(compare) {}

	Treap(const Treap&) = delete;
	Treap& operator=(const Treap&) = delete;

	~Treap() {
		clear(root);
	}

	void insert(const T& key) {
		root = insert(root, key, random_priority());
	}

	// void remove(const T& key) — 功能：从多重集合中删除一个 key 副本（count 减 1，归零时移除节点；若 key 不存在则无操作）。期望 O(log n)。
	void remove(const T& key) {
		root = erase(root, key);
	}

	// int get_rank(const T& key) const — 功能：查询 key 的 1-based 排名，定义为【严格小于 key 的元素总个数（含重复）+ 1】。期望 O(log n)。
	int get_rank(const T& key) const {
		const Node* cur = root;
		int result = 1;

		while (cur) {
			if (compare(key, cur->key)) {
				cur = cur->left;
			} else if (equal(key, cur->key)) {
				return result + (cur->left ? cur->left->size : 0);
			} else {
				result += cur->count + (cur->left ? cur->left->size : 0);
				cur = cur->right;
			}
		}

		return result;
	}

	// T get_value(int rank) const — 功能：查询 1-based 排名为 rank 的元素值（要求 1 <= rank <= 总元素数，越界触发 assert）。期望 O(log n)。
	T get_value(int rank) const {
		return value_at(root, rank);
	}

	// T get_prev(const T& key) const — 功能：查询严格小于 key 的最大元素（严格前驱）；若不存在则返回 std::numeric_limits<T>::lowest()。期望 O(log n)。
	T get_prev(const T& key) const {
		T result = std::numeric_limits<T>::lowest();
		const Node* cur = root;

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

	// T get_next(const T& key) const — 功能：查询严格大于 key 的最小元素（严格后继）；若不存在则返回 std::numeric_limits<T>::max()。期望 O(log n)。
	T get_next(const T& key) const {
		T result = std::numeric_limits<T>::max();
		const Node* cur = root;

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
