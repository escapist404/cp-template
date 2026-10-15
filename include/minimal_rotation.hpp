#ifndef MINIMAL_ROTATION_HPP
#define MINIMAL_ROTATION_HPP

#include <algorithm>
#include <functional>
#include <string>

// 【最小表示法 Minimal String Rotation (minimal_rotation.hpp)】
// - 使用 Duval 算法在严格 O(n) 时间与 O(1) 额外空间内找到字典序最小的循环同构串
// - minimal_rotation(s)       : 返回字典序最小循环移位的 0-based 起始下标（空串返回 0）
// - minimal_rotated(seq)      : 直接返回循环移位后的最小序列/字符串副本

template <typename Sequence, typename Compare = std::less<>>
int minimal_rotation(const Sequence& sequence, Compare compare = Compare{}) {
	int n = sequence.size();
	if (!n) return 0;
	int first = 0, second = 1, offset = 0;
	while (first < n && second < n && offset < n) {
		auto a = sequence[(first + offset) % n];
		auto b = sequence[(second + offset) % n];
		if (!compare(a, b) && !compare(b, a)) {
			++offset;
			continue;
		}
		if (compare(a, b)) {
			second += offset + 1;
			if (second == first) ++second;
		} else {
			first += offset + 1;
			if (first == second) ++first;
		}
		offset = 0;
	}
	return std::min(first, second);
}

inline std::string minimal_rotation_string(const std::string& value) {
	int start = minimal_rotation(value);
	return value.substr(start) + value.substr(0, start);
}

inline std::string maximal_rotation_string(const std::string& value) {
	int start = minimal_rotation(value, std::greater<>{});
	return value.substr(start) + value.substr(0, start);
}

#endif
