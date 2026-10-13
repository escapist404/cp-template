#ifndef HLD_HPP
#define HLD_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

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
	HeavyLightDecomposition(int n = 0) : n(n), adj(n) {}

	void add_edge(int u, int v) {
		adj[u].push_back(v), adj[v].push_back(u);
	}

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
	std::pair<int, int> subtree_range(int u) const {
		return {position[u], position[u] + size[u]};
	}

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
