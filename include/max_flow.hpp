#ifndef MAX_FLOW_HPP
#define MAX_FLOW_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

// ============================================================================
// 【Dinic 最大流与最小割 Max Flow (max_flow.hpp)】
// ----------------------------------------------------------------------------
// 模板结构体签名：template <typename Cap> struct MaxFlow
//   ACL 风格有向网络最大流，顶点编号为 0-based [0, n)，容量类型 Cap 需为有符号算术类型。
// 内部边信息结构体签名：
//   struct edge { int from, to; Cap cap, flow; }; // 起点、终点、原始容量 cap、当前流量 flow
// 构造函数与成员函数签名详解：
//   - MaxFlow() / MaxFlow(int n)
//     功能：构造 0 个或 n 个顶点的空流网络。O(n)。
//   - int add_edge(int from, int to, Cap cap)
//     功能：添加有向边 from -> to，容量为 cap（要求 0 <= from, to < n 且 cap >= 0，支持自环），返回该边的 0-based 编号。O(1)。
//   - Cap flow(int s, int t)
//   - Cap flow(int s, int t, Cap flow_limit)
//     功能：在当前残量网络上从源点 s 到汇点 t 增广最大流（最多再增广 flow_limit 流量，要求 s != t），返回本次新增流量。
//     复杂度：一般网络 O(V^2 E)，二分图匹配等单位网络 O(E sqrt(V))。
//   - std::vector<bool> min_cut(int s) const
//     功能：在跑完 flow(s, t) 后调用，返回长度为 n 的 bool 数组，visited[v] == true 表示残量网络中从 s 可达（属于最小割 S 集）。O(V + E)。
//   - edge get_edge(int i) const / std::vector<edge> edges() const
//     功能：按边编号 i 查询单条边或导出全部边的 {from, to, cap, flow} 状态。
//   - void change_edge(int i, Cap new_cap, Cap new_flow)
//     功能：动态将第 i 条边的容量与流量直接调整为 new_cap 和 new_flow（要求 0 <= new_flow <= new_cap）。O(1)。
// ============================================================================

template <typename Cap>
struct MaxFlow {
	// MaxFlow() — 功能：构造 0 个或 n 个顶点的空流网络。O(n)。
	MaxFlow() : n(0) {}
	// MaxFlow(int n) — 功能：构造 0 个或 n 个顶点的空流网络。O(n)。
	MaxFlow(int n) : n(n), graph(n) {}

	// int add_edge(int from, int to, Cap cap) — 功能：添加有向边 from -> to，容量为 cap（要求 0 <= from, to < n 且 cap >= 0，支持自环），返回该边的 0-based 编号。O(1)。
	int add_edge(int from, int to, Cap cap) {
		assert(0 <= from && from < n);
		assert(0 <= to && to < n);
		assert(0 <= cap);
		const int id = positions.size();
		positions.push_back({from, graph[from].size()});
		const int from_id = graph[from].size();
		int to_id = graph[to].size();
		if (from == to) ++to_id;
		graph[from].push_back({to, to_id, cap});
		graph[to].push_back({from, from_id, 0});
		return id;
	}

	struct edge {
		int from, to;
		Cap cap, flow;
	};

	// edge get_edge(int i) const — 功能：按边编号 i 查询单条边或导出全部边的 {from, to, cap, flow} 状态。
	edge get_edge(int i) const {
		assert(0 <= i && i < (int)positions.size());
		const auto [from, id] = positions[i];
		const auto e = graph[from][id];
		const auto reverse = graph[e.to][e.rev];
		return {from, e.to, e.cap + reverse.cap, reverse.cap};
	}

	// std::vector<edge> edges() const — 功能：按边编号 i 查询单条边或导出全部边的 {from, to, cap, flow} 状态。
	std::vector<edge> edges() const {
		std::vector<edge> result;
		for (int i = 0; i < (int)positions.size(); ++i) result.push_back(get_edge(i));
		return result;
	}

	// void change_edge(int i, Cap new_cap, Cap new_flow) — 功能：动态将第 i 条边的容量与流量直接调整为 new_cap 和 new_flow（要求 0 <= new_flow <= new_cap）。O(1)。
	void change_edge(int i, Cap new_cap, Cap new_flow) {
		assert(0 <= i && i < (int)positions.size());
		assert(0 <= new_flow && new_flow <= new_cap);
		const auto [from, id] = positions[i];
		auto& e = graph[from][id];
		auto& reverse = graph[e.to][e.rev];
		e.cap = new_cap - new_flow;
		reverse.cap = new_flow;
	}

	// Cap flow(int s, int t, Cap flow_limit) — 功能：在当前残量网络上从源点 s 到汇点 t 增广最大流（最多再增广 flow_limit 流量，要求 s != t），返回本次新增流量。
	Cap flow(int s, int t) {
		return flow(s, t, std::numeric_limits<Cap>::max());
	}

	Cap flow(int s, int t, Cap flow_limit) {
		assert(0 <= s && s < n && 0 <= t && t < n && s != t);
		std::vector<int> level(n), iter(n);
		std::queue<int> queue;
		auto bfs = [&]() {
			std::fill(level.begin(), level.end(), -1);
			level[s] = 0;
			while (!queue.empty()) queue.pop();
			queue.push(s);
			while (!queue.empty()) {
				const int v = queue.front();
				queue.pop();
				for (const auto& e : graph[v]) {
					if (e.cap == 0 || level[e.to] >= 0) continue;
					level[e.to] = level[v] + 1;
					if (e.to == t) return;
					queue.push(e.to);
				}
			}
		};
		auto dfs = [&](auto self, int v, Cap up) -> Cap {
			if (v == s) return up;
			Cap result = 0;
			const int level_v = level[v];
			for (int& i = iter[v]; i < (int)graph[v].size(); ++i) {
				auto& e = graph[v][i];
				if (level_v <= level[e.to] || graph[e.to][e.rev].cap == 0) continue;
				const Cap pushed = self(self, e.to,
					std::min(up - result, graph[e.to][e.rev].cap));
				if (pushed <= 0) continue;
				e.cap += pushed;
				graph[e.to][e.rev].cap -= pushed;
				result += pushed;
				if (result == up) return result;
			}
			level[v] = n;
			return result;
		};
		Cap result = 0;
		while (result < flow_limit) {
			bfs();
			if (level[t] == -1) break;
			std::fill(iter.begin(), iter.end(), 0);
			const Cap pushed = dfs(dfs, t, flow_limit - result);
			if (!pushed) break;
			result += pushed;
		}
		return result;
	}

	// std::vector<bool> min_cut(int s) const — 功能：在跑完 flow(s, t) 后调用，返回长度为 n 的 bool 数组，visited[v] == true 表示残量网络中从 s 可达（属于最小割 S 集）。O(V + E)。
	std::vector<bool> min_cut(int s) const {
		assert(0 <= s && s < n);
		std::vector<bool> visited(n);
		std::queue<int> queue;
		queue.push(s);
		visited[s] = true;
		while (!queue.empty()) {
			const int v = queue.front();
			queue.pop();
			for (const auto& e : graph[v]) if (e.cap && !visited[e.to]) {
				visited[e.to] = true;
				queue.push(e.to);
			}
		}
		return visited;
	}

private:
	struct internal_edge { int to, rev; Cap cap; };
	int n;
	std::vector<std::pair<int, int>> positions;
	std::vector<std::vector<internal_edge>> graph;
};

#endif
