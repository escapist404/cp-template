#ifndef TRIE_HPP
#define TRIE_HPP
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

// 【小写字母字典树 Trie (trie.hpp)】
// - 仅支持 26 个小写英文字母 'a'-'z'，单次插入/查询时间复杂度 O(|s|)
// - insert(s)     : 向字典树中插入字符串 s（允许重复插入，终点计数累加）
// - count(s)      : 返回字符串 s 作为完整单词被插入的精确次数
// - prefix_count(s): 返回以 s 为前缀的已插入单词总数

class Trie {
	struct Node {
		int cnt_prefix;
		int child[26];

		Node() : cnt_prefix(0) { std::fill(child, child + 26, -1); }
	};

	std::vector<Node> trie{{}};

public:
	void insert(const std::string& s) {
		int cur = 0;
		for (char ch : s) {
			const int x = ch - 'a';
			assert(0 <= x && x < 26);
			if (trie[cur].child[x] == -1) {
				trie[cur].child[x] = trie.size();
				trie.emplace_back();
			}
			cur = trie[cur].child[x];
			++trie[cur].cnt_prefix;
		}
	}

	int query(const std::string& s) const {
		int cur = 0;
		for (char ch : s) {
			const int x = ch - 'a';
			assert(0 <= x && x < 26);
			if (trie[cur].child[x] == -1) return 0;
			cur = trie[cur].child[x];
		}
		return trie[cur].cnt_prefix;
	}
};

#endif
