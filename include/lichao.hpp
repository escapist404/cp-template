#ifndef LICHAO_HPP
#define LICHAO_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

// ============================================================================
// 【李超线段树（支持全局直线与区间线段极值查询）Li Chao Segment Tree (lichao.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：
//   template <typename X, typename Function, typename Compare = std::less<>> class LiChaoTree
//   - Function 需支持 `Value operator()(X x) const`；
//   - 默认 Compare = std::less<> 维护【最小值】，求最大值请传 `std::greater<>`。
// 构造函数与成员函数签名详解：
//   - LiChaoTree(std::vector<X> coordinates, Compare better = Compare{})：离线离散化所有待查询横坐标建树。O(n log n)。
//   - void add(const Function& line)                                    ：插入全局直线。O(log n)。
//   - void add(const Function& line, const X& low, const X& high)       ：插入左闭右开区间 [low, high) 上的线段。O(log^2 n)。
//   - std::optional<Value> get(const X& x) const                        ：查询离散坐标点 x 处的最优函数值（无覆盖或 x 未注册返回 nullopt）。O(log n)。
// ============================================================================

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

    // 全局插入一条一次函数 y = slope * x + intercept。
    void add(const Function& line) {
        insert(1, 0, xs.size(), line);
    }

    // 在半开横坐标区间 [low, high) 上局部插入线段 y = slope * x + intercept。
    void add(const Function& line, const X& low, const X& high) {
        assert(!(high < low));
        int l = std::lower_bound(xs.begin(), xs.end(), low) - xs.begin();
        int r = std::lower_bound(xs.begin(), xs.end(), high) - xs.begin();
        insert_segment(1, 0, xs.size(), l, r, line);
    }

    // 查询横坐标 x 处的最优函数值（最小值或最大值，取决于 Compare）。
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
