#ifndef CENTROID_DECOMPOSITION_HPP
#define CENTROID_DECOMPOSITION_HPP

#include <algorithm>
#include <vector>

// ============================================================================
// 【树的重心分解 / 点分树 Centroid Decomposition (centroid_decomposition.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class CentroidDecomposition
//   对 0-based 连通无向树递归提取重心并构建点分树（重心树），点分树高度不超过 O(log n)。
// 构造函数与成员函数签名详解：
//   - CentroidDecomposition(const std::vector<std::vector<int>>& graph)
//     功能：输入 0-based 连通无向树邻接表 graph，在构造时完成整棵点分树的构建。
//     复杂度：时间复杂度 O(n log n)，空间复杂度 O(n)。
//   - const std::vector<int>& parent() const
//     功能：返回长度为 n 的点分树父节点数组 centroid_parent（点分树根节点的父节点为 -1）。O(1)。
//   - int parent_of(int vertex) const
//     功能：返回顶点 vertex (0 <= vertex < n) 在点分树上的直接父重心编号（根重心返回 -1）。O(1)。
// ============================================================================

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
	// CentroidDecomposition(const std::vector<std::vector<int>>& graph) — 功能：输入 0-based 连通无向树邻接表 graph，在构造时完成整棵点分树的构建。
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
