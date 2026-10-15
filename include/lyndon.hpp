#ifndef LYNDON_HPP
#define LYNDON_HPP

#include <utility>
#include <vector>

// 【Duval 算法 Lyndon 分解 (lyndon.hpp)】
// - lyndon_factorization(s): O(n) 将字符串唯一分解为满足 w_1 >= w_2 >= ... >= w_k 的 Lyndon 串序列
// - 返回左闭右开区间列表 std::vector<std::pair<int, int>>，每项 {l, r} 对应子串 s[l..r)

template <typename Sequence>
std::vector<std::pair<int, int>> lyndon_factorization(const Sequence& sequence) {
	const int n = sequence.size();
	std::vector<std::pair<int, int>> result;
	for (int i = 0; i < n;) {
		int j = i + 1, k = i;
		while (j < n && sequence[k] <= sequence[j]) {
			if (sequence[k] < sequence[j]) k = i;
			else ++k;
			++j;
		}
		while (i <= k) {
			const int length = j - k;
			result.push_back({i, i + length});
			i += length;
		}
	}
	return result;
}

#endif
