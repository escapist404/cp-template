#ifndef DYNAMIC_SEGTREE_HPP
#define DYNAMIC_SEGTREE_HPP

#include <cassert>
#include <utility>
#include <vector>
#include "binary_search.hpp"

// ============================================================================
// 【动态开点线段树与线段树合并 Dynamic Segment Tree (dynamic_segtree.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：
//   template <class V, auto op, auto e, class Index = long long> class DynamicSegmentTree
//   在共享节点池上维护定义域均为左闭右开 [low, high) 的多棵动态开点线段树。
//   每棵树由一个整型根句柄 int root 标识（root == 0 代表全为 e() 的空树）。
// 构造函数与成员函数签名详解：
//   - DynamicSegmentTree(Index low, Index high)
//     功能：指定全局左闭右开坐标域 [low, high)（要求 low <= high）并初始化 0 号空节点。O(1)。
//   - void set(int& root, Index p, V value)
//     功能：在以 root 为根的树中将位置 p (low <= p < high) 设为 value，并通过引用更新 root。O(log(high - low))。
//   - void add(int& root, Index p, V value)
//     功能：将位置 p 更新为 op(get(root, p), value)，并通过引用更新 root。O(log(high - low))。
//   - V prod(int root, Index l, Index r)
//     功能：查询树 root 在子区间 [l, r) (low <= l <= r <= high) 上的聚合值；未开点部分视为 e()。O(log(high - low))。
//   - V get(int root, Index p)
//     功能：查询树 root 在单点 p (low <= p < high) 处的值（未访问过返回 e()）。O(log(high - low))。
//   - V all_prod(int root)
//     功能：返回树 root 在整个值域 [low, high) 上的聚合结果。时间复杂度 O(1)。
//   - template <class F> Index max_right(int root, Index l, F f)
//     功能：在树 root 上从 l (low <= l <= high) 向右二分（要求 f(e()) == true），返回最大的 r in [l, high]。O(log(high - low))。
//   - template <class F> Index min_left(int root, Index r, F f)
//     功能：在树 root 上从 r (low <= r <= high) 向左二分（要求 f(e()) == true），返回最小的 l in [low, r]。O(log(high - low))。
//   - void merge(int& a, int& b)
//     功能：将树 b 原地合并入树 a（对应位置按 op(val_a, val_b) 合并），合并后将 b 置为 0（原树 b 不可再独立使用）。
//     复杂度：单次为两树公共节点数，n 棵单点树两两合并到底的总时间复杂度为 O(n log(high - low))。
// ============================================================================

template <class V, auto op, auto e, class Index = long long>
class DynamicSegmentTree {
    struct Node {
        V value;
        int left = 0, right = 0;
        Node(V value) : value(value) {}
    };
    Index low, high;
    std::vector<Node> nodes;

    V value(int root) {
        return root ? nodes[root].value : e();
    }

    template <class F>
    std::pair<Index, bool> max_right_node(int root, Index l, Index r, Index q, V& sum, F f) {
        if (r <= q) return {r, true};
        if (q <= l) {
            V next = op(sum, value(root));
            if (f(next)) {
                sum = next;
                return {r, true};
            }
            if (l + 1 == r) return {l, false};
        }
        Index mid = binary_midpoint(l, r);
        if (q < mid) {
            auto left = max_right_node(root ? nodes[root].left : 0, l, mid, q, sum, f);
            if (!left.second) return left;
        }
        return max_right_node(root ? nodes[root].right : 0, mid, r, q, sum, f);
    }

    template <class F>
    std::pair<Index, bool> min_left_node(int root, Index l, Index r, Index q, V& sum, F f) {
        if (q <= l) return {l, true};
        if (r <= q) {
            V next = op(value(root), sum);
            if (f(next)) {
                sum = next;
                return {l, true};
            }
            if (l + 1 == r) return {r, false};
        }
        Index mid = binary_midpoint(l, r);
        if (mid < q) {
            auto right = min_left_node(root ? nodes[root].right : 0, mid, r, q, sum, f);
            if (!right.second) return right;
        }
        return min_left_node(root ? nodes[root].left : 0, l, mid, q, sum, f);
    }

