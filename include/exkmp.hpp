#ifndef EXKMP_HPP
#define EXKMP_HPP

#include <algorithm>
#include <vector>

// ============================================================================
// 【扩展 KMP 算法 Extended KMP / ExKMP (exkmp.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解（Sequence 可为 std::string 或 std::vector<T>）：
//   1. template <typename Sequence>
//      std::vector<int> exkmp_z(const Sequence& sequence)
//      功能：计算序列 sequence（长度为 n）的 Z 函数数组 z，其中 z[i] = LCP(sequence, sequence[i..n))；
//      【注意约定】与 z_algo.hpp 不同，exkmp_z 在 n > 0 时定义 z[0] == n。时间复杂度 O(n)。
//   2. template <typename Sequence>
//      std::vector<int> exkmp(const Sequence& pattern, const Sequence& text)
//      功能：计算文本串 text（长度 m）的每个后缀 text[i..m) 与模式串 pattern（长度 n）的最长公共前缀长度。
//      【参数顺序】pattern 在前，text 在后。
//      返回：长度为 m 的数组 extend，其中 extend[i] = LCP(pattern, text[i..m))（若 pattern 或 text 为空则全为 0）。
//      复杂度：时间与空间复杂度均为 O(|pattern| + |text|)。
// ============================================================================

// template <typename Sequence> std::vector<int> exkmp_z(const Sequence& sequence) — 功能：计算序列 sequence（长度为 n）的 Z 函数数组 z，其中 z[i] = LCP(sequence, sequence[i..n))；
template <typename Sequence>
std::vector<int> exkmp_z(const Sequence& sequence) {
	const int n = sequence.size();
	std::vector<int> z(n);
	if (!n) return z;
	z[0] = n;
	for (int i = 1, l = 0, r = 0; i < n; ++i) {
		if (i <= r) z[i] = std::min(r - i + 1, z[i - l]);
		while (i + z[i] < n && sequence[z[i]] == sequence[i + z[i]]) ++z[i];
		if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
	}
	return z;
}

// 计算扩展 KMP 数组：返回 extend[i] = LCP(pattern, text[i..n-1])。
template <typename Sequence>
std::vector<int> exkmp(const Sequence& pattern, const Sequence& text) {
	const int n = pattern.size(), m = text.size();
	std::vector<int> extend(m);
	if (!n || !m) return extend;
	const std::vector<int> z = exkmp_z(pattern);
	for (int i = 0, l = 0, r = 0; i < m; ++i) {
		if (i <= r) extend[i] = std::min(r - i + 1, z[i - l]);
		while (extend[i] < n && i + extend[i] < m &&
			pattern[extend[i]] == text[i + extend[i]]) ++extend[i];
		if (i + extend[i] - 1 > r) l = i, r = i + extend[i] - 1;
	}
	return extend;
}

#endif
