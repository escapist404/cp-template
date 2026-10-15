#ifndef TARJAN_LCA_HPP
#define TARJAN_LCA_HPP
#include <iostream>
#include <vector>
#include <utility>

// ============================================================================
// 【Tarjan 离线求最近公共祖先 Tarjan Offline LCA (tarjan_lca.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   std::vector<int> tarjan_lca(const std::vector<std::vector<int>>& adj,
//                               int root,
//                               const std::vector<std::pair<int, int>>& queries)
// 参数与返回值详解：
//   - adj：0-based 连通无向树邻接表（长度为 n）。
//   - root：有根树的根节点编号（0 <= root < n）。
//   - queries：离线询问点对列表，第 i 项为 {u_i, v_i}（0 <= u_i, v_i < n）。
//   - 返回值 std::vector<int>：与 queries 等长的结果数组，第 i 项为 u_i 与 v_i 的最近公共祖先编号。
// 复杂度：基于一次 DFS 与并查集，时间复杂度 O((n + q) alpha(n))，空间复杂度 O(n + q)。
// ============================================================================

// std::vector<int> tarjan_lca(const std::vector<std::vector<int>>& adj, int root, — 功能：const std::vector<std::pair<int, int>>& queries)
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
