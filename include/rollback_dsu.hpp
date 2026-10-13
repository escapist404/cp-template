#ifndef ROLLBACK_DSU_HPP
#define ROLLBACK_DSU_HPP

#include <cassert>
#include <numeric>
#include <utility>
#include <vector>

// Union by size, without path compression. snapshot/rollback are O(1)/O(changes).
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
