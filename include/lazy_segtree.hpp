#ifndef LAZY_SEGTREE_HPP
#define LAZY_SEGTREE_HPP

#include <cassert>
#include <vector>

#include "bit.hpp"

// ============================================================================
// 【非递归懒标记线段树 Lazy Segment Tree (lazy_segtree.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：
//   template <class V, auto op, auto e, class L, auto mapping, auto composition, auto id>
//   class LazySegmentTree
// 模板参数与代数公理约定：
//   - V, op, e：数据幺半群类型、结合律合并函数 V op(V, V) 与单位元函数 V e()。
//   - L, id：懒标记类型与恒等标记函数 L id()（满足 mapping(id(), x) == x）。
//   - V mapping(L f, V x)：将标记 f 作用在区间聚合值 x 上并返回新值（【注意】不额外传区间长度，若区间加等操作依赖长度，需在 V 内含 len 字段）。
//   - L composition(L f, L g)：标记复合，严格定义为【先作用旧标记 g，再作用新标记 f】，即 mapping(composition(f, g), x) == mapping(f, mapping(g, x))。
// 构造函数与成员函数签名详解（0-based 下标，左闭右开区间 [l, r)）：
//   - LazySegmentTree() / LazySegmentTree(int n) / LazySegmentTree(std::vector<V> v)
//     功能：构造空树、全 e() 树或由初始数组 v 建树。时空复杂度 O(n)。
//   - void set(int p, V x)
//     功能：下推路径标记后将下标 p (0 <= p < n) 赋值为 x 并向上更新。时间复杂度 O(log n)。
//   - V get(int p)
//     功能：下推路径标记后返回下标 p (0 <= p < n) 的当前值。时间复杂度 O(log n)。
//   - V prod(int l, int r)
//     功能：查询半开区间 [l, r) (0 <= l <= r <= n) 的聚合值；空区间返回 e()。时间复杂度 O(log n)。
//   - V all_prod()
//     功能：返回全区间 [0, n) 的聚合值。时间复杂度 O(1)。
//   - void apply(int p, L f)
//     功能：对单点 p (0 <= p < n) 施加标记 f。时间复杂度 O(log n)。
//   - void apply(int l, int r, L f)
//     功能：对半开区间 [l, r) (0 <= l <= r <= n) 整体施加懒标记 f。时间复杂度 O(log n)。
//   - template <class G> int max_right(int l, G g)
//     功能：从左端点 l (0 <= l <= n) 向右树上二分（要求 g(e()) == true），返回使 g(prod(l, r)) 为真的最大 r in [l, n]。O(log n)。
//   - template <class G> int min_left(int r, G g)
//     功能：从右端点 r (0 <= r <= n) 向左树上二分（要求 g(e()) == true），返回使 g(prod(l, r)) 为真的最小 l in [0, r]。O(log n)。
// ============================================================================

template <class V, auto op, auto e, class L, auto mapping, auto composition, auto id>
class LazySegmentTree {
public:
    // LazySegmentTree() — 功能：构造空树、全 e() 树或由初始数组 v 建树。时空复杂度 O(n)。
    LazySegmentTree() : LazySegmentTree(0) {}
    // LazySegmentTree(int n) — 功能：构造空树、全 e() 树或由初始数组 v 建树。时空复杂度 O(n)。
    LazySegmentTree(int n) : LazySegmentTree(std::vector<V>(n, e())) {}
    // LazySegmentTree(std::vector<V> v) — 功能：构造空树、全 e() 树或由初始数组 v 建树。时空复杂度 O(n)。
    LazySegmentTree(std::vector<V> v) : n((int)v.size()) {
        size = ceil_pow2(n);
        log = floor_log2(size);
        value = std::vector<V>(2 * size, e());
        lazy = std::vector<L>(size, id());
        for (int i = 0; i < n; i++) value[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) update(i);
    }

    // void set(int p, V x) — 功能：下推路径标记后将下标 p (0 <= p < n) 赋值为 x 并向上更新。时间复杂度 O(log n)。
    void set(int p, V x) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        value[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    // V get(int p) — 功能：下推路径标记后返回下标 p (0 <= p < n) 的当前值。时间复杂度 O(log n)。
    V get(int p) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return value[p];
    }

    // V prod(int l, int r) — 功能：查询半开区间 [l, r) (0 <= l <= r <= n) 的聚合值；空区间返回 e()。时间复杂度 O(log n)。
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

    // void apply(int p, L f) — 功能：对单点 p (0 <= p < n) 施加标记 f。时间复杂度 O(log n)。
    void apply(int p, L f) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        value[p] = mapping(f, value[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    // void apply(int l, int r, L f) — 功能：对半开区间 [l, r) (0 <= l <= r <= n) 整体施加懒标记 f。时间复杂度 O(log n)。
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

    // template <class G> int max_right(int l, G g) — 功能：从左端点 l (0 <= l <= n) 向右树上二分（要求 g(e()) == true），返回使 g(prod(l, r)) 为真的最大 r in [l, n]。O(log n)。
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

    // template <class G> int min_left(int r, G g) — 功能：从右端点 r (0 <= r <= n) 向左树上二分（要求 g(e()) == true），返回使 g(prod(l, r)) 为真的最小 l in [0, r]。O(log n)。
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
        // 注意：composition(f_new, f_old) 表示先作用旧标记 f_old、再作用新标记 f_new。
        if (k < size) lazy[k] = composition(f, lazy[k]);
    }
    void push(int k) {
        all_apply(2 * k, lazy[k]);
        all_apply(2 * k + 1, lazy[k]);
        lazy[k] = id();
    }
};

#endif
