#ifndef LYNDON_HPP
#define LYNDON_HPP

#include <utility>
#include <vector>

// ============================================================================
// 【Duval 算法 Lyndon 分解 Lyndon Factorization (lyndon.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename Sequence>
//   std::vector<std::pair<int, int>> lyndon_factorization(const Sequence& sequence)
// 参数与返回值详解：
//   - sequence：输入字符串或支持下标与比较的序列（长度为 n）。
//   - 返回值 std::vector<std::pair<int, int>>：
//     由 Chen-Fox-Lyndon 定理，将序列唯一分解为满足字典序非递增 w_1 >= w_2 >= ... >= w_k 的 Lyndon 串序列
//     （Lyndon 串定义为严格小于其所有非平凡循环移位/非空真后缀的字符串）。
//     返回值中每项 {l, r} 表示一个 0-based【左闭右开子串区间 [l, r)】。
// 复杂度：Duval 三指针算法，时间复杂度严格 O(n)，额外空间 O(1)（除返回值外）。
// ============================================================================

// template <typename Sequence> std::vector<std::pair<int, int>> lyndon_factorization(const Sequence& sequence) — 功能：参数与返回值详解：
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
