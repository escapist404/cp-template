#ifndef WEIGHTED_DSU_HPP
#define WEIGHTED_DSU_HPP

#include <cassert>
#include <numeric>
#include <vector>

// merge(x, y, w) enforces value[y] - value[x] = w.
template <typename T = long long>
class WeightedDSU {
	std::vector<int> father, size;
	std::vector<T> weight;

	int find(int x) {
		if (father[x] == x) return x;
		int parent = father[x];
		father[x] = find(parent);
		weight[x] += weight[parent];
		return father[x];
	}

public:
	WeightedDSU(int n) : father(n), size(n, 1), weight(n) {
		std::iota(father.begin(), father.end(), 0);
	}

	bool merge(int x, int y, T difference) {
		assert(0 <= x && x < (int)father.size());
		assert(0 <= y && y < (int)father.size());
		int rx = find(x), ry = find(y);
		T wx = weight[x], wy = weight[y];
		if (rx == ry) return wy - wx == difference;
		if (size[rx] < size[ry]) {
			father[rx] = ry;
			weight[rx] = wy - wx - difference;
			size[ry] += size[rx];
		} else {
			father[ry] = rx;
			weight[ry] = difference + wx - wy;
			size[rx] += size[ry];
		}
		return true;
	}

	bool same(int x, int y) {
		assert(0 <= x && x < (int)father.size());
		assert(0 <= y && y < (int)father.size());
		return find(x) == find(y);
	}

	T difference(int x, int y) {
		assert(same(x, y));
		return weight[y] - weight[x];
	}
};

#endif
