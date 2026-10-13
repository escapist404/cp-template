#ifndef MONOTONE_QUEUE_HPP
#define MONOTONE_QUEUE_HPP

#include <cassert>
#include <deque>
#include <functional>
#include <vector>

// With std::less this keeps the maximum; use std::greater for the minimum.

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
