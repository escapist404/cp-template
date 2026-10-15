#ifndef MANACHER_HPP
#define MANACHER_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// ============================================================================
// 【Manacher 最长回文子串算法 (manacher.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   std::vector<int> manacher(const std::string& s)
// 参数与返回值详解：
//   - s：输入字符串（长度为 m = s.size()）。
//   - 内部构造原理：将原串扩展为长度 N = 2m + 3 的增广串 t = "^#s[0]#s[1]#...#s[m-1]#$"。
//   - 返回值 std::vector<int>：长度为 2 * s.size() + 3 的回文半径数组 r（r[k] 表示在增广串 t 中以 t[k] 为中心的极长回文半径，含中心）：
//     1. 【最长回文子串长度】：对所有 k，r[k] - 1 即为对应原串回文子串的真实长度；
//     2. 【原串下标 i (0-based) 为中心的极长奇回文长度】：等于 r[2 * i + 2] - 1；
//     3. 【原串下标 i 与 i+1 之间空隙为中心的极长偶回文长度】：等于 r[2 * i + 3] - 1；
//     4. 【原串左闭右开子串 s[l .. r_idx) (0 <= l < r_idx <= m) 是否为回文】：
//        当且仅当 r[l + r_idx + 1] - 1 >= r_idx - l。
// 复杂度：时间与空间复杂度均为严格线性 O(|s|)。
// ============================================================================

// std::vector<int> manacher(const std::string& s) — 功能：参数与返回值详解：
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
