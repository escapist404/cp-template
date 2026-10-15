#ifndef AC_AUTOMATON_HPP
#define AC_AUTOMATON_HPP

#include <array>
#include <cassert>
#include <queue>
#include <string>
#include <vector>

// ============================================================================
// 【AC 自动机 Aho-Corasick Automaton (ac_automaton.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class AhoCorasick
//   面向 26 个小写英文字母 ('a' - 'z') 的多模式串匹配自动机（Trie 图 + 失配树）。
//   【状态转移生命周期约定】
//   必须严格遵循：【阶段 1：连续调用 insert(s) 插入所有模式串】 -> 【阶段 2：调用一次 build() 构建 Trie 图与 BFS 拓扑序】 -> 【阶段 3：调用 query(text) 匹配】。
//   一旦调用了 build()，built 置为 true，此后不可再调用 insert()（否则触发 assert(!built)）。
// 成员函数签名详解：
//   - int insert(const std::string& s)
//     功能：向 Trie 中插入仅含 'a'-'z' 的模式串 s，返回该模式串的 0-based 编号（按插入顺序依次为 0, 1, ..., m-1）。O(|s|)。
//   - void build()
//     功能：通过 BFS 构建所有状态的失配指针 fail、将空转移补全为 DFA 转移图，并记录 BFS 拓扑序 order。O(26 * |trie|)。
//   - std::vector<int> query(const std::string& text) const
//     功能：【调用前必须已调用 build()】扫描文本串 text 并沿失配树逆拓扑序自底向上汇总频数，
//           返回长度为模式串总数 m 的数组 result，其中 result[i] 表示第 i 个插入的模式串在 text 中的出现总次数。
//     复杂度：时间复杂度严格线性 O(|text| + |trie| + m)，空间复杂度 O(|trie| + m)。
//   - int size() const
//     功能：返回自动机中的状态节点总数（含 0 号根节点）。O(1)。
//   - std::vector<std::pair<int, int>> state_links() const
//     功能：导出每个状态 u (0 <= u < size()) 的 {Trie 父节点 parent, 失配指针 fail}（根节点 parent = -1, fail = 0）。O(|trie|)。
//   - const std::vector<int>& pattern_vertices() const
//     功能：返回长度为 m 的数组，第 i 项为第 i 个模式串在自动机中对应的终止状态节点编号。O(1)。
// ============================================================================

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
	// int insert(const std::string& s) — 功能：向 Trie 中插入仅含 'a'-'z' 的模式串 s，返回该模式串的 0-based 编号（按插入顺序依次为 0, 1, ..., m-1）。O(|s|)。
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

	// void build() — 功能：通过 BFS 构建所有状态的失配指针 fail、将空转移补全为 DFA 转移图，并记录 BFS 拓扑序 order。O(26 * |trie|)。
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

	// std::vector<int> query(const std::string& text) const — 功能：【调用前必须已调用 build()】扫描文本串 text 并沿失配树逆拓扑序自底向上汇总频数，
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
	// std::vector<std::pair<int, int>> state_links() const — 功能：导出每个状态 u (0 <= u < size()) 的 {Trie 父节点 parent, 失配指针 fail}（根节点 parent = -1, fail = 0）。O(|trie|)。
	std::vector<std::pair<int, int>> state_links() const {
		std::vector<std::pair<int, int>> result;
		for (const auto& node : trie) result.push_back({node.parent, node.fail});
		return result;
	}
	const std::vector<int>& pattern_vertices() const { return pattern_node; }
};

#endif
