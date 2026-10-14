#define PROBLEM "https://judge.yosupo.jp/problem/cycle_detection_undirected"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/dsu.hpp"
#include "../../include/fastio.hpp"

struct Edge {
	int u, v;
};

int main() {
	OptimizedIO<1 << 20> io;
	int n, m;
	io >> n >> m;
	std::vector<Edge> edges(m);
	std::vector<std::vector<std::pair<int, int>>> tree(n);
	DisjointSetUnion dsu(n);
	int extra = -1;
	for (int i = 0; i < m; ++i) {
		io >> edges[i].u >> edges[i].v;
		if (extra == -1 && dsu.same(edges[i].u, edges[i].v)) {
			extra = i;
		} else if (extra == -1) {
			dsu.merge(edges[i].u, edges[i].v);
			tree[edges[i].u].push_back({edges[i].v, i});
			tree[edges[i].v].push_back({edges[i].u, i});
		}
	}
	if (extra == -1) {
		std::cout << -1 << '\n';
		return 0;
	}
	int s = edges[extra].u, t = edges[extra].v;
	if (s == t) {
		std::cout << 1 << '\n' << s << '\n' << extra << '\n';
		return 0;
	}
	std::vector<int> parent(n, -1), parent_edge(n, -1), stack{s};
	parent[s] = s;
	while (!stack.empty()) {
		int u = stack.back();
		stack.pop_back();
		for (auto [v, edge] : tree[u]) if (parent[v] == -1) {
			parent[v] = u;
			parent_edge[v] = edge;
			stack.push_back(v);
		}
	}
	std::vector<int> vertices, cycle_edges;
	for (int v = t; v != s; v = parent[v]) {
		vertices.push_back(v);
		cycle_edges.push_back(parent_edge[v]);
	}
	vertices.push_back(s);
	std::reverse(vertices.begin(), vertices.end());
	std::reverse(cycle_edges.begin(), cycle_edges.end());
	cycle_edges.push_back(extra);
	std::cout << vertices.size() << '\n';
	for (int v : vertices) std::cout << v << ' ';
	std::cout << '\n';
	for (int edge : cycle_edges) std::cout << edge << ' ';
	std::cout << '\n';
}
