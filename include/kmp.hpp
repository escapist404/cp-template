#ifndef KMP_HPP
#define KMP_HPP

#include <iostream>
#include <vector>
#include <string>

// 【KMP 字符串匹配与前缀函数 (kmp.hpp)】
// - prefix_function(s)  : O(n) 返回前缀函数 pi，其中 pi[i] 为子串 s[0..i] 的最长相等真前后缀长度
// - kmp(pattern, text)  : O(|pattern| + |text|) 查找模式串在文本串中的所有出现位置（注意参数顺序：pattern 在前，text 在后）
//   返回所有匹配起点的 0-based 下标升序数组（若 pattern 为空串则返回 0..|text|）

std::vector<int> kmp(const std::string& s, const std::string& t) {
	std::vector<int> result;
	if (s.empty()) return result;
	const auto get_fail = [&] {
		std::vector<int> fail(s.size());
		for (int i = 1, j = 0; i < (int)s.size(); ++i, j = fail[i - 1]) {
			while (j && s[i] != s[j])
				j = fail[j - 1];
			fail[i] = j + (s[i] == s[j]);
		}
		return fail;
	};
	const std::vector<int> fail = get_fail();
	for (int i = 0, j = 0; i < (int)t.size(); ++i) {
		while (j && t[i] != s[j])
			j = fail[j - 1];
		j += t[i] == s[j];
		if (j == (int)s.size()) {
			result.push_back(i - j + 1);
			j = fail[j - 1];
		}
	}
	return result;
}

#endif
