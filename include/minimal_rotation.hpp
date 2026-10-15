#ifndef MINIMAL_ROTATION_HPP
#define MINIMAL_ROTATION_HPP

#include <algorithm>
#include <functional>
#include <string>

// ============================================================================
// 【Duval 算法字符串最小/最大循环表示法 Minimal String Rotation (minimal_rotation.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. template <typename Sequence, typename Compare = std::less<>>
//      int minimal_rotation(const Sequence& sequence, Compare compare = Compare{})
//      功能：使用 Duval 双指针算法在严格线性时间内求序列 sequence（长度 n）按比较器 compare 字典序最小的循环同构串起始下标。
//      返回：0-based 起始下标 start in [0, n)（若有多个并列最小循环移位，返回最小的起始下标；空序列返回 0）。
//      复杂度：时间复杂度严格 O(n)（比较次数不超过 3n），额外空间复杂度 O(1)。
//   2. inline std::string minimal_rotation_string(const std::string& value)
//      功能：返回字符串 value 字典序最小的循环移位结果字符串 value.substr(start) + value.substr(0, start)。O(n)。
//   3. inline std::string maximal_rotation_string(const std::string& value)
//      功能：传入 std::greater<>{}，返回字符串 value 字典序最大的循环移位结果字符串。O(n)。
// ============================================================================

// template <typename Sequence, typename Compare = std::less<>> int minimal_rotation(const Sequence& sequence, Compare compare = Compare{}) — 功能：使用 Duval 双指针算法在严格线性时间内求序列 sequence（长度 n）按比较器 compare 字典序最小的循环同构串起始下标。
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

// inline std::string minimal_rotation_string(const std::string& value) — 功能：返回字符串 value 字典序最小的循环移位结果字符串 value.substr(start) + value.substr(0, start)。O(n)。
inline std::string minimal_rotation_string(const std::string& value) {
	int start = minimal_rotation(value);
	return value.substr(start) + value.substr(0, start);
}

// inline std::string maximal_rotation_string(const std::string& value) — 功能：传入 std::greater<>{}，返回字符串 value 字典序最大的循环移位结果字符串。O(n)。
inline std::string maximal_rotation_string(const std::string& value) {
	int start = minimal_rotation(value, std::greater<>{});
	return value.substr(start) + value.substr(0, start);
}

#endif
