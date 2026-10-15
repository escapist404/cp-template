#ifndef FENWICK2D_HPP
#define FENWICK2D_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

// ============================================================================
// 【二维树状数组 2D Fenwick Tree (fenwick2d.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T> class Fenwick2D
//   维护 n 行 m 列的 0-based 二维网格，所有矩形区域查询均遵循【左闭右开 [l, r) x [d, u)】约定。
// 构造函数与成员函数签名详解：
//   - Fenwick2D(int n, int m)
//     功能：构造 n x m (n >= 0, m >= 0) 全零二维树状数组。时间与空间复杂度 O(n * m)。
//   - void add(int x, int y, T value)
//     功能：单点加操作，将坐标 (x, y) (0 <= x < n, 0 <= y < m) 处的值加上 value。时间复杂度 O(log n log m)。
//   - T get(int x, int y) const
//     功能：查询二维前缀矩形 [0, x) x [0, y) 的元素总和（要求 0 <= x <= n, 0 <= y <= m）。O(log n log m)。
//   - T get(int l, int d, int r, int u) const
//     功能：利用二维容斥原理查询半开子矩形 [l, r) x [d, u) 的元素总和。
//     参数约定：【注意参数顺序】依次为左边界 l、下边界 d、右边界 r、上边界 u（满足 0 <= l <= r <= n 且 0 <= d <= u <= m）。
//     复杂度：时间复杂度 O(log n log m)。
// ============================================================================

template <typename T>
class Fenwick2D {
    int n, m;
    std::vector<std::vector<T>> tree;

public:
    // Fenwick2D(int n, int m) — 功能：构造 n x m (n >= 0, m >= 0) 全零二维树状数组。时间与空间复杂度 O(n * m)。
    Fenwick2D(int n, int m) : n(n), m(m), tree(n + 1, std::vector<T>(m + 1)) {
        assert(n >= 0 && m >= 0);
    }

    // void add(int x, int y, T value) — 功能：单点加操作，将坐标 (x, y) (0 <= x < n, 0 <= y < m) 处的值加上 value。时间复杂度 O(log n log m)。
    void add(int x, int y, T value) {
        assert(0 <= x && x < n && 0 <= y && y < m);
		for (++x; x <= n; x += lowbit(x)) {
			for (int j = y + 1; j <= m; j += lowbit(j)) {
                tree[x][j] += value;
            }
        }
    }

    // 查询二维前缀 [0, x) x [0, y) 的和
    T get(int x, int y) const {
        assert(0 <= x && x <= n && 0 <= y && y <= m);
        T result{};
		for (; x > 0; x -= lowbit(x)) {
			for (int j = y; j > 0; j -= lowbit(j)) {
                result += tree[x][j];
            }
        }
        return result;
    }

    // T get(int l, int d, int r, int u) const — 功能：利用二维容斥原理查询半开子矩形 [l, r) x [d, u) 的元素总和。
    T get(int l, int d, int r, int u) const {
        assert(0 <= l && l <= r && r <= n && 0 <= d && d <= u && u <= m);
        return get(r, u) - get(l, u) - get(r, d) + get(l, d);
    }
};
#endif
