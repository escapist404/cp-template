#ifndef LAZY_SEGTREE_HPP
#define LAZY_SEGTREE_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

// 【懒标记线段树 (lazy_segtree.hpp)】
// - ACL 风格 0-based 左闭右开区间 [l, r) 懒标记线段树，建树 O(n)，修改/查询/树上二分均为 O(log n)
// - 模板参数: LazySegmentTree<V, op, e, L, mapping, composition, id>
//   1. mapping(f, x)    : 将懒标记 f 作用在区间聚合值 x 上（若操作与区间长度相关，需在结构体 V 内维护 len 字段）
//   2. composition(f, g): 标记复合，严格定义为“先作用旧标记 g，再作用新标记 f”，即 mapping(f, mapping(g, x))
//   3. id()             : 返回恒等懒标记，满足 mapping(id(), x) == x
// - apply(p, f) / apply(l, r, f): 单点或区间 [l, r) 施加标记 f

template <class V, auto op, auto e, class L, auto mapping, auto composition, auto id>
class LazySegmentTree {
public:
    LazySegmentTree() : LazySegmentTree(0) {}
    LazySegmentTree(int n) : LazySegmentTree(std::vector<V>(n, e())) {}
    LazySegmentTree(std::vector<V> v) : n((int)v.size()) {
        size = ceil_pow2(n);
        log = floor_log2(size);
        value = std::vector<V>(2 * size, e());
        lazy = std::vector<L>(size, id());
        for (int i = 0; i < n; i++) value[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) update(i);
    }

    void set(int p, V x) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        value[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    V get(int p) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return value[p];
    }

    V prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return e();

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        V sml = e(), smr = e();
        while (l < r) {
            if (l & 1) sml = op(sml, value[l++]);
            if (r & 1) smr = op(value[--r], smr);
            l >>= 1;
            r >>= 1;
        }

        return op(sml, smr);
    }

    V all_prod() { return value[1]; }

    void apply(int p, L f) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        value[p] = mapping(f, value[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    void apply(int l, int r, L f) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return;

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        {
            int l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, f);
                if (r & 1) all_apply(--r, f);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    template <class G> int max_right(int l, G g) {
        assert(0 <= l && l <= n);
        assert(g(e()));
        if (l == n) return n;
        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        V sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(op(sm, value[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (g(op(sm, value[l]))) {
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

    template <class G> int min_left(int r, G g) {
        assert(0 <= r && r <= n);
        assert(g(e()));
        if (r == 0) return 0;
        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        V sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(op(value[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (g(op(value[r], sm))) {
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
    std::vector<L> lazy;

    void update(int k) { value[k] = op(value[2 * k], value[2 * k + 1]); }
    void all_apply(int k, L f) {
        value[k] = mapping(f, value[k]);
        // composition(new, old): apply old first, then new.
        if (k < size) lazy[k] = composition(f, lazy[k]);
    }
    void push(int k) {
        all_apply(2 * k, lazy[k]);
        all_apply(2 * k + 1, lazy[k]);
        lazy[k] = id();
    }
};

#endif
