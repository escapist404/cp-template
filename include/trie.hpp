#ifndef TRIE_HPP
#define TRIE_HPP
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

// Replace std::unordered_map to other containers,
// if there is enough memory but not enough time.

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
