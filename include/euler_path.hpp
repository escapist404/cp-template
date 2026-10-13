#ifndef EULER_PATH_HPP
#define EULER_PATH_HPP

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

struct EulerPath {
	bool exists = false;
	std::vector<int> vertices, edges;
};

// Edge IDs are indices in edges. Supports parallel edges and self-loops.
EulerPath euler_path(int n, const std::vector<std::pair<int, int>>& edges,
		bool directed = false, int start = -1) {
	assert(n >= 0);
	assert(start == -1 || (0 <= start && start < n));
	std::vector<std::vector<std::pair<int, int>>> adj(n);
	std::vector<int> in(n), out(n);
	for (int id = 0; id < (int)edges.size(); ++id) {
		const auto [u, v] = edges[id];
		assert(0 <= u && u < n && 0 <= v && v < n);
		adj[u].push_back({v, id});
		++out[u];
		++in[v];
		if (!directed) {
			adj[v].push_back({u, id});
			++out[v];
			++in[u];
		}
	}
	int required = -1, ends = 0, begins = 0, first = -1;
	for (int u = 0; u < n; ++u) {
		if (out[u] && first == -1) first = u;
		if (directed) {
			if (out[u] - in[u] == 1) {
				required = u;
				++begins;
			} else if (in[u] - out[u] == 1) {
				++ends;
			} else if (in[u] != out[u]) {
				return {};
			}
		} else if (out[u] & 1) {
			required = u;
			++ends;
		}
	}
	if (directed ? !((begins == 0 && ends == 0) || (begins == 1 && ends == 1))
	             : !(ends == 0 || ends == 2)) return {};
	if (edges.empty()) {
		return {true, n ? std::vector<int>{start == -1 ? 0 : start} : std::vector<int>{}, {}};
	}
	if (start == -1) start = required == -1 ? first : required;
	if (!out[start] || (directed && begins && start != required) ||
		(!directed && ends && !(out[start] & 1))) return {};
	std::vector<int> index(n);
	std::vector<char> used(edges.size());
	std::vector<std::pair<int, int>> stack{{start, -1}};
	EulerPath result;
	while (!stack.empty()) {
		const int u = stack.back().first;
		while (index[u] < (int)adj[u].size() && used[adj[u][index[u]].second]) ++index[u];
		if (index[u] == (int)adj[u].size()) {
			result.vertices.push_back(u);
			if (stack.back().second != -1) result.edges.push_back(stack.back().second);
			stack.pop_back();
		} else {
			const auto [v, id] = adj[u][index[u]++];
			used[id] = true;
			stack.push_back({v, id});
		}
	}
	if ((int)result.edges.size() != (int)edges.size()) return {};
	std::reverse(result.vertices.begin(), result.vertices.end());
	std::reverse(result.edges.begin(), result.edges.end());
	result.exists = true;
	return result;
}

#endif
