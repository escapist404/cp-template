#ifndef TARJAN_LCA_HPP
#define TARJAN_LCA_HPP
#include <iostream>
#include <vector>
#include <utility>

// 【Tarjan 离线最近公共祖先 (tarjan_lca.hpp)】
// - 0-based 连通无向树邻接表 adj，以 root 为根，离线批量回答 queries 中的点对 {u, v} 的 LCA
// - 时间复杂度 O((n + q) alpha(n))，空间复杂度 O(n + q)，返回与 queries 等长的 LCA 结果数组

std::vector<int> tarjan_lca(const std::vector<std::vector<int>>& adj, int root, const std::vector<std::pair<int, int>>& queries) {
	const int n = adj.size();
	std::vector<std::vector<std::pair<int, int>>> queries_on_node(n);
	for (int i = 0; i < (int)queries.size(); ++i) {
		const auto &[u, v] = queries[i];
		queries_on_node[u].push_back({v, i});
		queries_on_node[v].push_back({u, i});
	}

	std::vector<int> res(queries.size());
	std::vector<bool> vis(n);
	std::vector<int> father(n, -1);
	const auto get = [&](int cur, const auto& self) -> int {
		return father[cur] == cur ? cur : father[cur] = self(father[cur], self);
	};

	const auto dfs = [&](int cur, int f, const auto& self) -> void {
		father[cur] = cur;

		for (const int to : adj[cur])
			if (to != f)
				self(to, cur, self);

		vis[cur] = true;
		for (const auto &[other, idx] : queries_on_node[cur])
			if (vis[other])
				res[idx] = get(other, get);

		father[cur] = f;
	};
	dfs(root, root, dfs);

	return res;
}

#endif
