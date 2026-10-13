#ifndef TOPOSORT_HPP
#define TOPOSORT_HPP

#include <queue>
#include <vector>

// Returns a topological order, or an empty vector if the graph has a cycle.
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
