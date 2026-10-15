#ifndef MONOTONE_QUEUE_HPP
#define MONOTONE_QUEUE_HPP

#include <cassert>
#include <deque>
#include <functional>
#include <vector>

// 【单调队列与滑动窗口最值 (monotone_queue.hpp)】
// - MonotoneQueue<T, Compare>: 默认 std::less<T> 维护单调递减队列（front() 为最大值）；求最小值请传 std::greater<T>
//   - push(index, val)      : 队尾压入元素并淘汰不优旧元素
//   - pop_before(first_idx) : 弹出所有满足 index < first_idx 的过期队首
//   - front() / front_index(): O(1) 获取当前极值与对应下标（要求非空）
// - sliding_window(values, width, compare): O(n) 返回所有宽度为 width 的连续窗口极值数组

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
