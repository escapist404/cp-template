#ifndef VDCC_HPP
#define VDCC_HPP

#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

#include "scc.hpp"

// ============================================================================
// 【无向图点双连通分量 Vertex-Biconnected Components / VDCC (vdcc.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   std::vector<std::vector<std::pair<int, int>>> get_vdcc(const std::vector<std::vector<int>>& adj)
// 参数与返回值详解：
//   - adj：0-based 无向图邻接表（每条无向边需双向添加；不支持自环）。
//   - 返回值 std::vector<std::vector<std::pair<int, int>>>：
//     因为割点可同时属于多个点双连通分量，而每条边唯一属于一个点双，本函数按【边集】返回各个点双连通分量：
//     - 对含有边的点双，返回该分量内部的所有树边与返祖边 {u, v}；
//     - 对没有任何关联边的孤立顶点，会在结果末尾追加一个空边集 {} 以标识该孤立点分量。
// 复杂度：时间与空间复杂度均为 O(n + m)。
// ============================================================================

std::vector<std::vector<std::pair<int, int>>>
// std::vector<std::vector<std::pair<int, int>>> get_vdcc(const std::vector<std::vector<int>>& adj) — 功能：参数与返回值详解：
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
