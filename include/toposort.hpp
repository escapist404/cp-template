#ifndef TOPOSORT_HPP
#define TOPOSORT_HPP

#include <queue>
#include <vector>

// 【Kahn 拓扑排序与有向图判环 (toposort.hpp)】
// - 0-based 有向图邻接表 adj，时间复杂度 O(n + m)
// - 若图为有向无环图 (DAG)，返回长度为 n 的拓扑序顶点配列；若图中存在有向环（含自环），返回空向量 {}

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
