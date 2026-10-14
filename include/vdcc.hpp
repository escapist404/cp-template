#ifndef VDCC_HPP
#define VDCC_HPP

#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

#include "scc.hpp"

// 0-based vertex-biconnected components (blocks) returned as edge lists; isolated vertices yield empty lists.

std::vector<std::vector<std::pair<int, int>>>
get_vdcc(const std::vector<std::vector<int>>& adj) {
	const int n = adj.size();
	std::stack<std::pair<int, int>> edge_stack;
	std::vector<std::vector<std::pair<int, int>>> vdcc;
	std::vector<int> children(n);

	tarjan_traverse(adj, true,
		[](int, const auto&, const auto&) {},
		[&](int cur, int to, const auto&, const auto&) {
			++children[cur];
			edge_stack.push({cur, to});
		},
		[&](int cur, int to, const auto& dfn, const auto& low) {
			if (low[to] < dfn[cur]) return;
			vdcc.emplace_back();
			while (true) {
				const auto edge = edge_stack.top();
				edge_stack.pop();
				vdcc.back().push_back(edge);
				if (edge == std::make_pair(cur, to)) break;
			}
		},
		[&](int cur, int to, const auto& dfn, auto& low) {
			if (dfn[to] < dfn[cur]) {
				low[cur] = std::min(low[cur], dfn[to]);
				edge_stack.push({cur, to});
			}
		},
		[](int, int, const auto&, const auto&) {},
		[&](int root, const auto&, const auto&) {
			if (!children[root]) vdcc.emplace_back();
		});

	return vdcc;
}

#endif
