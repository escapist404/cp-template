#ifndef SPARSE_TABLE_HPP
#define SPARSE_TABLE_HPP

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>
#include "bit.hpp"

// ============================================================================
// 【ST 表与猫树静态区间查询表 Sparse Table & Disjoint Sparse Table (sparse_table.hpp)】
// ----------------------------------------------------------------------------
// 1. 默认算子结构体签名：
//    - template <typename T> struct SparseTableMax { T operator()(T a, T b); };
//      返回 std::max(a, b)，作为 SparseTable 与 DisjointSparseTable 的默认合并算子。
// 2. 经典 ST 表类签名：template <typename T, typename Op = SparseTableMax<T>> class SparseTable
//    要求二元运算 Op 同时满足【结合律与幂等律】（如 max, min, gcd, 按位与/或）。
//    - SparseTable(std::vector<T> a, Op op = Op())
//      功能：预处理长度为 n 的静态数组 a。时间与空间复杂度均为 O(n log n)。
//    - T query(int l, int r)
//      功能：查询 0-based 左闭右开【非空】区间 [l, r)（要求 0 <= l < r <= n，空区间触发 assert）。
//      返回：op(a[l], ..., a[r - 1])，单次查询严格 O(1)。
// 3. 猫树（不相交稀疏表）类签名：template <typename T, typename Op = SparseTableMax<T>> class DisjointSparseTable
//    仅要求二元运算 Op 满足【结合律】（无需幂等律或交换律，可用于区间求和、模乘、矩阵乘法、线性函数复合等）。
//    - DisjointSparseTable(std::vector<T> a, Op op = Op())
//      功能：分治预处理中心前后缀聚合表。时间与空间复杂度 O(n log n)。
//    - T query(int l, int r)
//      功能：查询 0-based 左闭右开【非空】区间 [l, r)（要求 0 <= l < r <= n）。单次查询严格 O(1)。
// ============================================================================

template <typename T>
struct SparseTableMax {
	T operator()(T a, T b) { return std::max(a, b); }
};

// 稀疏表区间查询 [l, r)（0-based 非空半开区间）：要求 Op 满足结合律与幂等律（O(1) 查询）。
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

// 结合律运算（无需幂等律，如矩阵乘法/模乘）的 Disjoint Sparse Table，支持 O(1) 区间查询 [l, r)。
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
