#ifndef MAX_FLOW_HPP
#define MAX_FLOW_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

// 【Dinic 最大流与最小割 (max_flow.hpp)】
// - ACL 风格 0-based 有向网络最大流 MaxFlow<Cap>，时间复杂度 O(V^2 E)（单位网络 O(E sqrt(V))）
// - add_edge(from, to, cap)   : 添加容量为 cap 的有向边（cap >= 0），返回边的 0-based 编号
// - flow(s, t, flow_limit)    : 计算并增广从 s 到 t 的最大流（可多次调用在残量网络上继续增广）
// - min_cut(s)                : 在跑完 flow(s, t) 后调用，返回长度 n 的 bool 数组表示残量网络中从 s 可达的最小割 S 集
// - get_edge(i) / edges() / change_edge(i, new_cap, new_flow): 查询或动态修改边容量与流量

template <typename Cap>
struct MaxFlow {
	MaxFlow() : n(0) {}
	MaxFlow(int n) : n(n), graph(n) {}

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

	edge get_edge(int i) const {
		assert(0 <= i && i < (int)positions.size());
		const auto [from, id] = positions[i];
		const auto e = graph[from][id];
		const auto reverse = graph[e.to][e.rev];
		return {from, e.to, e.cap + reverse.cap, reverse.cap};
	}

	std::vector<edge> edges() const {
		std::vector<edge> result;
		for (int i = 0; i < (int)positions.size(); ++i) result.push_back(get_edge(i));
		return result;
	}

	void change_edge(int i, Cap new_cap, Cap new_flow) {
		assert(0 <= i && i < (int)positions.size());
		assert(0 <= new_flow && new_flow <= new_cap);
		const auto [from, id] = positions[i];
		auto& e = graph[from][id];
		auto& reverse = graph[e.to][e.rev];
		e.cap = new_cap - new_flow;
		reverse.cap = new_flow;
	}

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
