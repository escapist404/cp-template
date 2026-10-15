#ifndef HLD_HPP
#define HLD_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

// ============================================================================
// 【重链剖分 / 树链剖分 Heavy-Light Decomposition (hld.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class HeavyLightDecomposition
//   将 0-based 连通有根树剖分为重链，使任意两点路径可拆分为 O(log n) 段连续 DFS 序区间，且每棵子树对应单个连续区间。
// 构造函数与成员函数签名详解：
//   - HeavyLightDecomposition(int n = 0)
//     功能：初始化包含 n 个顶点 [0, n) 的空树结构。O(n)。
//   - void add_edge(int u, int v)
//     功能：添加一条连接 u 与 v (0 <= u, v < n) 的无向树边。O(1)。
//   - void prework(int root)
//     功能：【必须在加完所有边后调用一次】以 root 为根执行两次 DFS，计算父节点、深度、重儿子、链顶 head 与 DFS 序 pos。O(n)。
//   - int parent(int u) const
//     功能：返回顶点 u 的父节点编号（根节点 root 的父节点为 -1）。O(1)。
//   - int depth_of(int u) const
//     功能：返回顶点 u 的深度（根节点深度为 0）。O(1)。
//   - int head(int u) const
//     功能：返回顶点 u 所在重链的链顶（该重链上深度最小的顶点）编号。O(1)。
//   - int pos(int u) const
//     功能：返回顶点 u 在重链优先 DFS 序中的 0-based 下标 in [0, n)；同条重链从链顶 head(u) 到 u 对应连续闭区间 [pos(head(u)), pos(u)]。O(1)。
//   - std::pair<int, int> subtree_range(int u) const
//     功能：返回以 u 为根的子树在 pos 序中对应的【左闭右开区间 [l, r)】（即 {pos(u), pos(u) + size[u]}）。O(1)。
//   - int lca(int u, int v) const
//     功能：沿重链跳转到同一条重链上求 u 与 v 的最近公共祖先。时间复杂度 O(log n)。
// ============================================================================

class HeavyLightDecomposition {
	int n;
	std::vector<std::vector<int>> adj;
	std::vector<int> father, depth, size, heavy, top, position;
	int timer = 0;

	void dfs_1(int cur, int f) {
		size[cur] = 1;

		for (const int to : adj[cur]) {
			if (to == f)
				continue;

			depth[to] = depth[cur] + 1;
			father[to] = cur;

			dfs_1(to, cur);

			size[cur] += size[to];
			if (!~heavy[cur] || size[to] > size[heavy[cur]])
				heavy[cur] = to;
		}
	}

	void dfs_2(int cur, int cur_top) {
		top[cur] = cur_top;
		position[cur] = timer++;

		if (!~heavy[cur])
			return;

		dfs_2(heavy[cur], cur_top);
		for (const int to : adj[cur])
			if (to != father[cur] && to != heavy[cur])
				dfs_2(to, to);
	}

public:
	// HeavyLightDecomposition(int n = 0) — 功能：初始化包含 n 个顶点 [0, n) 的空树结构。O(n)。
	HeavyLightDecomposition(int n = 0) : n(n), adj(n) {}

	// void add_edge(int u, int v) — 功能：添加一条连接 u 与 v (0 <= u, v < n) 的无向树边。O(1)。
	void add_edge(int u, int v) {
		adj[u].push_back(v), adj[v].push_back(u);
	}

	// void prework(int root) — 功能：【必须在加完所有边后调用一次】以 root 为根执行两次 DFS，计算父节点、深度、重儿子、链顶 head 与 DFS 序 pos。O(n)。
	void prework(int root) {
		depth = size = std::vector<int>(n);
		father = heavy = top = position = std::vector<int>(n, -1);
		timer = 0;
		dfs_1(root, -1), dfs_2(root, root);
	}

	int parent(int u) const { return father[u]; }
	int depth_of(int u) const { return depth[u]; }
	int head(int u) const { return top[u]; }
	int pos(int u) const { return position[u]; }
	// std::pair<int, int> subtree_range(int u) const — 功能：返回以 u 为根的子树在 pos 序中对应的【左闭右开区间 [l, r)】（即 {pos(u), pos(u) + size[u]}）。O(1)。
	std::pair<int, int> subtree_range(int u) const {
		return {position[u], position[u] + size[u]};
	}

	// int lca(int u, int v) const — 功能：沿重链跳转到同一条重链上求 u 与 v 的最近公共祖先。时间复杂度 O(log n)。
	int lca(int u, int v) const {
		while (top[u] != top[v]) {
			if (depth[top[u]] < depth[top[v]])
				std::swap(u, v);
			u = father[top[u]];
		}
		if (depth[u] > depth[v])
			std::swap(u, v);
		return u;
	}
};

#endif
