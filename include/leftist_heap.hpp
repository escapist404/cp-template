#ifndef LEFTIST_HEAP_HPP
#define LEFTIST_HEAP_HPP

#include <functional>
#include <utility>

// 【左偏树 / 可并堆 (leftist_heap.hpp)】
// - 支持 O(log n) 插入、弹出与堆合并的左偏树 LeftistHeap<T, Compare>，禁用拷贝
// - 比较器约定: Compare(a, b) 为 true 表示 a 比 b 更靠近堆顶；因此默认 std::less<T> 为【小根堆】，大根堆请传 std::greater<T>
// - push(key) / top() / pop() / empty() / size(): 标准优先队列操作（top/pop 要求非空）
// - merge(other): O(log n) 将另一个左偏树 other 合并入当前堆，并将 other 清空

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

	// 将另一个左偏树合并入当前堆，并将 other 清空
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
