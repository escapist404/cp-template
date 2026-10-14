#define PROBLEM "https://judge.yosupo.jp/problem/cycle_detection"

#include "../../include/fastio.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/scc.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, m;
	io >> n >> m;
	std::vector<std::vector<int>> graph(n);
	std::vector<std::vector<std::pair<int, int>>> adjacency(n);
	std::vector<std::pair<int, int>> edges(m);
	for (int i = 0; i < m; ++i) {
		io >> edges[i].first >> edges[i].second;
		graph[edges[i].first].push_back(edges[i].second);
		adjacency[edges[i].first].push_back({edges[i].second, i});
	}
	auto components = get_scc(graph);
	std::vector<int> component(n), chosen;
	for (int id = 0; id < (int)components.size(); ++id) {
		for (int v : components[id]) component[v] = id;
		if (components[id].size() > 1) chosen = components[id];
	}
	if (chosen.empty()) {
		for (int i = 0; i < m; ++i)
			if (edges[i].first == edges[i].second) {
				std::cout << 1 << '\n' << i << '\n';
				return 0;
			}
		std::cout << -1 << '\n';
		return 0;
	}
	int id = component[chosen[0]];
	std::vector<int> color(n), parent(n, -1), parent_edge(n, -1), next(n);
	for (int start : chosen) if (!color[start]) {
		std::vector<int> stack{start};
		color[start] = 1;
		while (!stack.empty()) {
			int u = stack.back();
			if (next[u] == (int)graph[u].size()) {
				color[u] = 2;
				stack.pop_back();
				continue;
			}
			auto [v, edge] = adjacency[u][next[u]++];
			if (component[v] != id) continue;
			if (!color[v]) {
				parent[v] = u;
				parent_edge[v] = edge;
				color[v] = 1;
				stack.push_back(v);
			} else if (color[v] == 1) {
				std::vector<int> cycle;
				for (int x = u; x != v; x = parent[x]) cycle.push_back(parent_edge[x]);
				std::reverse(cycle.begin(), cycle.end());
				cycle.push_back(edge);
				std::cout << cycle.size() << '\n';
				for (int edge : cycle) std::cout << edge << '\n';
				return 0;
			}
		}
	}
	std::cout << -1 << '\n';
}
