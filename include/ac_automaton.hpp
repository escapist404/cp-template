#ifndef AC_AUTOMATON_HPP
#define AC_AUTOMATON_HPP

#include <array>
#include <cassert>
#include <queue>
#include <string>
#include <vector>

// 【AC 自动机 Aho-Corasick Automaton (ac_automaton.hpp)】
// - 支持 26 个小写字母 'a'-'z' 的多模式串匹配自动机，模式串编号为 0-based 的插入顺序 [0, m)
// - 使用流程: 先依次调用 insert(pattern) 插入所有模式串，然后【必须调用一次 build()】构建失配指针与拓扑序，最后调用 query(text)
// - query(text): O(|text| + 结点数 + m) 统计每个模式串在 text 中的出现次数（沿 fail 树逆 BFS 序累加），返回长度 m 的计数数组
// - size() / state_links() / pattern_vertices(): 导出状态总数、各状态 {parent, fail} 与模式串终点状态编号

class AhoCorasick {
	struct Node {
		std::array<int, 26> next{};
		int fail = 0;
		int parent = -1;
		Node() { next.fill(-1); }
	};

	std::vector<Node> trie{{}};
	std::vector<int> pattern_node;
	std::vector<int> order;
	bool built = false;

public:
	int insert(const std::string& s) {
		assert(!built);
		int cur = 0;
		for (const char c : s) {
			const int x = c - 'a';
			assert(0 <= x && x < 26);
			if (trie[cur].next[x] == -1) {
				trie[cur].next[x] = trie.size();
				trie.emplace_back();
				trie.back().parent = cur;
			}
			cur = trie[cur].next[x];
		}
		pattern_node.push_back(cur);
		return pattern_node.size() - 1;
	}

	void build() {
		if (built) return;
		std::queue<int> queue;
		order.clear();
		for (int c = 0; c < 26; ++c) {
			if (trie[0].next[c] == -1) trie[0].next[c] = 0;
			else queue.push(trie[0].next[c]);
		}
		while (!queue.empty()) {
			const int u = queue.front();
			queue.pop();
			order.push_back(u);
			for (int c = 0; c < 26; ++c) {
				int& v = trie[u].next[c];
				if (v == -1) v = trie[trie[u].fail].next[c];
				else trie[v].fail = trie[trie[u].fail].next[c], queue.push(v);
			}
		}
		built = true;
	}

	std::vector<int> query(const std::string& text) const {
		assert(built);
		std::vector<int> visit(trie.size()), result(pattern_node.size());
		int cur = 0;
		for (const char c : text) {
			const int x = c - 'a';
			assert(0 <= x && x < 26);
			cur = trie[cur].next[x];
			++visit[cur];
		}
		for (int i = (int)order.size() - 1; i >= 0; --i)
			visit[trie[order[i]].fail] += visit[order[i]];
		for (int i = 0; i < (int)pattern_node.size(); ++i)
			result[i] = visit[pattern_node[i]];
		return result;
	}

	int size() const { return trie.size(); }
	std::vector<std::pair<int, int>> state_links() const {
		std::vector<std::pair<int, int>> result;
		for (const auto& node : trie) result.push_back({node.parent, node.fail});
		return result;
	}
	const std::vector<int>& pattern_vertices() const { return pattern_node; }
};

#endif
