#ifndef LEFTIST_HEAP_HPP
#define LEFTIST_HEAP_HPP

#include <functional>
#include <utility>

// ============================================================================
// 【左偏树 / 可并堆 Leftist Heap (leftist_heap.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T = int, typename Compare = std::less<T>> class LeftistHeap
//   支持 O(log n) 合并的优先队列，禁用拷贝构造与拷贝赋值。
//   【比较器方向约定】Compare(a, b) 为 true 表示 a 比 b 优先级更高（更靠近堆顶）：
//   - 默认 Compare = std::less<T> 为【小根堆】（top() 返回最小值）；
//   - 若需【大根堆】，请实例化为 LeftistHeap<T, std::greater<T>>。
// 构造函数与成员函数签名详解：
//   - LeftistHeap() / LeftistHeap(const Compare& compare)
//     功能：构造空左偏树。O(1)。
//   - bool empty() const / int size() const
//     功能：判空 / 返回堆中当前元素个数。O(1)。
//   - const T& top() const
//     功能：返回堆顶最优元素的常引用（要求 !empty()，空堆调用触发 assert）。O(1)。
//   - void push(const T& key)
//     功能：新建单节点并与当前根合并，插入元素 key。时间复杂度 O(log n)。
//   - void pop()
//     功能：合并根的左右子树并释放原堆顶节点（要求 !empty()）。时间复杂度 O(log n)。
//   - void merge(LeftistHeap& other)
//     功能：将另一个左偏树 other 的全部节点合并入当前堆，并将 other 置为空堆（支持 &other == this 自合并保护）。O(log n)。
// ============================================================================

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
	// LeftistHeap() — 功能：构造空左偏树。O(1)。
	LeftistHeap() = default;

	// LeftistHeap(const Compare& compare) — 功能：构造空左偏树。O(1)。
	LeftistHeap(const Compare& compare)
		: compare(compare) {}

	LeftistHeap(const LeftistHeap&) = delete;
	LeftistHeap& operator=(const LeftistHeap&) = delete;

	~LeftistHeap() {
		clear(root);
	}

	// bool empty() const — 功能：判空 / 返回堆中当前元素个数。O(1)。
	bool empty() const {
		return root == nullptr;
	}

	// int size() const — 功能：判空 / 返回堆中当前元素个数。O(1)。
	int size() const {
		return heap_size;
	}

	// const T& top() const — 功能：返回堆顶最优元素的常引用（要求 !empty()，空堆调用触发 assert）。O(1)。
	const T& top() const {
		return root->key;
	}

	// void push(const T& key) — 功能：新建单节点并与当前根合并，插入元素 key。时间复杂度 O(log n)。
	void push(const T& key) {
		root = merge_nodes(root, new Node(key));
		++heap_size;
	}

	// void pop() — 功能：合并根的左右子树并释放原堆顶节点（要求 !empty()）。时间复杂度 O(log n)。
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
