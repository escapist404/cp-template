#ifndef FENWICK_HPP
#define FENWICK_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

template <typename T>
class Fenwick {
	int n;
	std::vector<T> tree;

public:
	Fenwick(int n) : n(n), tree(n + 1, T()) { assert(n >= 0); }
	Fenwick(std::vector<T> init) : Fenwick((int)init.size()) {
		for (int i = 0; i < n; ++i) add(i, init[i]);
	}

	// Add value at position p
	void add(int p, T value) {
		assert(0 <= p && p < n);
		for (++p; p <= n; p += lowbit(p)) tree[p] += value;
	}

	// Sum on [0, p)
	T get(int p) {
		assert(0 <= p && p <= n);
		T result = T();
		for (; p; p -= lowbit(p)) result += tree[p];
		return result;
	}

	// Sum on [l, r)
	T get(int l, int r) {
		assert(0 <= l && l <= r && r <= n);
		return get(r) - get(l);
	}

	// First p with get(p + 1) >= value; n if none
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
