#ifndef LEFTIST_HEAP_HPP
#define LEFTIST_HEAP_HPP

#include <functional>
#include <utility>

// Mergeable leftist heap. Compare(a, b) means that a has higher priority
// than b; the default therefore keeps the smallest key at the top.
template <typename T = int, typename Compare = std::less<T>>
class LeftistHeap {
	struct Node {
		T key;
		Node* left = nullptr;
		Node* right = nullptr;
		int null_path_length = 1;

		Node(const T& key) : key(key) {}
	};

	Node* root = nullptr;
	int heap_size = 0;
	Compare compare;

	static int null_path_length(const Node* node) {
		return node ? node->null_path_length : 0;
	}

	Node* merge_nodes(Node* lhs, Node* rhs) {
		if (!lhs) {
			return rhs;
		}
		if (!rhs) {
			return lhs;
		}

		if (compare(rhs->key, lhs->key)) {
			std::swap(lhs, rhs);
		}
		lhs->right = merge_nodes(lhs->right, rhs);
		if (null_path_length(lhs->left) < null_path_length(lhs->right)) {
			std::swap(lhs->left, lhs->right);
		}
		lhs->null_path_length = null_path_length(lhs->right) + 1;
		return lhs;
	}

	static void clear(Node* node) {
		if (!node) {
			return;
		}
		clear(node->left);
		clear(node->right);
		delete node;
	}

public:
	LeftistHeap() = default;

	LeftistHeap(const Compare& compare)
		: compare(compare) {}

	LeftistHeap(const LeftistHeap&) = delete;
	LeftistHeap& operator=(const LeftistHeap&) = delete;

	~LeftistHeap() {
		clear(root);
	}

	bool empty() const {
		return root == nullptr;
	}

	int size() const {
		return heap_size;
	}

	const T& top() const {
		return root->key;
	}

	void push(const T& key) {
		root = merge_nodes(root, new Node(key));
		++heap_size;
	}

	void pop() {
		Node* old_root = root;
		root = merge_nodes(root->left, root->right);
		delete old_root;
		--heap_size;
	}

	// Merge other into this heap and leave other empty.
	void merge(LeftistHeap& other) {
		if (this == &other) {
			return;
		}
		root = merge_nodes(root, other.root);
		heap_size += other.heap_size;
		other.root = nullptr;
		other.heap_size = 0;
	}
};

#endif
