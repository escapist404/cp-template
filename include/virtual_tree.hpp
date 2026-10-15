#ifndef VIRTUAL_TREE_HPP
#define VIRTUAL_TREE_HPP

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

#include "bit.hpp"

// 【虚树 Virtual Tree (virtual_tree.hpp)】
// - 0-based 连通无向树，预处理 O(n log n)，对给定的 k 个关键点构造虚树耗时 O(k log k)
// - build(keys): 传入关键点列表（自动去重并按 DFS 序排序，固定包含整棵树的根 root），返回 pair{vertices, edges}
//   - vertices : 虚树中包含的所有顶点（关键点及其两两 LCA，按 DFS 序排列，vertices[0] == root）
//   - edges    : 虚树的有向树边列表 {parent_in_vt, child_in_vt}

struct VirtualTree {
	std::vector<int> vertices;
	std::vector<std::pair<int, int>> edges;
};

class VirtualTreeBuilder {
	std::vector<std::vector<int>> adj, ancestor;
	std::vector<int> depth, tin, tout;
	int logn, timer = 0;

	void dfs(int cur, int parent) {
		tin[cur] = timer++;
		ancestor[cur][0] = parent;
		for (int i = 1; i < logn; ++i)
			ancestor[cur][i] = ancestor[ancestor[cur][i - 1]][i - 1];
		for (int to : adj[cur])
			if (to != parent) {
				depth[to] = depth[cur] + 1;
				dfs(to, cur);
			}
		tout[cur] = timer;
	}

	bool ancestor_of(int u, int v) const {
		return tin[u] <= tin[v] && tout[v] <= tout[u];
	}

	int lca(int u, int v) const {
		if (ancestor_of(u, v)) return u;
		if (ancestor_of(v, u)) return v;
		for (int i = logn - 1; i >= 0; --i)
			if (!ancestor_of(ancestor[u][i], v))
				u = ancestor[u][i];
		return ancestor[u][0];
	}

public:
	VirtualTreeBuilder(const std::vector<std::vector<int>>& graph, int root = 0)
		: adj(graph), depth(graph.size()), tin(graph.size()), tout(graph.size()), logn(1) {
		logn = std::max(1, bit_width((int)graph.size()));
		ancestor.assign(graph.size(), std::vector<int>(logn));
		if (!graph.empty()) {
			assert(0 <= root && root < (int)graph.size());
			dfs(root, root);
		}
	}

	VirtualTree build(std::vector<int> nodes) const {
		if (nodes.empty()) return {};
		for (int vertex : nodes)
			assert(0 <= vertex && vertex < (int)adj.size());
		std::sort(nodes.begin(), nodes.end(), [&](int a, int b) { return tin[a] < tin[b]; });
		nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
		int original = nodes.size();
		for (int i = 1; i < original; ++i)
			nodes.push_back(lca(nodes[i - 1], nodes[i]));
		std::sort(nodes.begin(), nodes.end(), [&](int a, int b) { return tin[a] < tin[b]; });
		nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());

		VirtualTree result{nodes, {}};
		std::vector<int> stack;
		for (int vertex : nodes) {
			while (!stack.empty() && !ancestor_of(stack.back(), vertex))
				stack.pop_back();
			if (!stack.empty())
				result.edges.push_back({stack.back(), vertex});
			stack.push_back(vertex);
		}
		return result;
	}
};

#endif
