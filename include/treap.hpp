#ifndef TREAP_HPP
#define TREAP_HPP

#include <functional>
#include <limits>
#include <random>
#include <utility>

// FHQ Treap with duplicate keys.
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
	Treap() = default;

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

	void remove(const T& key) {
		root = erase(root, key);
	}

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

	T get_value(int rank) const {
		return value_at(root, rank);
	}

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
