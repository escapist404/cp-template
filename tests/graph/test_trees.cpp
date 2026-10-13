#include <cassert>
#include <set>
#include <vector>
#include "../../include/tree_diameter.hpp"
#include "../../include/centroid_decomposition.hpp"
#include "../../include/virtual_tree.hpp"
#include "../../include/link_cut_tree.hpp"
#include "../../include/binary_lifting_lca.hpp"
#include "../../include/hld.hpp"
#include "../../include/tarjan_lca.hpp"
#include "../../include/dsu_on_tree.hpp"

int main() {
    {
	std::vector<std::vector<int>> tree(6);
	auto edge = [&](int u, int v) { tree[u].push_back(v), tree[v].push_back(u); };
	edge(0, 1), edge(0, 2), edge(1, 3), edge(1, 4), edge(2, 5);
	auto diameter = tree_diameter(tree);
	assert(diameter.first == 4);
	auto diameter_ends = std::set<int>{diameter.second.first, diameter.second.second};
	assert(diameter_ends.size() == 2);
	assert(diameter_ends.count(3) + diameter_ends.count(4) + diameter_ends.count(5) == 2);

	CentroidDecomposition decomposition(tree);
	int centroid_roots = 0;
	for (int i = 0; i < 6; ++i)
		centroid_roots += decomposition.parent_of(i) == -1;
	assert(centroid_roots == 1);

	VirtualTreeBuilder builder(tree);
	auto virtual_tree = builder.build({3, 4, 5});
	assert(virtual_tree.vertices.size() == 5);
	assert(virtual_tree.edges.size() == 4);

	LinkCutTree<int> dynamic_tree(3);
	dynamic_tree.set_value(1, 1);
	dynamic_tree.set_value(2, 2);
	dynamic_tree.set_value(3, 3);
	assert(dynamic_tree.link(1, 2));
	assert(dynamic_tree.link(2, 3));
	assert(dynamic_tree.connected(1, 3));
	assert(dynamic_tree.path_query(1, 3) == 6);
	dynamic_tree.set_value(2, 5);
	assert(dynamic_tree.path_query(1, 3) == 9);
	assert(dynamic_tree.cut(2, 3));
	assert(!dynamic_tree.connected(1, 3));
	assert(dynamic_tree.link(1, 3));
	assert(dynamic_tree.path_query(2, 3) == 9);    }
    {
    BinaryLiftingLCA lca(5);
    lca.add_edge(0, 1), lca.add_edge(0, 2), lca.add_edge(1, 3), lca.add_edge(1, 4);
    lca.prework(0);
    assert(lca.lca(3, 4) == 1 && lca.lca(2, 4) == 0);

    HeavyLightDecomposition hld(5);
    hld.add_edge(0, 1), hld.add_edge(0, 2), hld.add_edge(1, 3), hld.add_edge(1, 4);
    hld.prework(0);
    assert(hld.lca(3, 4) == 1 && hld.parent(1) == 0);
    auto range = hld.subtree_range(1);
    assert(range.second - range.first == 3);

    std::vector<std::vector<int>> tree(5);
    tree[0] = {1, 2}; tree[1] = {0, 3, 4}; tree[2] = {0};
    tree[3] = {1}; tree[4] = {1};
    auto offline = tarjan_lca(tree, 0, {{3, 4}, {2, 4}});
    assert(offline == std::vector<int>({1, 0}));

    int active = 0;
    DSUOnTree dsu_tree(5);
    dsu_tree.add_edge(0, 1), dsu_tree.add_edge(0, 2), dsu_tree.add_edge(1, 3), dsu_tree.add_edge(1, 4);
    auto counts = dsu_tree.solve(0, active,
        [](int, int& state) { ++state; },
        [](int, int& state) { --state; },
        [](int state, int) { return state; });
    assert(counts[0] == 5 && counts[1] == 3 && counts[2] == 1);

    }
}
