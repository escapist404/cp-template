#ifndef LICHAO_HPP
#define LICHAO_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

// 【李超线段树 Li Chao Tree (lichao.hpp)】
// - 在离散或连续横坐标集合上动态插入一次函数 y = m * x + b（或线段），支持 O(log n) 单点查询极值（默认求最小值，可配置最大值）

template <typename X, typename Function, typename Compare = std::less<>>
class LiChaoTree {
    using Value = std::decay_t<decltype(std::declval<Function>()(std::declval<X>()))>;
    std::vector<X> xs;
    std::vector<std::optional<Function>> tree;
    Compare better;

    void insert(int node, int l, int r, Function line) {
        if (!tree[node]) {
            tree[node] = std::move(line);
            return;
        }
        int mid = (l + r) / 2;
        bool left = better(line(xs[l]), (*tree[node])(xs[l]));
        bool middle = better(line(xs[mid]), (*tree[node])(xs[mid]));
        if (middle) {
            std::swap(line, *tree[node]);
        }
        if (r - l == 1) {
            return;
        }
        if (left != middle) {
            insert(node * 2, l, mid, std::move(line));
        } else {
            insert(node * 2 + 1, mid, r, std::move(line));
        }
    }

    void insert_segment(int node, int l, int r, int a, int b, const Function& line) {
        if (b <= l || r <= a) {
            return;
        }
        if (a <= l && r <= b) {
            insert(node, l, r, line);
            return;
        }
        int mid = (l + r) / 2;
        insert_segment(node * 2, l, mid, a, b, line);
        insert_segment(node * 2 + 1, mid, r, a, b, line);
    }

public:
    LiChaoTree(std::vector<X> coordinates, Compare better = Compare{})
        : xs(std::move(coordinates)), better(better) {
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
        assert(!xs.empty());
        tree.resize(4 * xs.size());
    }

    // Add a function on all coordinates.
    void add(const Function& line) {
        insert(1, 0, xs.size(), line);
    }

    // Activate a function only at coordinates x in [low,high).
    void add(const Function& line, const X& low, const X& high) {
        assert(!(high < low));
        int l = std::lower_bound(xs.begin(), xs.end(), low) - xs.begin();
        int r = std::lower_bound(xs.begin(), xs.end(), high) - xs.begin();
        insert_segment(1, 0, xs.size(), l, r, line);
    }

    // Return the best value at coordinate x.
    std::optional<Value> get(const X& x) const {
        int p = std::lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        assert(p < (int)xs.size() && xs[p] == x);
        int node = 1, l = 0, r = xs.size();
        std::optional<Value> result;
        while (true) {
            if (tree[node]) {
                Value value = (*tree[node])(x);
                if (!result || better(value, *result)) {
                    result = std::move(value);
                }
            }
            if (r - l == 1) {
                return result;
            }
            int mid = (l + r) / 2;
            if (p < mid) {
                r = mid;
                node *= 2;
            } else {
                l = mid;
                node = node * 2 + 1;
            }
        }
    }
};
#endif
