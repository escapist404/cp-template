#ifndef BLOSSOM_HPP
#define BLOSSOM_HPP

#include <algorithm>
#include <cassert>
#include <queue>
#include <vector>

// Edmonds' blossom algorithm for maximum matching in a general graph.
class GeneralMatching {
	int n;
	std::vector<std::vector<int>> graph;
	std::vector<int> match, parent, base;
	std::vector<bool> used, blossom;
	std::queue<int> queue;

	int lca(int a, int b) {
		std::vector<bool> seen(n);
		while (true) {
			a = base[a];
			seen[a] = true;
			if (match[a] == -1) break;
			a = parent[match[a]];
		}
		while (true) {
			b = base[b];
			if (seen[b]) return b;
			b = parent[match[b]];
		}
	}

	void mark_path(int v, int b, int child) {
		while (base[v] != b) {
			blossom[base[v]] = blossom[base[match[v]]] = true;
			parent[v] = child;
			child = match[v];
			v = parent[match[v]];
		}
	}

	int find_path(int root) {
		used.assign(n, false);
		parent.assign(n, -1);
		base.resize(n);
		for (int i = 0; i < n; ++i) base[i] = i;
		while (!queue.empty()) queue.pop();
		queue.push(root);
		used[root] = true;
		while (!queue.empty()) {
			int u = queue.front(); queue.pop();
			for (int v : graph[u]) {
				if (base[u] == base[v] || match[u] == v) continue;
				if (v == root || (match[v] != -1 && parent[match[v]] != -1)) {
					int cur = lca(u, v);
					blossom.assign(n, false);
					mark_path(u, cur, v);
					mark_path(v, cur, u);
					for (int i = 0; i < n; ++i) if (blossom[base[i]]) {
						base[i] = cur;
						if (!used[i]) used[i] = true, queue.push(i);
					}
				} else if (parent[v] == -1) {
					parent[v] = u;
					if (match[v] == -1) return v;
					int next = match[v];
					used[next] = true;
					queue.push(next);
				}
			}
		}
		return -1;
	}

public:
	GeneralMatching(int n) : n(n), graph(n), match(n, -1) {}

	void add_edge(int u, int v) {
		assert(0 <= u && u < n && 0 <= v && v < n && u != v);
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	int max_matching() {
		std::fill(match.begin(), match.end(), -1);
		int result = 0;
		for (int i = 0; i < n; ++i) if (match[i] == -1) {
			int v = find_path(i);
			if (v == -1) continue;
			++result;
			for (int cur = v; cur != -1;) {
				int prev = parent[cur];
				int next = prev == -1 ? -1 : match[prev];
				match[cur] = prev;
				if (prev != -1) match[prev] = cur;
				cur = next;
			}
		}
		return result;
	}

	const std::vector<int>& matching() { return match; }
};

#endif
