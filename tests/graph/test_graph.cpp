#include "../../include/toposort.hpp"
#include "../../include/zero_one_bfs.hpp"
#include "../../include/kruskal.hpp"

#include <algorithm>
#include <cassert>
#include <vector>
#include "../../include/bipartite_matching.hpp"
#include "../../include/dijkstra.hpp"
#include "../../include/edcc.hpp"
#include "../../include/euler_path.hpp"
#include "../../include/km.hpp"
#include "../../include/max_flow.hpp"
#include "../../include/min_cost_max_flow.hpp"
#include "../../include/prufer.hpp"
#include "../../include/rollback_dsu.hpp"
#include "../../include/scc.hpp"
#include "../../include/tree_hash.hpp"
#include "../../include/two_sat.hpp"
#include "../../include/vdcc.hpp"

int main() {
	const auto distance = dijkstra<long long>({{{1, 3000000000000000000LL}}, {{2, 3000000000000000000LL}}, {}}, {0});
	assert(distance[2] == 6000000000000000000LL);
    const auto matching = bipartite_matching({{0, 1}, {1}}, 2);
    assert(matching.size == 2);
    TwoSat sat(2);
    sat.add_true(0, true);
    sat.add_or(0, false, 1, true);
    assert(sat.solve() && sat.assignment()[1]);
	const std::vector<std::vector<int>> scc_graph{{1}, {0, 2}, {}};
	const auto scc = get_scc(scc_graph);
	assert(scc.size() == 2);
	for (const auto &component : scc) {
		if (component.size() == 2)
			assert((component[0] == 0 && component[1] == 1) ||
				(component[0] == 1 && component[1] == 0));
		else
			assert(component.size() == 1 && component[0] == 2);
	}
	const auto dag = get_dag(scc_graph, scc);
	assert(dag.size() == scc.size());
	const auto edcc = get_edcc({{1}, {0, 2}, {1, 3}, {2}});
	assert(edcc.size() == 4);
	for (int i = 0; i < 4; ++i)
		assert(edcc[i].size() == 1 && edcc[i][0] == i);
	const auto parallel_edcc = get_edcc({{1, 1}, {0, 0}});
	assert(parallel_edcc.size() == 1 && parallel_edcc[0].size() == 2);
	const auto vdcc = get_vdcc({{1}, {0, 2}, {1}});
	assert(vdcc.size() == 2 && vdcc[0].size() == 1 && vdcc[1].size() == 1);
	const auto parallel_vdcc = get_vdcc({{1, 1}, {0, 0}});
	assert(parallel_vdcc.size() == 1 && parallel_vdcc[0].size() == 2);
	const auto isolated_vdcc = get_vdcc({{}});
	assert(isolated_vdcc.size() == 1 && isolated_vdcc[0].empty());
	MaxFlow<int> flow(4);
	flow.add_edge(0, 1, 2);
	flow.add_edge(0, 2, 1);
	flow.add_edge(1, 3, 1);
	flow.add_edge(2, 3, 1);
	assert(flow.flow(0, 3) == 2);
	MaxFlow<int> self_loop(2);
	const int self_id = self_loop.add_edge(0, 0, 5);
	self_loop.add_edge(0, 1, 2);
	assert(self_loop.flow(0, 1) == 2);
	assert(self_loop.get_edge(self_id).cap == 5 && self_loop.get_edge(self_id).flow == 0);
	MinCostMaxFlow<int, int> min_cost_flow(3);
	min_cost_flow.add_edge(0, 1, 2, 1);
	min_cost_flow.add_edge(1, 2, 2, 2);
	assert(min_cost_flow.flow(0, 2) == std::make_pair(2, 6));
    const EulerPath path = euler_path(3, {{0, 1}, {1, 2}, {2, 0}});
    assert(path.exists && path.edges.size() == 3);
    const auto km = kuhn_munkres(std::vector<std::vector<int>>{{3, 1}, {2, 4}});
    assert(km.weight == 7 && km.left[0] == 0 && km.left[1] == 1);
    const auto code = tree_to_prufer({{1}, {0, 2}, {1, 3}, {2}});
    assert((code == std::vector<int>{1, 2}));
    assert(prufer_to_tree(code).size() == 3);
    RollbackDSU dsu(3);
    const int version = dsu.snapshot();
    assert(dsu.merge(0, 1));
    assert(dsu.component_count() == 2);
    dsu.rollback(version);
    assert(dsu.component_count() == 3);
    TreeHash first(4);
    first.add_edge(0, 1);
    first.add_edge(1, 2);
    first.add_edge(1, 3);
    TreeHash second(4);
    second.add_edge(0, 2);
    second.add_edge(2, 1);
    second.add_edge(2, 3);
    assert(first.solve() == second.solve());

    {
	std::vector<std::vector<int>> dag{{1, 2}, {3}, {3}, {}};
	auto order = topological_sort(dag);
	assert(order.size() == 4);
	std::vector<std::vector<int>> cycle{{1}, {0}};
	assert(topological_sort(cycle).empty());

	std::vector<std::vector<std::pair<int, int>>> graph(4);
	graph[0] = {{1, 1}, {2, 0}};
	graph[2] = {{1, 0}, {3, 1}};
	graph[1] = {{3, 1}};
	assert((zero_one_bfs(graph, 0) == std::vector<int>{0, 0, 0, 1}));

    KruskalEdge<int> e1{0, 1, 1}, e2{1, 2, 2}, e3{0, 2, 5};
    auto mst = kruskal(3, std::vector<KruskalEdge<int>>{e3, e2, e1});
    assert(mst.first == 3 && mst.second.size() == 2);


    }
}
