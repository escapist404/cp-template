#ifndef BINARY_LIFTING_LCA_HPP
#define BINARY_LIFTING_LCA_HPP

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

#include "bit.hpp"

// 【树上倍增求最近公共祖先 (binary_lifting_lca.hpp)】
// - 0-based 顶点编号 [0, n)，调用 add_edge(u, v) 加满树边后必须调用一次 prework(root)
// - 预处理时空复杂度 O(n log n)，单次 lca(u, v) 查询时间复杂度 O(log n)

class BinaryLiftingLCA {
	int n, logn;
	std::vector<std::vector<int>> adj;
	std::vector<int> depth;
	std::vector<std::vector<int>> anc;

	void dfs(int cur, int f) {
		for (const int to : adj[cur]) {
			if (to == f)
				continue;

			depth[to] = depth[cur] + 1;
			anc[to][0] = cur;
			for (int i = 1; i < logn && ~anc[to][i - 1]; ++i)
				anc[to][i] = anc[anc[to][i - 1]][i - 1];
			dfs(to, cur);
		}
	}

public:
	BinaryLiftingLCA(int n = 0) : n(n), logn(n ? bit_width(n) : 1), adj(n) {}

	void add_edge(int u, int v) {
		adj[u].push_back(v), adj[v].push_back(u);
	}

	void prework(int root) {
		depth = std::vector<int>(n);
		anc = std::vector<std::vector<int>>(n, std::vector<int>(logn, -1));
		dfs(root, -1);
	}

	int lca(int u, int v) const {
		if (depth[u] > depth[v])
			std::swap(u, v);
		for (int i = logn - 1; i >= 0; --i)
			if (~anc[v][i] && depth[anc[v][i]] >= depth[u])
				v = anc[v][i];
		if (u == v)
			return u;
		for (int i = logn - 1; i >= 0; --i)
			if (anc[u][i] != anc[v][i])
				u = anc[u][i], v = anc[v][i];
		return anc[u][0];
	}
};

#endif
