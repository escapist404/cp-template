#ifndef SEGTREE_HPP
#define SEGTREE_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

// ACL generic interface; indices and ranges are 0-based, [l, r).

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
