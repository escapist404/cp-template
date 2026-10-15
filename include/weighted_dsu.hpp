#ifndef WEIGHTED_DSU_HPP
#define WEIGHTED_DSU_HPP

#include <cassert>
#include <numeric>
#include <vector>

// 【带权并查集 / 势能并查集 (weighted_dsu.hpp)】
// - 0-based 下标 [0, n)，维护节点间有向势能差，均摊复杂度 O(alpha(n))
// - merge(x, y, d) : 添加约束 value[y] - value[x] == d（注意方向为 y 减 x）；若与已有约束矛盾则不修改并返回 false
// - same(x, y)     : 判断 x 与 y 的势能差是否已唯一确定
// - difference(x,y): 返回 value[y] - value[x]（调用前必须保证 same(x, y) 为 true，否则触发 assert）

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
