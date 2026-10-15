#ifndef FENWICK_HPP
#define FENWICK_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

// 【一维树状数组 (fenwick.hpp)】
// - 0-based 下标，左闭右开区间 [l, r)，构造 O(n)，修改/查询/倍增二分均为 O(log n)
// - add(p, val)     : 单点加 a[p] += val (0 <= p < n)
// - get(p)          : 查询前缀 [0, p) 的和 (0 <= p <= n，get(0) = 0)
// - get(l, r)       : 查询区间 [l, r) 的和 (0 <= l <= r <= n)
// - lower_bound(val): 要求所有元素非负；返回满足前缀和 get(p + 1) >= val 的最小 0-based 下标 p（总和不足返回 n）

template <typename T>
class Fenwick {
	int n;
	std::vector<T> tree;

public:
	Fenwick(int n) : n(n), tree(n + 1, T()) { assert(n >= 0); }
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
