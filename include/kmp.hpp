#ifndef KMP_HPP
#define KMP_HPP

#include <iostream>
#include <vector>
#include <string>

// ============================================================================
// 【KMP 字符串模式匹配算法 (kmp.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   std::vector<int> kmp(const std::string& s, const std::string& t)
// 参数与返回值详解（【严禁搞反参数顺序】第一个参数 s 是模式串，第二个参数 t 是文本串）：
//   - s：模式串（Pattern），内部先构建其最长相等真前后缀失配表 fail。
//   - t：待搜索的文本串（Text）。
//   - 返回值 std::vector<int>：模式串 s 在文本串 t 中所有完整出现位置的 0-based 起始下标列表（按升序排列，可重叠）。
//     【边界说明】若模式串 s 为空串，遵循代码行为返回空向量 {}。
// 复杂度：时间与空间复杂度均为严格线性 O(|s| + |t|)。
// ============================================================================

// std::vector<int> kmp(const std::string& s, const std::string& t) — 功能：参数与返回值详解（【严禁搞反参数顺序】第一个参数 s 是模式串，第二个参数 t 是文本串）：
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
