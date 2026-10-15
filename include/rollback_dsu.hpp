#ifndef ROLLBACK_DSU_HPP
#define ROLLBACK_DSU_HPP

#include <cassert>
#include <numeric>
#include <utility>
#include <vector>

// ============================================================================
// 【可撤销并查集 Rollback DSU (rollback_dsu.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class RollbackDSU
//   仅使用按大小合并（不使用路径压缩），保证每次合并最多修改 O(1) 个槽位，树高不超过 log2(n) + 1。
//   常配合线段树分治、整体二分或莫队回滚处理动态图连通性。下标为 0-based [0, n)。
// 构造函数与成员函数签名详解：
//   - RollbackDSU(int n)
//     功能：初始化包含 n 个独立顶点 [0, n) 的可撤销并查集。时间与空间复杂度 O(n)。
//   - int get(int x) const
//     功能：沿父指针向上查找元素 x (0 <= x < n) 所在集合的根节点（不修改树结构）。最坏时间复杂度 O(log n)。
//   - bool merge(int a, int b)
//     功能：合并 a 与 b 所在集合（较小集合挂到较大集合下），并将有效合并压入 history 栈。
//     返回：若发生了实际合并返回 true；若原本已在同一集合（未压入历史记录）返回 false。时间复杂度 O(log n)。
//   - int snapshot() const
//     功能：获取当前历史操作栈的大小（即当前状态版本号 version）。时间复杂度 O(1)。
//   - void rollback(int version)
//     功能：将并查集状态连续回退至历史栈大小等于 version 的时刻（要求 0 <= version <= snapshot()）。
//     复杂度：O(snapshot() - version)。
//   - int component_count() const
//     功能：返回当前连通分量总数。时间复杂度 O(1)。
//   - int component_size(int x) const
//     功能：返回元素 x 所在连通分量的大小。时间复杂度 O(log n)。
// ============================================================================

class RollbackDSU {
    std::vector<int> father, sizes;
    std::vector<std::pair<int, int>> history;
    int components;

public:
    // RollbackDSU(int n) — 功能：初始化包含 n 个独立顶点 [0, n) 的可撤销并查集。时间与空间复杂度 O(n)。
    RollbackDSU(int n) : father(n), sizes(n, 1), components(n) {
        std::iota(father.begin(), father.end(), 0);
    }

    // int get(int x) const — 功能：沿父指针向上查找元素 x (0 <= x < n) 所在集合的根节点（不修改树结构）。最坏时间复杂度 O(log n)。
    int get(int x) const {
        assert(0 <= x && x < (int)father.size());
        while (father[x] != x) {
            x = father[x];
        }
        return x;
    }

    // bool merge(int a, int b) — 功能：合并 a 与 b 所在集合（较小集合挂到较大集合下），并将有效合并压入 history 栈。
    bool merge(int a, int b) {
        a = get(a);
        b = get(b);
        if (a == b) {
            return false;
        }
        if (sizes[a] > sizes[b]) {
            std::swap(a, b);
        }
        history.push_back({a, b});
        father[a] = b;
        sizes[b] += sizes[a];
        --components;
        return true;
    }

    // int snapshot() const — 功能：获取当前历史操作栈的大小（即当前状态版本号 version）。时间复杂度 O(1)。
    int snapshot() const {
        return history.size();
    }

    // void rollback(int version) — 功能：将并查集状态连续回退至历史栈大小等于 version 的时刻（要求 0 <= version <= snapshot()）。
    void rollback(int version) {
        assert(0 <= version && version <= (int)history.size());
        while ((int)history.size() > version) {
            auto [a, b] = history.back();
            history.pop_back();
            sizes[b] -= sizes[a];
            father[a] = a;
            ++components;
        }
    }

    // int component_count() const — 功能：返回当前连通分量总数。时间复杂度 O(1)。
    int component_count() const {
        return components;
    }

    // int component_size(int x) const — 功能：返回元素 x 所在连通分量的大小。时间复杂度 O(log n)。
    int component_size(int x) const {
        return sizes[get(x)];
    }
};
#endif