    int set_node(int root, Index l, Index r, Index p, V value) {
        if (!root) {
            root = (int)nodes.size();
            nodes.emplace_back(e());
        }
        if (l + 1 == r) {
            nodes[root].value = value;
            return root;
        }
        Index mid = binary_midpoint(l, r);
        if (p < mid) {
            int child = set_node(nodes[root].left, l, mid, p, value);
            nodes[root].left = child;
        } else {
            int child = set_node(nodes[root].right, mid, r, p, value);
            nodes[root].right = child;
        }
        nodes[root].value = op(nodes[nodes[root].left].value, nodes[nodes[root].right].value);
        return root;
    }

    V prod_node(int root, Index l, Index r, Index a, Index b) {
        if (!root || b <= l || r <= a) {
            return e();
        }
        if (a <= l && r <= b) {
            return nodes[root].value;
        }
        Index mid = binary_midpoint(l, r);
        return op(prod_node(nodes[root].left, l, mid, a, b),
                  prod_node(nodes[root].right, mid, r, a, b));
    }

    int merge_nodes(int a, int b, Index l, Index r) {
        if (!a || !b) {
            return a ? a : b;
        }
        if (l + 1 == r) {
            nodes[a].value = op(nodes[a].value, nodes[b].value);
            return a;
        }
        Index mid = binary_midpoint(l, r);
        nodes[a].left = merge_nodes(nodes[a].left, nodes[b].left, l, mid);
        nodes[a].right = merge_nodes(nodes[a].right, nodes[b].right, mid, r);
        nodes[a].value = op(nodes[nodes[a].left].value, nodes[nodes[a].right].value);
        return a;
    }

public:
    // DynamicSegmentTree(Index low, Index high) — 功能：指定全局左闭右开坐标域 [low, high)（要求 low <= high）并初始化 0 号空节点。O(1)。
    DynamicSegmentTree(Index low, Index high)
        : low(low), high(high), nodes{Node(e())} {
        assert(low < high);
    }

    // void set(int& root, Index p, V value) — 功能：在以 root 为根的树中将位置 p (low <= p < high) 设为 value，并通过引用更新 root。O(log(high - low))。
    void set(int& root, Index p, V value) {
        assert(low <= p && p < high);
        root = set_node(root, low, high, p, value);
    }

    // void add(int& root, Index p, V value) — 功能：将位置 p 更新为 op(get(root, p), value)，并通过引用更新 root。O(log(high - low))。
    void add(int& root, Index p, V value) {
        set(root, p, op(get(root, p), value));
    }

    // V prod(int root, Index l, Index r) — 功能：查询树 root 在子区间 [l, r) (low <= l <= r <= high) 上的聚合值；未开点部分视为 e()。O(log(high - low))。
    V prod(int root, Index l, Index r) {
        assert(low <= l && l <= r && r <= high);
        return prod_node(root, low, high, l, r);
    }

    // V get(int root, Index p) — 功能：查询树 root 在单点 p (low <= p < high) 处的值（未访问过返回 e()）。O(log(high - low))。
    V get(int root, Index p) {
        assert(low <= p && p < high);
        return prod_node(root, low, high, p, p + 1);
    }

    // V all_prod(int root) — 功能：返回树 root 在整个值域 [low, high) 上的聚合结果。时间复杂度 O(1)。
    V all_prod(int root) {
        return nodes[root].value;
    }

    // template <class F> Index max_right(int root, Index l, F f) — 功能：在树 root 上从 l (low <= l <= high) 向右二分（要求 f(e()) == true），返回最大的 r in [l, high]。O(log(high - low))。
    template <class F>
    Index max_right(int root, Index l, F f) {
        assert(low <= l && l <= high && f(e()));
        if (l == high) return high;
        V sum = e();
        return max_right_node(root, low, high, l, sum, f).first;
    }

    // template <class F> Index min_left(int root, Index r, F f) — 功能：在树 root 上从 r (low <= r <= high) 向左二分（要求 f(e()) == true），返回最小的 l in [low, r]。O(log(high - low))。
    template <class F>
    Index min_left(int root, Index r, F f) {
        assert(low <= r && r <= high && f(e()));
        if (r == low) return low;
        V sum = e();
        return min_left_node(root, low, high, r, sum, f).first;
    }

    // 单点增量更新：a[p] = op(a[p], value)，要求 op 满足结合律与交换律（如加法/最值）。

    // void merge(int& a, int& b) — 功能：将树 b 原地合并入树 a（对应位置按 op(val_a, val_b) 合并），合并后将 b 置为 0（原树 b 不可再独立使用）。
    void merge(int& a, int& b) {
        assert(&a != &b && (!a || a != b));
        a = merge_nodes(a, b, low, high);
        b = 0;
    }
};
#endif
