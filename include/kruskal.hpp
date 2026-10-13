#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <algorithm>
#include <utility>
#include <vector>

#include "dsu.hpp"

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
