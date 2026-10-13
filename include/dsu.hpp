#ifndef DSU_HPP
#define DSU_HPP

#include <vector>
#include <algorithm>

class DisjointSetUnion {
	int component;
	std::vector<int> data;

public:
	DisjointSetUnion(int n) : component(n), data(n, 1) {}

	int get(int x) {
		if (data[x] > 0) {
			return x;
		}
		data[x] = -get(-data[x] - 1) - 1;
		return -data[x] - 1;
	}

	void merge(int x, int y) {
		x = get(x), y = get(y);
		if (x == y) return;
		if (data[x] > data[y]) {
			std::swap(x, y);
		}
		data[y] += data[x], data[x] = -(y + 1);
		--component;
	}

	int size(int x) {
		return data[get(x)];
	}

	bool is_root(int x) {
		return data[x] > 0;
	}

	bool same(int x, int y) {
		return get(x) == get(y);
	}

	int component_number() {
		return component;
	}
};

#endif
