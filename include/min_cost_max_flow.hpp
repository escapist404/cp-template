#ifndef MIN_COST_MAX_FLOW_HPP
#define MIN_COST_MAX_FLOW_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <utility>
#include <vector>

template <typename Cap, typename Cost>
struct MinCostMaxFlow {
private:
	template <typename E>
	struct Csr {
		std::vector<int> start;
		std::vector<E> list;

		Csr(int n, const std::vector<std::pair<int, E>>& edges)
			: start(n + 1), list(edges.size()) {
			for (const auto& edge : edges) ++start[edge.first + 1];
			for (int i = 1; i <= n; ++i) start[i] += start[i - 1];
			auto counter = start;
			for (const auto& edge : edges) list[counter[edge.first]++] = edge.second;
		}
	};

public:
	MinCostMaxFlow() : n(0) {}
	MinCostMaxFlow(int n) : n(n) {}

	int add_edge(int from, int to, Cap cap, Cost cost) {
		assert(0 <= from && from < n);
		assert(0 <= to && to < n);
		assert(0 <= cap && 0 <= cost);
		const int id = edges_data.size();
		edges_data.push_back({from, to, cap, 0, cost});
		return id;
	}

	struct edge {
		int from, to;
		Cap cap, flow;
		Cost cost;
	};

	edge get_edge(int i) const {
		assert(0 <= i && i < (int)edges_data.size());
		return edges_data[i];
	}

	std::vector<edge> edges() const { return edges_data; }

	std::pair<Cap, Cost> flow(int s, int t) {
		return flow(s, t, std::numeric_limits<Cap>::max());
	}

	std::pair<Cap, Cost> flow(int s, int t, Cap flow_limit) {
		return slope(s, t, flow_limit).back();
	}

	std::vector<std::pair<Cap, Cost>> slope(int s, int t) {
		return slope(s, t, std::numeric_limits<Cap>::max());
	}

	std::vector<std::pair<Cap, Cost>> slope(int s, int t, Cap flow_limit) {
		assert(0 <= s && s < n && 0 <= t && t < n && s != t);
		const int m = edges_data.size();
		std::vector<int> edge_index(m);
		auto graph = [&]() {
			std::vector<int> degree(n), reverse_index(m);
			std::vector<std::pair<int, internal_edge>> edges;
			edges.reserve(2 * m);
			for (int i = 0; i < m; ++i) {
				const auto edge = edges_data[i];
				edge_index[i] = degree[edge.from]++;
				reverse_index[i] = degree[edge.to]++;
				edges.push_back({edge.from, {edge.to, -1, edge.cap - edge.flow, edge.cost}});
				edges.push_back({edge.to, {edge.from, -1, edge.flow, -edge.cost}});
			}
			auto result = Csr<internal_edge>(n, edges);
			for (int i = 0; i < m; ++i) {
				const auto edge = edges_data[i];
				edge_index[i] += result.start[edge.from];
				reverse_index[i] += result.start[edge.to];
				result.list[edge_index[i]].rev = reverse_index[i];
				result.list[reverse_index[i]].rev = edge_index[i];
			}
			return result;
		}();
		auto result = slope_impl(graph, s, t, flow_limit);
		for (int i = 0; i < m; ++i) {
			const auto& edge = graph.list[edge_index[i]];
			edges_data[i].flow = edges_data[i].cap - edge.cap;
		}
		return result;
	}

private:
	struct internal_edge {
		int to, rev;
		Cap cap;
		Cost cost;
	};
	int n;
	std::vector<edge> edges_data;

	std::vector<std::pair<Cap, Cost>> slope_impl(
			Csr<internal_edge>& graph, int s, int t, Cap flow_limit) {
		std::vector<std::pair<Cost, Cost>> dual_dist(n);
		std::vector<int> previous(n);
		std::vector<bool> visited(n);
		struct QueueItem {
			Cost key;
			int vertex;
			bool operator<(const QueueItem& other) const { return key > other.key; }
		};
		std::vector<int> zero_queue;
		std::vector<QueueItem> heap;
		auto dual_ref = [&]() {
			for (auto& value : dual_dist) value.second = std::numeric_limits<Cost>::max();
			std::fill(visited.begin(), visited.end(), false);
			zero_queue.clear();
			heap.clear();
			size_t heap_size = 0;
			dual_dist[s].second = 0;
			zero_queue.push_back(s);
			while (!zero_queue.empty() || !heap.empty()) {
				int v;
				if (!zero_queue.empty()) {
					v = zero_queue.back();
					zero_queue.pop_back();
				} else {
					while (heap_size < heap.size()) {
						++heap_size;
						std::push_heap(heap.begin(), heap.begin() + heap_size);
					}
					v = heap.front().vertex;
					std::pop_heap(heap.begin(), heap.end());
					heap.pop_back();
					--heap_size;
				}
				if (visited[v]) continue;
				visited[v] = true;
				if (v == t) break;
				const Cost dual_v = dual_dist[v].first;
				const Cost distance_v = dual_dist[v].second;
				for (int i = graph.start[v]; i < graph.start[v + 1]; ++i) {
					const auto& edge = graph.list[i];
					if (!edge.cap) continue;
					const Cost cost = edge.cost - dual_dist[edge.to].first + dual_v;
					if (dual_dist[edge.to].second - distance_v > cost) {
						const Cost distance_to = distance_v + cost;
						dual_dist[edge.to].second = distance_to;
						previous[edge.to] = edge.rev;
						if (distance_to == distance_v) zero_queue.push_back(edge.to);
						else heap.push_back({distance_to, edge.to});
					}
				}
			}
			if (!visited[t]) return false;
			for (int v = 0; v < n; ++v) if (visited[v])
				dual_dist[v].first -= dual_dist[t].second - dual_dist[v].second;
			return true;
		};
		Cap flow = 0;
		Cost cost = 0, previous_cost = -1;
		std::vector<std::pair<Cap, Cost>> result{{0, 0}};
		while (flow < flow_limit) {
			if (!dual_ref()) break;
			Cap pushed = flow_limit - flow;
			for (int v = t; v != s; v = graph.list[previous[v]].to)
				pushed = std::min(pushed, graph.list[graph.list[previous[v]].rev].cap);
			for (int v = t; v != s; v = graph.list[previous[v]].to) {
				auto& edge = graph.list[previous[v]];
				edge.cap += pushed;
				graph.list[edge.rev].cap -= pushed;
			}
			const Cost distance = -dual_dist[s].first;
			flow += pushed;
			cost += pushed * distance;
			if (previous_cost == distance) result.pop_back();
			result.push_back({flow, cost});
			previous_cost = distance;
		}
		return result;
	}
};

#endif
