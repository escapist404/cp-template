#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <cassert>
#include <functional>
#include <initializer_list>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

template <typename T>
std::vector<T> dijkstra(
		const std::vector<std::vector<std::pair<int, T>>>& adj,
		const std::initializer_list<int> sources) {

	const T inf = std::numeric_limits<T>::max();
	std::vector<T> distance(adj.size(), inf);

	using State = std::pair<T, int>;
	std::priority_queue<
		State,
		std::vector<State>,
		std::greater<State>> queue;

	for (const int source : sources) {
		assert(0 <= source && source < (int)(adj.size()));
		distance[source] = T();
		queue.emplace(T(), source);
	}

	while (!queue.empty()) {
		const auto [current_distance, from] = queue.top();
		queue.pop();

		if (current_distance != distance[from]) {
			continue;
		}

		for (const auto& [to, weight] : adj[from]) {
			// Dijkstra requires non-negative edge weights.  Keep `inf` as the
			// actual type maximum and skip only additions that would overflow.
			if (weight > T() && current_distance > inf - weight) {
				continue;
			}
			const T candidate = current_distance + weight;
			if (candidate >= distance[to]) {
				continue;
			}

			distance[to] = candidate;
			queue.emplace(candidate, to);
		}
	}

	return distance;
}

#endif
