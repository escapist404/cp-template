#ifndef TWO_SAT_HPP
#define TWO_SAT_HPP

#include <algorithm>
#include <cassert>
#include <vector>

#include "scc.hpp"

class TwoSat {
	int n;
	std::vector<std::vector<int>> graph;
	std::vector<int> component, answer;

public:
	TwoSat(int n) : n(n), graph(2 * n) {}

	// (x == xv) implies (y == yv), including its contrapositive.
	void add_implication(int x, bool xv, int y, bool yv) {
		assert(0 <= x && x < n && 0 <= y && y < n);
		const int a = 2 * x + xv, b = 2 * y + yv;
		graph[a].push_back(b);
		graph[b ^ 1].push_back(a ^ 1);
	}
	void add_or(int x, bool xv, int y, bool yv) {
		add_implication(x, !xv, y, yv);
	}
	void add_true(int x, bool value) {
		add_implication(x, !value, x, value);
	}

	bool solve() {
		const auto components = get_scc(graph);
		component.assign(2 * n, 0);
		for (int id = 0; id < (int)components.size(); ++id)
			for (const int v : components[id]) component[v] = id;
		answer.assign(n, false);
		for (int i = 0; i < n; ++i) {
			if (component[2 * i] == component[2 * i + 1]) {
				return false;
			}
			answer[i] = component[2 * i] > component[2 * i + 1];
		}
		return true;
	}
	// Valid only after solve() returns true.
	const std::vector<int>& assignment() const {
		return answer;
	}
};

#endif
