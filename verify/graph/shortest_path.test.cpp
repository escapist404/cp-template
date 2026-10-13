#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"

#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

#include "../../include/dijkstra.hpp"

struct Edge {
	int from, to;
	long long weight;
};

int main() {
	int n, m, s, t;
	std::cin >> n >> m >> s >> t;
	std::vector<std::vector<std::pair<int, long long>>> graph(n);
	std::vector<std::vector<int>> edge_ids(n);
	std::vector<Edge> edges;
	for (int i = 0; i < m; ++i) {
		Edge edge;
		std::cin >> edge.from >> edge.to >> edge.weight;
		graph[edge.from].push_back({edge.to, edge.weight});
		edge_ids[edge.from].push_back(i);
		edges.push_back(edge);
	}
	auto distance = dijkstra<long long>(graph, {s});
	const long long inf = __LONG_LONG_MAX__;
	if (distance[t] == inf) {
		std::cout << -1 << '\n';
		return 0;
	}
	std::vector<int> parent(n, -1);
	parent[s] = s;
	std::queue<int> queue;
	queue.push(s);
	while (!queue.empty()) {
		int u = queue.front();
		queue.pop();
		for (int id : edge_ids[u]) {
			const auto& edge = edges[id];
			if (parent[edge.to] == -1 && distance[u] + edge.weight == distance[edge.to]) {
				parent[edge.to] = u;
				queue.push(edge.to);
			}
		}
	}
	std::vector<std::pair<int, int>> path;
	for (int current = t; current != s; current = parent[current])
		path.push_back({parent[current], current});
	std::reverse(path.begin(), path.end());
	std::cout << distance[t] << ' ' << path.size() << '\n';
	for (auto [u, v] : path) std::cout << u << ' ' << v << '\n';
}
