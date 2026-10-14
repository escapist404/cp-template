#ifndef CENTROID_DECOMPOSITION_HPP
#define CENTROID_DECOMPOSITION_HPP

#include <algorithm>
#include <vector>

// 0-based centroid decomposition tree. parent_of(root) is -1; build is O(n log n).

class CentroidDecomposition {
	std::vector<std::vector<int>> adj;
	std::vector<int> size, centroid_parent;
	std::vector<bool> removed;

	int calc_size(int cur, int parent) {
		size[cur] = 1;
		for (int to : adj[cur])
			if (to != parent && !removed[to])
				size[cur] += calc_size(to, cur);
		return size[cur];
	}

	int find_centroid(int cur, int parent, int total) {
		for (int to : adj[cur])
			if (to != parent && !removed[to] && size[to] > total / 2)
				return find_centroid(to, cur, total);
		return cur;
	}

	void build(int entry, int parent) {
		int centroid = find_centroid(entry, -1, calc_size(entry, -1));
		centroid_parent[centroid] = parent;
		removed[centroid] = true;
		for (int to : adj[centroid])
			if (!removed[to])
				build(to, centroid);
	}

public:
	CentroidDecomposition(const std::vector<std::vector<int>>& graph)
		: adj(graph), size(graph.size()), centroid_parent(graph.size(), -1),
		  removed(graph.size()) {
		if (!adj.empty())
			build(0, -1);
	}

	const std::vector<int>& parent() const { return centroid_parent; }
	int parent_of(int vertex) const { return centroid_parent[vertex]; }
};

#endif
