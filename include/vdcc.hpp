#ifndef VDCC_HPP
#define VDCC_HPP

#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

#include "scc.hpp"

// 【无向图点双连通分量 VDCC (vdcc.hpp)】
// - 0-based 无向图邻接表 adj（每条无向边需双向添加），时间与空间复杂度均为 O(n + m)
// - get_vdcc(adj): 按【边集】返回每个点双连通分量 std::vector<std::pair<int, int>>；若存在无关联边的孤立顶点，对应追加一个空边集 {}

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
