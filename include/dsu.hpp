#ifndef DSU_HPP
#define DSU_HPP

#include <vector>
#include <algorithm>

// 【并查集 (dsu.hpp)】
// - 0-based 下标 [0, n)，同时采用路径压缩与按大小合并，单次操作均摊复杂度 O(alpha(n))
// - get(x)            : 返回 x 所在集合的代表元（根节点）
// - merge(x, y)       : 合并 x 与 y 所在集合（较小集合挂到较大集合下）
// - same(x, y)        : 判断 x 与 y 是否连通
// - size(x)           : 返回 x 所在连通分量的大小
// - is_root(x)        : 判断 x 是否为集合根节点
// - component_number(): 返回当前连通分量总数

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
