#ifndef TRIE_HPP
#define TRIE_HPP
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

// ============================================================================
// 【26 小写字母字典树 Trie (trie.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class Trie
//   维护小写英文字母集 ('a' - 'z') 上的多重字符串集合（根节点编号为 0）。
// 成员函数签名详解：
//   - void insert(const std::string& s)
//     功能：将仅含小写字母 'a'-'z' 的字符串 s 插入字典树，并将终点状态的尾标记计数 count 加 1。
//     复杂度：时间复杂度 O(|s|)，每个新增节点空间 O(26)。
//   - int query(const std::string& s) const
//     功能：查询字符串 s 在字典树中作为【完整单词】被插入的次数（若路径不存在或仅作为前缀出现则返回 0）。
//     复杂度：时间复杂度 O(|s|)。
// ============================================================================

class Trie {
	struct Node {
		int cnt_prefix;
		int child[26];

		Node() : cnt_prefix(0) { std::fill(child, child + 26, -1); }
	};

	std::vector<Node> trie{{}};

public:
	// void insert(const std::string& s) — 功能：将仅含小写字母 'a'-'z' 的字符串 s 插入字典树，并将终点状态的尾标记计数 count 加 1。
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

	// int query(const std::string& s) const — 功能：查询字符串 s 在字典树中作为【完整单词】被插入的次数（若路径不存在或仅作为前缀出现则返回 0）。
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
