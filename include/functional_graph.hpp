#ifndef FUNCTIONAL_GRAPH_HPP
#define FUNCTIONAL_GRAPH_HPP

#include <cassert>
#include <cstdint>
#include <queue>
#include <vector>

// 【函数图 / 基环内向树分解与倍增跳转 (functional_graph.hpp)】
// - 0-based 顶点编号 [0, n)，next[u] 表示 u 的唯一出边终点，构造时空复杂度 O(64 n)
// - jump(u, steps)       : O(log steps) 返回从 u 出发走 steps 步（支持 uint64_t 步数）到达的顶点
// - component(u)         : O(1) 返回 u 所属弱连通分量（对应唯一基环）的 0-based 编号
// - distance_to_cycle(u) : O(1) 返回从 u 进入基环所需的最少步数（环上顶点为 0）
// - cycle_length(u)      : O(1) 返回 u 所在分量基环的长度

class FunctionalGraph {
	enum { LOG = 64 };
	int n;
	std::vector<std::vector<int>> up;
	std::vector<int> cycle, distance, length;

public:
	FunctionalGraph(const std::vector<int>& next) : n(next.size()), up(LOG, next),
		cycle(n, -1), distance(n), length(n) {
		std::vector<int> indegree(n);
		std::vector<std::vector<int>> reverse(n);
		for (int i = 0; i < n; ++i) {
			assert(0 <= next[i] && next[i] < n);
			++indegree[next[i]];
			reverse[next[i]].push_back(i);
		}
		std::queue<int> queue;
		for (int i = 0; i < n; ++i) if (!indegree[i]) queue.push(i);
		while (!queue.empty()) {
			int u = queue.front(); queue.pop();
			int v = next[u];
			if (!--indegree[v]) queue.push(v);
		}
		int id = 0;
		for (int i = 0; i < n; ++i) if (indegree[i] && cycle[i] == -1) {
			std::vector<int> nodes;
			for (int v = i;; v = next[v]) {
				cycle[v] = id;
				nodes.push_back(v);
				if (next[v] == i) break;
			}
			for (int v : nodes) length[v] = nodes.size();
			++id;
		}
		std::queue<int> order;
		for (int i = 0; i < n; ++i) if (cycle[i] != -1) order.push(i);
		while (!order.empty()) {
			int u = order.front(); order.pop();
			for (int v : reverse[u]) if (cycle[v] == -1) {
				cycle[v] = cycle[u];
				distance[v] = distance[u] + 1;
				length[v] = length[u];
				order.push(v);
			}
		}
		for (int k = 1; k < LOG; ++k)
			for (int i = 0; i < n; ++i)
				up[k][i] = up[k - 1][up[k - 1][i]];
	}

	int jump(int vertex, std::uint64_t steps) {
		assert(0 <= vertex && vertex < n);
		for (int k = 0; steps; ++k, steps >>= 1)
			if (steps & 1) vertex = up[k][vertex];
		return vertex;
	}

	int component(int vertex) { return cycle.at(vertex); }
	int distance_to_cycle(int vertex) { return distance.at(vertex); }
	int cycle_length(int vertex) { return length.at(vertex); }
};

#endif
