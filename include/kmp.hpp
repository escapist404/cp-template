#ifndef KMP_HPP
#define KMP_HPP

#include <iostream>
#include <vector>
#include <string>

// Return all 0-indexed positions where pattern `s` occurs in text `t`.
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
