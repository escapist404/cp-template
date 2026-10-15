#ifndef SCC_HPP
#define SCC_HPP
#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

// ============================================================================
// 【Tarjan 强连通分量与缩点 DAG (scc.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解（0-based 顶点编号 [0, n)，时间与空间复杂度均为 O(n + m)）：
//   1. inline std::vector<std::vector<int>> get_scc(const std::vector<std::vector<int>>& adj)
//      功能：使用 Tarjan 算法求有向图 adj 的所有强连通分量 (SCC)。
//      返回：二维向量 components，每个元素为一个 SCC 的顶点列表。
//      【顺序约定】返回的 components 按 Tarjan 弹栈顺序排列，即缩点 DAG 的【逆拓扑序】（汇点分量在最前，源点分量在最后；逆序遍历即正拓扑序）。
//   2. inline std::vector<std::vector<int>> get_dag(
//          const std::vector<std::vector<int>>& adj,
//          const std::vector<std::vector<int>>& components)
//      功能：根据原图 adj 与已求出的强连通分量列表 components 构造缩点后的有向无环图 (DAG) 邻接表。
//      约定：缩点编号 0 .. components.size()-1 与 components 下标一一对应；自动过滤分量内部自环，跨分量的平行重边保留原样（不去重）。
//   3. inline std::vector<std::vector<int>> get_dag(const std::vector<std::vector<int>>& adj)
//      功能：便捷重载，等价于 get_dag(adj, get_scc(adj))。
//   4. template <typename EnterVertex, typename EnterTree, typename ExitTree,
//                typename BackEdge, typename FinishVertex, typename FinishRoot>
//      void tarjan_traverse(const std::vector<std::vector<int>>& graph, bool undirected, ...)
//      功能：通用 Tarjan 深度优先搜索底层遍历器（供 scc.hpp / edcc.hpp / vdcc.hpp 统一复用）。
// ============================================================================

template <typename EnterVertex, typename EnterTree, typename ExitTree,
		typename BackEdge, typename FinishVertex, typename FinishRoot>
// template <typename EnterVertex, typename EnterTree, typename ExitTree, typename BackEdge, typename FinishVertex, typename FinishRoot> void tarjan_traverse(const std::vector<std::vector<int>>& graph, bool undirected, ...) — 功能：通用 Tarjan 深度优先搜索底层遍历器（供 scc.hpp / edcc.hpp / vdcc.hpp 统一复用）。
void tarjan_traverse(const std::vector<std::vector<int>>& graph, bool undirected,
		EnterVertex enter_vertex, EnterTree enter_tree, ExitTree exit_tree,
		BackEdge back_edge,
		FinishVertex finish_vertex, FinishRoot finish_root) {
	const int n = graph.size();
	int timer = 0;
	std::vector<int> dfn(n, -1), low(n, -1);
	const auto dfs = [&](int cur, int parent, const auto& self) -> void {
		dfn[cur] = low[cur] = timer++;
		enter_vertex(cur, dfn, low);
		bool skipped_parent = false;
		for (int to : graph[cur]) {
			if (undirected && to == parent && !skipped_parent) {
				skipped_parent = true;
				continue;
			}
			if (dfn[to] == -1) {
				enter_tree(cur, to, dfn, low);
				self(to, cur, self);
				low[cur] = std::min(low[cur], low[to]);
				exit_tree(cur, to, dfn, low);
			} else {
				back_edge(cur, to, dfn, low);
			}
		}
		finish_vertex(cur, parent, dfn, low);
	};
	for (int root = 0; root < n; ++root) {
		if (dfn[root] == -1) {
			dfs(root, -1, dfs);
			finish_root(root, dfn, low);
		}
	}
}


// 0-based 索引

// inline std::vector<std::vector<int>> get_scc(const std::vector<std::vector<int>>& adj) — 功能：使用 Tarjan 算法求有向图 adj 的所有强连通分量 (SCC)。
inline std::vector<std::vector<int>> get_scc(const std::vector<std::vector<int>>& adj) {
	const int n = adj.size();
	std::stack<int> stack;
	std::vector<bool> in_stack(n, false);
	std::vector<std::vector<int>> components;

	tarjan_traverse(adj, false,
		[&](int cur, const auto&, const auto&) {
			stack.push(cur);
			in_stack[cur] = true;
		},
		[](int, int, const auto&, const auto&) {},
		[](int, int, const auto&, const auto&) {},
		[&](int cur, int to, const auto& dfn, auto& low) {
			if (in_stack[to]) low[cur] = std::min(low[cur], dfn[to]);
		},
		[&](int cur, int, const auto& dfn, const auto& low) {
			if (dfn[cur] != low[cur]) return;
			if (stack.empty() || !in_stack[cur]) return;
			components.emplace_back();
			while (true) {
				const int node = stack.top();
				stack.pop();
				in_stack[node] = false;
				components.back().push_back(node);
				if (node == cur) break;
			}
		},
		[](int, const auto&, const auto&) {});

	return components;
}

// inline std::vector<std::vector<int>> get_dag( const std::vector<std::vector<int>>& adj, const std::vector<std::vector<int>>& components) — 功能：根据原图 adj 与已求出的强连通分量列表 components 构造缩点后的有向无环图 (DAG) 邻接表。
inline std::vector<std::vector<int>> get_dag(const std::vector<std::vector<int>>& adj,
									  const std::vector<std::vector<int>>& components) {
	const int n = adj.size();
	if (components.empty()) return {};
	std::vector<int> color(n);
	for (int id = 0; id < (int)components.size(); ++id)
		for (const int v : components[id]) color[v] = id;
	std::vector<std::vector<int>> dag(components.size());
	for (int u = 0; u < n; ++u)
		for (const int v : adj[u])
			if (color[u] != color[v]) dag[color[u]].push_back(color[v]);
	return dag;
}

inline std::vector<std::vector<int>> get_dag(
		const std::vector<std::vector<int>>& adj) {
	return get_dag(adj, get_scc(adj));
}

#endif
