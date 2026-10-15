#ifndef SEGTREE_HPP
#define SEGTREE_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

// 【非递归线段树 (segtree.hpp)】
// - ACL 风格 0-based 左闭右开区间 [l, r) 线段树，建树 O(n)，单点修改/区间查询/树上二分均为 O(log n)
// - 模板参数: SegmentTree<V, op, e>，要求 op(a, b) 满足结合律（无需交换律），e() 返回双边单位元
// - set(p, x) / get(p) : 单点赋值与单点读取 (0 <= p < n)
// - prod(l, r)         : 返回区间 [l, r) 从左到右按 op 聚合的结果（空区间返回 e()）
// - all_prod()         : O(1) 返回全区间 [0, n) 聚合结果
// - max_right(l, f)    : 要求 f(e()) == true 且单调；返回最大的 r in [l, n] 使 f(prod(l, r)) 为真
// - min_left(r, f)     : 要求 f(e()) == true 且单调；返回最小的 l in [0, r] 使 f(prod(l, r)) 为真

template <class V, auto op, auto e>
class SegmentTree {
public:
    SegmentTree() : SegmentTree(0) {}
    SegmentTree(int n) : SegmentTree(std::vector<V>(n, e())) {}
    SegmentTree(std::vector<V> v) : n((int)v.size()) {
        size = ceil_pow2(n);
        log = floor_log2(size);
        value = std::vector<V>(2 * size, e());
        for (int i = 0; i < n; i++) value[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) update(i);
    }

    void set(int p, V x) {
        assert(0 <= p && p < n);
        p += size;
        value[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    V get(int p) {
        assert(0 <= p && p < n);
        return value[p + size];
    }

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
