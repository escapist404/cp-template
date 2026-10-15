#ifndef SPARSE_TABLE_HPP
#define SPARSE_TABLE_HPP

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>
#include "bit.hpp"

// 【ST 表与猫树静态区间表 (sparse_table.hpp)】
// - 0-based 左闭右开非空区间 [l, r) 静态查询（要求 0 <= l < r <= n），预处理 O(n log n)，单次查询严格 O(1)
// - SparseTable<T, Op>        : 要求 Op 满足结合律与幂等律（如 max / min / gcd，默认 SparseTableMax 求最大值）
// - DisjointSparseTable<T, Op>: 猫树实现，仅要求 Op 满足结合律（无需幂等律或交换律，支持区间求和、矩阵乘等）

template <typename T>
struct SparseTableMax {
	T operator()(T a, T b) { return std::max(a, b); }
};

// 0-based, nonempty [l, r). Op must be associative and idempotent.
template <typename T, typename Op = SparseTableMax<T>>
class SparseTable {
	int n;
	std::vector<std::vector<T>> table;
	Op op;

public:
	SparseTable(std::vector<T> a, Op op = Op()) : n(a.size()), op(op) {
		if (!n) return;
		table.assign(bit_width(n), a);
		for (int k = 1; k < (int)table.size(); ++k)
			for (int i = 0; i + (1 << k) <= n; ++i)
				table[k][i] = op(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
	}

	T query(int l, int r) {
		assert(0 <= l && l < r && r <= n);
		int k = 31 - __builtin_clz((unsigned)(r - l));
		return op(table[k][l], table[k][r - (1 << k)]);
	}
};

// Any associative operation, including noncommutative ones. Query is O(1).
template <typename T, typename Op = SparseTableMax<T>>
class DisjointSparseTable {
	int n;
	std::vector<T> values;
	std::vector<std::vector<T>> table;
	Op op;

public:
	DisjointSparseTable(std::vector<T> a, Op op = Op())
		: n(a.size()), values(std::move(a)), op(op) {
		table.assign(ceil_log2(n), values);
		for (int k = 0; k < (int)table.size(); ++k) {
			int half = 1 << k;
			for (int l = 0; l < n; l += half * 2) {
				int m = std::min(l + half, n), r = std::min(l + half * 2, n);
				for (int i = m - 2; i >= l; --i)
					table[k][i] = op(values[i], table[k][i + 1]);
				for (int i = m + 1; i < r; ++i)
					table[k][i] = op(table[k][i - 1], values[i]);
			}
		}
	}

	T query(int l, int r) {
		assert(0 <= l && l < r && r <= n);
		if (l == --r) return values[l];
		int k = 31 - __builtin_clz((unsigned)(l ^ r));
		return op(table[k][l], table[k][r]);
	}
};

#endif
