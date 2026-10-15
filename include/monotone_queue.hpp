#ifndef MONOTONE_QUEUE_HPP
#define MONOTONE_QUEUE_HPP

#include <cassert>
#include <deque>
#include <functional>
#include <vector>

// ============================================================================
// 【单调队列与定长滑动窗口极值 Monotone Queue (monotone_queue.hpp)】
// ----------------------------------------------------------------------------
// 1. 单调队列模板类签名：template <typename T, typename Compare = std::less<T>> class MonotoneQueue
//    【比较器方向约定】Compare(old_val, new_val) 为 true 时队尾旧元素被新元素淘汰：
//    - 默认 Compare = std::less<T> 维护【单调递减队列】，队首 front() 为【窗口最大值】（相同值保留最新下标）；
//    - 若需维护【窗口最小值】，请传入 std::greater<T>{}。
//    成员函数签名详解（每个元素最多入队出队一次，均摊 O(1)）：
//    - MonotoneQueue(Compare compare = Compare{})：构造空单调队列。
//    - void clear() / bool empty() const / int size() const：清空 / 判空 / 当前队列内候选元素数。
//    - void push(int index, T value)：压入下标为 index、值为 value 的新元素并从队尾淘汰不优元素。均摊 O(1)。
//    - void pop_before(int first_index)：从队首弹出所有满足 index < first_index 的过期元素。均摊 O(1)。
//    - int front_index() const / const T& front() const：返回当前队首最优元素的下标 / 值（要求 !empty()）。O(1)。
// 2. 滑动窗口便捷函数签名：
//    - template <typename T, typename Compare = std::less<T>>
//      std::vector<T> sliding_window(const std::vector<T>& values, int width, Compare compare = Compare{})
//      功能：对长度为 n 的数组 values 计算所有长度为 width (width >= 1) 的连续子区间极值。
//      返回：长度为 max(0, n - width + 1) 的极值数组，第 i 项对应窗口 values[i .. i + width)。O(n)。
// ============================================================================

template <typename T, typename Compare = std::less<T>>
class MonotoneQueue {
	struct Item {
		int index;
		T value;
	};

	std::deque<Item> data;
	Compare compare;

public:
	MonotoneQueue(Compare compare = Compare{}) : compare(compare) {}

	void clear() { data.clear(); }
	bool empty() const { return data.empty(); }
	int size() const { return data.size(); }

	void push(int index, T value) {
		while (!data.empty() && compare(data.back().value, value))
			data.pop_back();
		data.push_back({index, value});
	}

	void pop_before(int first_index) {
		while (!data.empty() && data.front().index < first_index)
			data.pop_front();
	}

	int front_index() const {
		assert(!data.empty());
		return data.front().index;
	}

	const T& front() const {
		assert(!data.empty());
		return data.front().value;
	}
};

// - template <typename T, typename Compare = std::less<T>> std::vector<T> sliding_window(const std::vector<T>& values, int width, Compare compare = Compare{}) — 功能：template <typename T, typename Compare = std::less<T>>
template <typename T, typename Compare = std::less<T>>
std::vector<T> sliding_window(const std::vector<T>& values, int width,
		Compare compare = Compare{}) {
	assert(width > 0);
	MonotoneQueue<T, Compare> queue(compare);
	std::vector<T> result;
	for (int i = 0; i < (int)values.size(); ++i) {
		queue.push(i, values[i]);
		queue.pop_before(i - width + 1);
		if (i + 1 >= width)
			result.push_back(queue.front());
	}
	return result;
}

#endif
