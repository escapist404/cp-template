#ifndef EXKMP_HPP
#define EXKMP_HPP

#include <algorithm>
#include <vector>

// 【扩展 KMP / ExKMP (exkmp.hpp)】
// - exkmp(pattern, text): O(|pattern| + |text|) 返回文本串每个后缀 text[i..m) 与模式串 pattern 的 LCP 长度数组 extend
// - 注意参数顺序: pattern 在前，text 在后；内部复用 z_algorithm(pattern)

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

// Return extend[i] = LCP(pattern, text[i..]).
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
