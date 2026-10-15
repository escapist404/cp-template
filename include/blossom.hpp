#ifndef BLOSSOM_HPP
#define BLOSSOM_HPP

#include <algorithm>
#include <cassert>
#include <queue>
#include <vector>

// ============================================================================
// 【Edmonds 带花树一般无向图最大匹配 (blossom.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class GeneralMatching
//   通过奇环缩花（Blossom）在含奇环的一般无向图上求最大匹配，顶点编号为 0-based [0, n)。
// 构造函数与成员函数签名详解：
//   - GeneralMatching(int n)
//     功能：初始化包含 n 个顶点 [0, n) 的空无向图。时间复杂度 O(n)。
//   - void add_edge(int u, int v)
//     功能：添加一条连接 u 与 v 的无向边（要求 0 <= u, v < n 且 u != v，不支持自环）。O(1)。
//   - int max_matching()
//     功能：运行带花树增广算法求解最大匹配，返回最大匹配对数。时间复杂度 O(n^3)，空间复杂度 O(n + m)。
//   - const std::vector<int>& matching()
//     功能：【在调用 max_matching() 后使用】返回长度为 n 的匹配数组 match：
//           若顶点 u 已匹配，则 match[u] 为其配对顶点 v（且满足 match[v] == u）；若未匹配则为 -1。O(1)。
// ============================================================================

class GeneralMatching {
	int n;
	std::vector<std::vector<int>> graph;
	std::vector<int> match, parent, base;
	std::vector<bool> used, blossom;
	std::queue<int> queue;

	int lca(int a, int b) {
		std::vector<bool> seen(n);
		while (true) {
			a = base[a];
			seen[a] = true;
			if (match[a] == -1) break;
			a = parent[match[a]];
		}
		while (true) {
			b = base[b];
			if (seen[b]) return b;
			b = parent[match[b]];
		}
	}

	void mark_path(int v, int b, int child) {
		while (base[v] != b) {
			blossom[base[v]] = blossom[base[match[v]]] = true;
			parent[v] = child;
			child = match[v];
			v = parent[match[v]];
		}
	}

	int find_path(int root) {
		used.assign(n, false);
		parent.assign(n, -1);
		base.resize(n);
		for (int i = 0; i < n; ++i) base[i] = i;
		while (!queue.empty()) queue.pop();
		queue.push(root);
		used[root] = true;
		while (!queue.empty()) {
			int u = queue.front(); queue.pop();
			for (int v : graph[u]) {
				if (base[u] == base[v] || match[u] == v) continue;
				if (v == root || (match[v] != -1 && parent[match[v]] != -1)) {
					int cur = lca(u, v);
					blossom.assign(n, false);
					mark_path(u, cur, v);
					mark_path(v, cur, u);
					for (int i = 0; i < n; ++i) if (blossom[base[i]]) {
						base[i] = cur;
						if (!used[i]) used[i] = true, queue.push(i);
					}
				} else if (parent[v] == -1) {
					parent[v] = u;
					if (match[v] == -1) return v;
					int next = match[v];
					used[next] = true;
					queue.push(next);
				}
			}
		}
		return -1;
	}

public:
	// GeneralMatching(int n) — 功能：初始化包含 n 个顶点 [0, n) 的空无向图。时间复杂度 O(n)。
	GeneralMatching(int n) : n(n), graph(n), match(n, -1) {}

	// void add_edge(int u, int v) — 功能：添加一条连接 u 与 v 的无向边（要求 0 <= u, v < n 且 u != v，不支持自环）。O(1)。
	void add_edge(int u, int v) {
		assert(0 <= u && u < n && 0 <= v && v < n && u != v);
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	// int max_matching() — 功能：运行带花树增广算法求解最大匹配，返回最大匹配对数。时间复杂度 O(n^3)，空间复杂度 O(n + m)。
	int max_matching() {
		std::fill(match.begin(), match.end(), -1);
		int result = 0;
		for (int i = 0; i < n; ++i) if (match[i] == -1) {
			int v = find_path(i);
			if (v == -1) continue;
			++result;
			for (int cur = v; cur != -1;) {
				int prev = parent[cur];
				int next = prev == -1 ? -1 : match[prev];
				match[cur] = prev;
				if (prev != -1) match[prev] = cur;
				cur = next;
			}
		}
		return result;
	}

	const std::vector<int>& matching() { return match; }
};

#endif
