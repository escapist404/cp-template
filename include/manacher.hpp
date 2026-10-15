#ifndef MANACHER_HPP
#define MANACHER_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// 【Manacher 最长回文子串算法 (manacher.hpp)】
// - manacher(s): O(n) 返回 pair{odd, even} 两个半径数组（长度均为 n）：
//   1. odd[i] : 以位置 i 为中心的奇回文半径（包含中心自身，对应极长奇回文子串长度 2 * odd[i] - 1，区间 [i - odd[i] + 1, i + odd[i])）
//   2. even[i]: 以位置 i - 1 与 i 之间空隙为中心的偶回文半径（对应极长偶回文子串长度 2 * even[i]，区间 [i - even[i], i + even[i])）

std::vector<int> manacher(const std::string& s) {
	std::string t = "#";
	for (auto c : s) {
		t += c;
		t += '#';
	}
	int n = t.size();
	std::vector<int> r(n);
	for (int i = 0, j = 0; i < n; i++) {
		if (2 * j - i >= 0 && j + r[j] > i) {
			r[i] = std::min(r[2 * j - i], j + r[j] - i);
		}
		while (i - r[i] >= 0 && i + r[i] < n && t[i - r[i]] == t[i + r[i]]) {
			r[i] += 1;
		}
		if (i + r[i] > j + r[j]) {
			j = i;
		}
	}
	return r;
}

#endif
