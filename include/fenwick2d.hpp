#ifndef FENWICK2D_HPP
#define FENWICK2D_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

// T is an additive group: T{} is zero; += and subtraction are required.
template <typename T>
class Fenwick2D {
    int n, m;
    std::vector<std::vector<T>> tree;

public:
    Fenwick2D(int n, int m) : n(n), m(m), tree(n + 1, std::vector<T>(m + 1)) {
        assert(n >= 0 && m >= 0);
    }

    void add(int x, int y, T value) {
        assert(0 <= x && x < n && 0 <= y && y < m);
		for (++x; x <= n; x += lowbit(x)) {
			for (int j = y + 1; j <= m; j += lowbit(j)) {
                tree[x][j] += value;
            }
        }
    }

    // Prefix [0,x) x [0,y).
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

    T get(int l, int d, int r, int u) const {
        assert(0 <= l && l <= r && r <= n && 0 <= d && d <= u && u <= m);
        return get(r, u) - get(l, u) - get(r, d) + get(l, d);
    }
};
#endif
