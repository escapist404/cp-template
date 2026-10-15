#ifndef FUNCTIONAL_GRAPH_HPP
#define FUNCTIONAL_GRAPH_HPP

#include <cassert>
#include <cstdint>
#include <queue>
#include <vector>

// ============================================================================
// 【函数图 / 基环内向树倍增跳转与环分解 (functional_graph.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class FunctionalGraph
//   每个顶点 0 <= u < n 恰有一条有向出边 u -> next[u]，每个弱连通分量是一棵指向唯一有向环的基环内向树。
// 构造函数与成员函数签名详解：
//   - FunctionalGraph(const std::vector<int>& next)
//     功能：预处理 64 层倍增表及每个点的所属基环编号、入环距离与环长（要求 0 <= next[u] < n）。
//     复杂度：时间与空间复杂度均为 O(64 * n)。
//   - int jump(int vertex, std::uint64_t steps)
//     功能：返回从顶点 vertex 出发沿出边连续走 steps 步（0 <= steps < 2^64）后到达的顶点编号。O(log(steps))。
//   - int component(int vertex)
//     功能：返回顶点 vertex 所属弱连通分量（对应唯一基环）的 0-based 编号；两点的后继轨迹最终汇合当且仅当 component 相同。O(1)。
//   - int distance_to_cycle(int vertex)
//     功能：返回从 vertex 出发首次进入其所在基环所需的最少步数（已在基环上的顶点返回 0）。O(1)。
//   - int cycle_length(int vertex)
//     功能：返回 vertex 所属弱连通分量的有向基环长度（>= 1）。O(1)。
// ============================================================================

class FunctionalGraph {
	enum { LOG = 64 };
	int n;
	std::vector<std::vector<int>> up;
	std::vector<int> cycle, distance, length;

public:
	// FunctionalGraph(const std::vector<int>& next) — 功能：预处理 64 层倍增表及每个点的所属基环编号、入环距离与环长（要求 0 <= next[u] < n）。
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

	// int jump(int vertex, std::uint64_t steps) — 功能：返回从顶点 vertex 出发沿出边连续走 steps 步（0 <= steps < 2^64）后到达的顶点编号。O(log(steps))。
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
