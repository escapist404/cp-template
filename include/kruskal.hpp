#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <algorithm>
#include <utility>
#include <vector>

#include "dsu.hpp"

// ============================================================================
// 【Kruskal 最小生成树 / 最小生成森林 (kruskal.hpp)】
// ----------------------------------------------------------------------------
// 边结构体签名：
//   template <typename T> struct KruskalEdge { int u, v; T weight; };
//   表示连接 0-based 顶点 u 与 v、权值为 weight 的无向边。
// 算法函数签名：
//   template <typename T>
//   std::pair<T, std::vector<KruskalEdge<T>>> kruskal(int n, std::vector<KruskalEdge<T>> edges)
// 参数与返回值详解：
//   - n：图的总顶点数，顶点编号为 0-based [0, n)。
//   - edges：无向边列表（按值传递并在内部按 weight 升序排序）。
//   - 返回值 std::pair{total_weight, mst_edges}：
//     1. total_weight (T)：所选生成树（或生成森林）的边权总和；
//     2. mst_edges (std::vector<KruskalEdge<T>>)：被选入最小生成树/森林的边集（按权值升序）。
//     【连通性判定】当 n == 0 或 mst_edges.size() == n - 1 时原图连通，否则返回的是各连通块的最小生成森林。
// 复杂度：时间复杂度 O(m log m + n)，空间复杂度 O(n + m)。
// ============================================================================

template <typename T>
struct KruskalEdge {
	int u, v;
	T weight;
};

// template <typename T> std::pair<T, std::vector<KruskalEdge<T>>> kruskal(int n, std::vector<KruskalEdge<T>> edges) — 功能：参数与返回值详解：
template <typename T>
std::pair<T, std::vector<KruskalEdge<T>>> kruskal(int n, std::vector<KruskalEdge<T>> edges) {
	std::sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
		return a.weight < b.weight;
	});
	DisjointSetUnion dsu(n);
	T weight = T();
	std::vector<KruskalEdge<T>> result;
	for (const auto& edge : edges)
		if (!dsu.same(edge.u, edge.v)) {
			dsu.merge(edge.u, edge.v);
			weight += edge.weight;
			result.push_back(edge);
		}
	return {weight, result};
}

#endif
