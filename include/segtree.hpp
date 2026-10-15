#ifndef SEGTREE_HPP
#define SEGTREE_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

// ============================================================================
// 【非递归线段树 Segment Tree (segtree.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <class V, auto op, auto e> class SegmentTree
//   ACL 风格非递归（ZKW）线段树，维护 0-based 半开区间 [0, n) 上的幺半群（Monoid）信息：
//   - V：节点聚合值类型；
//   - op：二元合并函数 V op(V a, V b)，必须满足结合律 op(op(a, b), c) == op(a, op(b, c))（无需交换律）；
//   - e：单位元函数 V e()，满足 op(e(), x) == x 且 op(x, e()) == x。
// 构造函数与成员函数签名详解：
//   - SegmentTree() / SegmentTree(int n)
//     功能：构造长度为 0 或长度为 n（初始全为 e()）的线段树。时空复杂度 O(n)。
//   - SegmentTree(std::vector<V> v)
//     功能：以数组 v 初始化底层叶子并自底向上 O(n) 建树。
//   - void set(int p, V x)
//     功能：将 0-based 下标 p (0 <= p < n) 处的元素直接赋值为 x，并更新祖先节点。时间复杂度 O(log n)。
//   - V get(int p)
//     功能：读取下标 p (0 <= p < n) 处的当前单点值。时间复杂度 O(1)。
//   - V prod(int l, int r)
//     功能：查询左闭右开区间 [l, r) (0 <= l <= r <= n) 按从左到右顺序聚合的值 op(v[l], ..., v[r-1])；l == r 时返回 e()。O(log n)。
//   - V all_prod()
//     功能：返回整个区间 [0, n) 的聚合结果（n == 0 时返回 e()）。时间复杂度 O(1)。
//   - template <class F> int max_right(int l, F f)
//     功能：树上二分，传入左端点 l (0 <= l <= n) 与单调判定函数 bool f(V)（要求 f(e()) == true），
//           返回满足 f(prod(l, r)) == true 的最大右边界 r in [l, n]。时间复杂度 O(log n)。
//   - template <class F> int min_left(int r, F f)
//     功能：树上二分，传入右端点 r (0 <= r <= n) 与单调判定函数 bool f(V)（要求 f(e()) == true），
//           返回满足 f(prod(l, r)) == true 的最小左边界 l in [0, r]。时间复杂度 O(log n)。
// ============================================================================

template <class V, auto op, auto e>
class SegmentTree {
public:
    // SegmentTree() — 功能：构造长度为 0 或长度为 n（初始全为 e()）的线段树。时空复杂度 O(n)。
    SegmentTree() : SegmentTree(0) {}
    // SegmentTree(int n) — 功能：构造长度为 0 或长度为 n（初始全为 e()）的线段树。时空复杂度 O(n)。
    SegmentTree(int n) : SegmentTree(std::vector<V>(n, e())) {}
    // SegmentTree(std::vector<V> v) — 功能：以数组 v 初始化底层叶子并自底向上 O(n) 建树。
    SegmentTree(std::vector<V> v) : n((int)v.size()) {
        size = ceil_pow2(n);
        log = floor_log2(size);
        value = std::vector<V>(2 * size, e());
        for (int i = 0; i < n; i++) value[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) update(i);
    }

    // void set(int p, V x) — 功能：将 0-based 下标 p (0 <= p < n) 处的元素直接赋值为 x，并更新祖先节点。时间复杂度 O(log n)。
    void set(int p, V x) {
        assert(0 <= p && p < n);
        p += size;
        value[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    // V get(int p) — 功能：读取下标 p (0 <= p < n) 处的当前单点值。时间复杂度 O(1)。
    V get(int p) {
        assert(0 <= p && p < n);
        return value[p + size];
    }

    // V prod(int l, int r) — 功能：查询左闭右开区间 [l, r) (0 <= l <= r <= n) 按从左到右顺序聚合的值 op(v[l], ..., v[r-1])；l == r 时返回 e()。O(log n)。
    V prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        V sml = e(), smr = e();
        l += size;
        r += size;

        while (l < r) {
            if (l & 1) sml = op(sml, value[l++]);
            if (r & 1) smr = op(value[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }

    V all_prod() { return value[1]; }

    // template <class F> int max_right(int l, F f) — 功能：树上二分，传入左端点 l (0 <= l <= n) 与单调判定函数 bool f(V)（要求 f(e()) == true），
    template <class F> int max_right(int l, F f) {
        assert(0 <= l && l <= n);
        assert(f(e()));
        if (l == n) return n;
        l += size;
        V sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, value[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, value[l]))) {
                        sm = op(sm, value[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, value[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    // template <class F> int min_left(int r, F f) — 功能：树上二分，传入右端点 r (0 <= r <= n) 与单调判定函数 bool f(V)（要求 f(e()) == true），
    template <class F> int min_left(int r, F f) {
        assert(0 <= r && r <= n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        V sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(value[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(value[r], sm))) {
                        sm = op(value[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(value[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

private:
    int n, size, log;
    std::vector<V> value;

    void update(int k) { value[k] = op(value[2 * k], value[2 * k + 1]); }
};

#endif
