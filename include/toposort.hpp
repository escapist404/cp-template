#ifndef TOPOSORT_HPP
#define TOPOSORT_HPP

#include <queue>
#include <vector>

// ============================================================================
// 【Kahn 算法拓扑排序与有向图判环 (toposort.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   std::vector<int> topological_sort(const std::vector<std::vector<int>>& adj)
// 参数与返回值详解：
//   - adj：0-based 有向图邻接表，adj[u] 包含所有有向边 u -> v 的终点 v（0 <= v < n）。
//   - 返回值 std::vector<int>：
//     - 若图为有向无环图 (DAG)，返回长度为 n = adj.size() 的合法拓扑序顶点序列（对每条边 u -> v，u 均排在 v 之前）；
//     - 若图中存在有向环（含自环 u -> u），返回【空向量 {}】（可通过 result.empty() && !adj.empty() 判环）。
// 复杂度：时间与空间复杂度均为 O(n + m)。
// ============================================================================

// std::vector<int> topological_sort(const std::vector<std::vector<int>>& adj) — 功能：参数与返回值详解：
std::vector<int> topological_sort(const std::vector<std::vector<int>>& adj) {
	std::vector<int> indegree(adj.size());
	for (const auto& edges : adj)
		for (int to : edges)
			++indegree[to];

	std::queue<int> queue;
	for (int i = 0; i < (int)adj.size(); ++i)
		if (!indegree[i])
			queue.push(i);

	std::vector<int> result;
	while (!queue.empty()) {
		int from = queue.front();
		queue.pop();
		result.push_back(from);
		for (int to : adj[from])
			if (!--indegree[to])
				queue.push(to);
	}
	if (result.size() != adj.size())
		result.clear();
	return result;
}

#endif
