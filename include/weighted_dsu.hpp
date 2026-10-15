#ifndef WEIGHTED_DSU_HPP
#define WEIGHTED_DSU_HPP

#include <cassert>
#include <numeric>
#include <vector>

// ============================================================================
// 【带权并查集 / 势能并查集 Weighted DSU (weighted_dsu.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T = long long> class WeightedDSU
//   维护节点间的有向阿贝尔群势能差（要求 T 支持 +、-、+=、-=、== 及默认构造零元 T()）。
//   所有节点编号均为 0-based，合法范围 [0, n)。
// 构造函数与成员函数签名详解：
//   - WeightedDSU(int n)
//     功能：初始化 n 个独立元素 [0, n)，每个元素自成一集合，相对根的势能差 weight[x] = 0。时空复杂度 O(n)。
//   - bool merge(int x, int y, T difference)
//     功能：添加有向势能差约束 value[y] - value[x] == difference（【注意方向】是 y 减去 x）。
//     返回：若 x 与 y 原先不连通，则按大小合并并返回 true；若已连通且与现有势能差一致返回 true；
//           若已连通但 difference(x, y) != difference（约束矛盾），则不修改结构并返回 false。均摊 O(alpha(n))。
//   - bool same(int x, int y)
//     功能：判断 x 与 y 是否在同一连通分量内（即 value[y] - value[x] 是否唯一确定）。均摊 O(alpha(n))。
//   - T difference(int x, int y)
//     功能：返回有向势能差 value[y] - value[x]。
//     前置条件：【必须先满足 same(x, y) == true】，否则内部触发 assert(same(x, y))。均摊 O(alpha(n))。
// ============================================================================

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
	// WeightedDSU(int n) — 功能：初始化 n 个独立元素 [0, n)，每个元素自成一集合，相对根的势能差 weight[x] = 0。时空复杂度 O(n)。
	WeightedDSU(int n) : father(n), size(n, 1), weight(n) {
		std::iota(father.begin(), father.end(), 0);
	}

	// bool merge(int x, int y, T difference) — 功能：添加有向势能差约束 value[y] - value[x] == difference（【注意方向】是 y 减去 x）。
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

	// bool same(int x, int y) — 功能：判断 x 与 y 是否在同一连通分量内（即 value[y] - value[x] 是否唯一确定）。均摊 O(alpha(n))。
	bool same(int x, int y) {
		assert(0 <= x && x < (int)father.size());
		assert(0 <= y && y < (int)father.size());
		return find(x) == find(y);
	}

	// T difference(int x, int y) — 功能：返回有向势能差 value[y] - value[x]。
	T difference(int x, int y) {
		assert(same(x, y));
		return weight[y] - weight[x];
	}
};

#endif
