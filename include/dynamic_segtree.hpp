#ifndef DYNAMIC_SEGTREE_HPP
#define DYNAMIC_SEGTREE_HPP

#include <cassert>
#include <utility>
#include <vector>
#include "binary_search.hpp"

// 【动态开点线段树与线段树合并 (dynamic_segtree.hpp)】
// - 维护左闭右开大值域 [low, high) 上的动态开点线段树森林，单次操作 O(log(high - low))
// - 每棵树由 int root = 0 标识（0 表示全 e() 的空树）；set(root, p, v) / add(root, p, v) 按引用更新 root
// - prod(root, l, r) / get(root, p) / all_prod(root): 区间查询、单点查询与全区间聚合
// - max_right(root, l, f) / min_left(root, r, f)    : 大值域树上二分（要求 f(e()) == true）
// - merge(a, b): 原地合并两棵树并返回新根（叶子按 op(val_a, val_b) 合并；合并后旧树 b 节点被复用，不可再访问）

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
    DynamicSegmentTree(Index low, Index high)
        : low(low), high(high), nodes{Node(e())} {
        assert(low < high);
    }

    void set(int& root, Index p, V value) {
        assert(low <= p && p < high);
        root = set_node(root, low, high, p, value);
    }

    void add(int& root, Index p, V value) {
        set(root, p, op(get(root, p), value));
    }

    V prod(int root, Index l, Index r) {
        assert(low <= l && l <= r && r <= high);
        return prod_node(root, low, high, l, r);
    }

    V get(int root, Index p) {
        assert(low <= p && p < high);
        return prod_node(root, low, high, p, p + 1);
    }

    V all_prod(int root) {
        return nodes[root].value;
    }

    template <class F>
    Index max_right(int root, Index l, F f) {
        assert(low <= l && l <= high && f(e()));
        if (l == high) return high;
        V sum = e();
        return max_right_node(root, low, high, l, sum, f).first;
    }

    template <class F>
    Index min_left(int root, Index r, F f) {
        assert(low <= r && r <= high && f(e()));
        if (r == low) return low;
        V sum = e();
        return min_left_node(root, low, high, r, sum, f).first;
    }

    // For associative commutative op (e.g. sum/min/max).

    void merge(int& a, int& b) {
        assert(&a != &b && (!a || a != b));
        a = merge_nodes(a, b, low, high);
        b = 0;
    }
};
#endif
