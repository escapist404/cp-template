#ifndef TARJAN_LCA_HPP
#define TARJAN_LCA_HPP
#include <iostream>
#include <vector>
#include <utility>

// 0 index

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
