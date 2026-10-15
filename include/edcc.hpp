#ifndef EDCC_HPP
#define EDCC_HPP

#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <algorithm>

#include "scc.hpp"

// 【无向图边双连通分量 EDCC (edcc.hpp)】
// - 0-based 无向图邻接表 adj（每条无向边需双向添加，支持重边与自环），平均时间复杂度 O(n + m)
// - get_edcc(adj): 识别所有割边（桥）并通过 BFS 将全部顶点划分为不相交的边双连通分量点集

std::vector<std::vector<int>> get_edcc(const std::vector<std::vector<int>>& adj) {
	const int n = adj.size();
	std::vector<std::unordered_set<int>> bridge(n);

	tarjan_traverse(adj, true,
		[](int, const auto&, const auto&) {},
		[](int, int, const auto&, const auto&) {},
		[&](int cur, int to, const auto& dfn, const auto& low) {
			if (low[to] > dfn[cur]) {
				bridge[cur].insert(to);
				bridge[to].insert(cur);
			}
		},
		[](int cur, int to, const auto& dfn, auto& low) {
			low[cur] = std::min(low[cur], dfn[to]);
		},
		[](int, int, const auto&, const auto&) {},
		[](int, const auto&, const auto&) {});

	std::vector<int> edcc_idx(n, -1);
	std::vector<std::vector<int>> edcc;

	for (int u = 0; u < n; ++u) {
		if (~edcc_idx[u])
			continue;

		edcc.push_back({});
		edcc_idx[u] = edcc.size() - 1;
		std::queue<int> q;
		q.push(u);

		while (!q.empty()) {
			int cur = q.front();
			q.pop();
			edcc.back().push_back(cur);

			for (const int to : adj[cur]) {
				if (~edcc_idx[to])
					continue;

				// 擦除一条割边记录，防止平行重边误判
				auto it = bridge[cur].find(to);
				if (it != bridge[cur].end()) {
					bridge[cur].erase(it);
					bridge[to].erase(bridge[to].find(cur));
					continue;
				}

				edcc_idx[to] = edcc.size() - 1;
				q.push(to);
			}
		}
	}

	return edcc;
}

#endif
