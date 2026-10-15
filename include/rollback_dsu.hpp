#ifndef ROLLBACK_DSU_HPP
#define ROLLBACK_DSU_HPP

#include <cassert>
#include <numeric>
#include <utility>
#include <vector>

// 【可撤销并查集 (rollback_dsu.hpp)】
// - 0-based 下标 [0, n)，仅按大小合并（无路径压缩），find/merge 严格 O(log n)，常用于线段树分治与整体二分
// - merge(a, b)      : 合并集合；若发生实际合并则记录历史并返回 true，已连通则返回 false
// - snapshot()       : O(1) 返回当前历史操作栈版本号 version
// - rollback(version): O(变更次数) 将并查集状态精确回滚至 snapshot() 对应的版本
// - component_count() / component_size(x): 查询当前连通块总数与 x 所在连通块大小

class RollbackDSU {
    std::vector<int> father, sizes;
    std::vector<std::pair<int, int>> history;
    int components;

public:
    RollbackDSU(int n) : father(n), sizes(n, 1), components(n) {
        std::iota(father.begin(), father.end(), 0);
    }

    int get(int x) const {
        assert(0 <= x && x < (int)father.size());
        while (father[x] != x) {
            x = father[x];
        }
        return x;
    }

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

    int snapshot() const {
        return history.size();
    }

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

    int component_count() const {
        return components;
    }

    int component_size(int x) const {
        return sizes[get(x)];
    }
};
#endif
