#ifndef BINARY_LIFTING_LCA_HPP
#define BINARY_LIFTING_LCA_HPP

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

#include "bit.hpp"

// ============================================================================
// 【树上倍增求最近公共祖先 Binary Lifting LCA (binary_lifting_lca.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class BinaryLiftingLCA
//   预处理 2^i 级祖先表求解有根树上的最近公共祖先，顶点编号为 0-based [0, n)。
// 构造函数与成员函数签名详解：
//   - BinaryLiftingLCA(int n = 0)
//     功能：初始化 n 个顶点的树结构，倍增层数 logn = bit_width(n)。时间复杂度 O(n)。
//   - void add_edge(int u, int v)
//     功能：添加一条连接 u 与 v (0 <= u, v < n) 的无向树边。O(1)。
//   - void prework(int root)
//     功能：【加完所有树边后必须调用一次】以 root 为根 DFS 预处理每个点的深度与 2^i 级祖先（越过根的祖先记为 -1）。
//     复杂度：时间与空间复杂度均为 O(n log n)。
//   - int lca(int u, int v) const
//     功能：查询顶点 u 与 v 的最近公共祖先编号。单次查询时间复杂度 O(log n)。
// ============================================================================

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
	// BinaryLiftingLCA(int n = 0) — 功能：初始化 n 个顶点的树结构，倍增层数 logn = bit_width(n)。时间复杂度 O(n)。
	BinaryLiftingLCA(int n = 0) : n(n), logn(n ? bit_width(n) : 1), adj(n) {}

	// void add_edge(int u, int v) — 功能：添加一条连接 u 与 v (0 <= u, v < n) 的无向树边。O(1)。
	void add_edge(int u, int v) {
		adj[u].push_back(v), adj[v].push_back(u);
	}

	// void prework(int root) — 功能：【加完所有树边后必须调用一次】以 root 为根 DFS 预处理每个点的深度与 2^i 级祖先（越过根的祖先记为 -1）。
	void prework(int root) {
		depth = std::vector<int>(n);
		anc = std::vector<std::vector<int>>(n, std::vector<int>(logn, -1));
		dfs(root, -1);
	}

	// int lca(int u, int v) const — 功能：查询顶点 u 与 v 的最近公共祖先编号。单次查询时间复杂度 O(log n)。
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
