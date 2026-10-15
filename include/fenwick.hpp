#ifndef FENWICK_HPP
#define FENWICK_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

// ============================================================================
// 【一维树状数组 Fenwick Tree / Binary Indexed Tree (fenwick.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T> class Fenwick
//   维护长度为 n 的 0-based 数组，所有区间查询均遵循【左闭右开 [l, r)】约定。
// 构造函数与成员函数签名详解：
//   - Fenwick(int n)
//     功能：构造长度为 n (n >= 0)、初始全为 T() 的树状数组。时空复杂度 O(n)。
//   - Fenwick(std::vector<T> init)
//     功能：使用初始数组 init 在严格线性 O(n) 时间内直接建树（非 O(n log n) 逐个 add）。
//   - void add(int p, T value)
//     功能：单点加操作，将 0-based 下标 p (0 <= p < n) 处的元素加上 value。时间复杂度 O(log n)。
//   - T get(int p)
//     功能：查询前缀半开区间 [0, p) 的元素和（要求 0 <= p <= n；p == 0 时返回 T()）。时间复杂度 O(log n)。
//   - T get(int l, int r)
//     功能：查询子区间 [l, r) 的元素和，即 get(r) - get(l)（要求 0 <= l <= r <= n；l == r 返回 T()）。O(log n)。
//   - int lower_bound(T value)
//     功能：树上倍增二分，返回满足前缀和 get(p + 1) >= value 的最小 0-based 下标 p。
//     前置条件：原数组中所有元素必须非负（保证前缀和单调不降）。
//     边界返回：若 value <= T() 返回 0；若全部元素总和 get(n) < value，则返回 n。时间复杂度 O(log n)。
// ============================================================================

template <typename T>
class Fenwick {
	int n;
	std::vector<T> tree;

public:
	Fenwick(int n) : n(n), tree(n + 1, T()) { assert(n >= 0); }
	// Fenwick(int n) — 功能：构造长度为 n (n >= 0)、初始全为 T() 的树状数组。时空复杂度 O(n)。
	Fenwick(std::vector<T> init) : Fenwick((int)init.size()) {
		for (int i = 0; i < n; ++i) add(i, init[i]);
	}

	// 单点加：位置 p 加上 value
	void add(int p, T value) {
		assert(0 <= p && p < n);
		for (++p; p <= n; p += lowbit(p)) tree[p] += value;
	}

	// 查询前缀 [0, p) 的和
	T get(int p) {
		assert(0 <= p && p <= n);
		T result = T();
		for (; p; p -= lowbit(p)) result += tree[p];
		return result;
	}

	// 查询区间 [l, r) 的和
	T get(int l, int r) {
		assert(0 <= l && l <= r && r <= n);
		return get(r) - get(l);
	}

	// 返回满足前缀和 get(p + 1) >= value 的最小下标 p（不存在返回 n）
	int lower_bound(T value) {
		if (n == 0 || value <= T()) return 0;
		int p = 0;
		T sum = T();
		for (int step = highbit(n); step; step >>= 1) {
			if (p + step <= n && sum + tree[p + step] < value) {
				p += step;
				sum += tree[p];
			}
		}
		return p == n ? n : p;
	}
};

#endif
