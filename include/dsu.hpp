#ifndef DSU_HPP
#define DSU_HPP

#include <vector>
#include <algorithm>

// ============================================================================
// 【并查集 Disjoint Set Union (dsu.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class DisjointSetUnion
//   同时采用路径压缩与按集合大小合并（启发式合并），所有元素下标均为 0-based，合法范围 [0, n)。
// 构造函数与成员函数签名详解：
//   - DisjointSetUnion(int n)
//     功能：初始化包含 n 个独立元素 [0, n) 的并查集，初始连通块数为 n，每个集合大小为 1。
//     复杂度：时间与空间均为 O(n)。
//   - int get(int x)
//     功能：查找元素 x (0 <= x < n) 所在连通分量的代表元（根节点），并沿途执行路径压缩。
//     返回：根节点编号 in [0, n)。均摊时间复杂度 O(alpha(n))。
//   - void merge(int x, int y)
//     功能：合并元素 x 与 y 所在的集合；若已在同一集合则无操作，否则将较小集合的根挂到较大集合的根下，连通块数减 1。
//     复杂度：均摊 O(alpha(n))。
//   - int size(int x)
//     功能：返回元素 x 所在连通分量的元素个数。均摊时间复杂度 O(alpha(n))。
//   - bool is_root(int x)
//     功能：判断元素 x 当前是否为其所在集合的根节点（即内部编码 data[x] > 0）。严格 O(1)。
//   - bool same(int x, int y)
//     功能：判断元素 x 与 y 是否处于同一连通分量（即 get(x) == get(y)）。均摊 O(alpha(n))。
//   - int component_number()
//     功能：返回当前剩余的连通分量总数。严格 O(1)。
// ============================================================================

class DisjointSetUnion {
	int component;
	std::vector<int> data;

public:
	// DisjointSetUnion(int n) — 功能：初始化包含 n 个独立元素 [0, n) 的并查集，初始连通块数为 n，每个集合大小为 1。
	DisjointSetUnion(int n) : component(n), data(n, 1) {}

	// int get(int x) — 功能：查找元素 x (0 <= x < n) 所在连通分量的代表元（根节点），并沿途执行路径压缩。
	int get(int x) {
		if (data[x] > 0) {
			return x;
		}
		data[x] = -get(-data[x] - 1) - 1;
		return -data[x] - 1;
	}

	// void merge(int x, int y) — 功能：合并元素 x 与 y 所在的集合；若已在同一集合则无操作，否则将较小集合的根挂到较大集合的根下，连通块数减 1。
	void merge(int x, int y) {
		x = get(x), y = get(y);
		if (x == y) return;
		if (data[x] > data[y]) {
			std::swap(x, y);
		}
		data[y] += data[x], data[x] = -(y + 1);
		--component;
	}

	// int size(int x) — 功能：返回元素 x 所在连通分量的元素个数。均摊时间复杂度 O(alpha(n))。
	int size(int x) {
		return data[get(x)];
	}

	// bool is_root(int x) — 功能：判断元素 x 当前是否为其所在集合的根节点（即内部编码 data[x] > 0）。严格 O(1)。
	bool is_root(int x) {
		return data[x] > 0;
	}

	// bool same(int x, int y) — 功能：判断元素 x 与 y 是否处于同一连通分量（即 get(x) == get(y)）。均摊 O(alpha(n))。
	bool same(int x, int y) {
		return get(x) == get(y);
	}

	// int component_number() — 功能：返回当前剩余的连通分量总数。严格 O(1)。
	int component_number() {
		return component;
	}
};

#endif
