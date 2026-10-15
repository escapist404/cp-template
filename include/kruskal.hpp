#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <algorithm>
#include <utility>
#include <vector>

#include "dsu.hpp"

// 【Kruskal 最小生成树 (kruskal.hpp)】
// - 0-based 顶点编号 [0, n)，时间复杂度 O(m log m + n)
// - 返回 std::pair{total_weight, mst_edges}：所选树边的总权重与按边权升序排列的边集
// - 若原图不连通则返回最小生成森林；可通过检查 mst_edges.size() == n - 1 判定原图是否连通

template <typename T>
struct KruskalEdge {
	int u, v;
	T weight;
};

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
