#ifndef KM_HPP
#define KM_HPP

#include <cassert>
#include <limits>
#include <utility>
#include <vector>

// Rectangular complete weighted bipartite graph, left <= right.
// Hungarian primal-dual implementation of maximum-weight matching.
template <typename T>
struct KuhnMunkresResult {
	T weight{};
	std::vector<int> left, right;
};

// T must be a signed arithmetic ordered type with finite, nonoverflowing sums.
template <typename T>
KuhnMunkresResult<T> kuhn_munkres(const std::vector<std::vector<T>>& weight) {
	const int n = weight.size();
	const int m = n ? weight[0].size() : 0;
	assert(n <= m);
	for (const auto& row : weight) assert((int)row.size() == m);
	const T infinity = std::numeric_limits<T>::max() / 4;
	std::vector<T> u(n + 1), v(m + 1);
	std::vector<int> match(m + 1), previous(m + 1);
	for (int i = 1; i <= n; ++i) {
		match[0] = i;
		int y = 0;
		std::vector<T> slack(m + 1, infinity);
		std::vector<char> used(m + 1);
		do {
			used[y] = true;
			const int x = match[y];
			int next = 0;
			T delta = infinity;
			for (int j = 1; j <= m; ++j) if (!used[j]) {
				const T cost = -weight[x - 1][j - 1] - u[x] - v[j];
				if (cost < slack[j]) {
					slack[j] = cost;
					previous[j] = y;
				}
				if (slack[j] < delta) {
					delta = slack[j];
					next = j;
				}
			}
			for (int j = 0; j <= m; ++j) {
				if (used[j]) {
					u[match[j]] += delta;
					v[j] -= delta;
				} else {
					slack[j] -= delta;
				}
			}
			y = next;
		} while (match[y]);
		do {
			const int next = previous[y];
			match[y] = match[next];
			y = next;
		} while (y);
	}
	KuhnMunkresResult<T> result;
	result.left.assign(n, -1);
	result.right.assign(m, -1);
	for (int j = 1; j <= m; ++j) if (match[j]) {
		const int x = match[j] - 1;
		result.left[x] = j - 1;
		result.right[j - 1] = x;
		result.weight += weight[x][j - 1];
	}
	return result;
}

#endif
