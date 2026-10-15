#ifndef TREE_HASH_HPP
#define TREE_HASH_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

// ============================================================================
// 【有根树与无根树同构哈希 Tree Isomorphism Hash (tree_hash.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class TreeHash
//   基于 SplitMix64 非线性混淆器计算有根树或无根树的 64 位同构哈希值，顶点编号为 0-based [0, n)。
//   【重要提示】比较两棵树是否同构时，两个 TreeHash 实例必须使用完全相同的随机种子 seed。
// 构造函数与成员函数签名详解：
//   - TreeHash(int n, u64 seed = 0x243f6a8885a308d3ULL)
//     功能：初始化包含 n 个顶点 [0, n) 的树容器及哈希种子 seed。O(n)。
//   - void add_edge(int u, int v)
//     功能：添加无向树边 (u, v)（要求 0 <= u, v < n，总共添加 n - 1 条边构成连通树）。O(1)。
//   - std::vector<int> centers() const
//     功能：通过拓扑剥叶法返回无根树的 1 个或 2 个中心（重心/直径中心）顶点编号。时间复杂度 O(n)。
//   - u64 solve(int root = -1) const
//     功能：
//       - 当传入 0 <= root < n 时，计算以 root 为根的【有根树同构哈希值】；
//       - 当使用默认值 root == -1 时，自动枚举 centers() 中的 1~2 个中心计算【无根树同构哈希值】。
//     复杂度：因需对各节点子树的哈希值排序，时间复杂度为 O(n log n)，空间复杂度 O(n)。
// ============================================================================

class TreeHash {
    using u64 = std::uint64_t;
    u64 seed;
    std::vector<std::vector<int>> graph;

    static u64 mix(u64 x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

public:
    // TreeHash(int n, u64 seed = 0x243f6a8885a308d3ULL) — 功能：初始化包含 n 个顶点 [0, n) 的树容器及哈希种子 seed。O(n)。
    TreeHash(int n, u64 seed = 0x243f6a8885a308d3ULL) : seed(seed), graph(n) {}

    // void add_edge(int u, int v) — 功能：添加无向树边 (u, v)（要求 0 <= u, v < n，总共添加 n - 1 条边构成连通树）。O(1)。
    void add_edge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // std::vector<int> centers() const — 功能：通过拓扑剥叶法返回无根树的 1 个或 2 个中心（重心/直径中心）顶点编号。时间复杂度 O(n)。
    std::vector<int> centers() const {
        int n = graph.size();
        std::vector<int> degree(n), leaves;
        for (int i = 0; i < n; ++i) {
            degree[i] = graph[i].size();
            if (degree[i] <= 1) {
                leaves.push_back(i);
            }
        }
        int remaining = n;
        while (remaining > 2) {
            assert(!leaves.empty());
            remaining -= leaves.size();
            std::vector<int> next;
            for (int u : leaves) {
                for (int v : graph[u]) {
                    if (--degree[v] == 1) {
                        next.push_back(v);
                    }
                }
            }
            leaves = std::move(next);
        }
        return leaves;
    }

    // u64 solve(int root = -1) const — 功能：
    u64 solve(int root = -1) const {
        if (graph.empty()) {
            return mix(seed);
        }
        if (root == -1) {
            auto c = centers();
            std::vector<u64> hashes;
            for (int center : c) {
                hashes.push_back(solve(center));
            }
            std::sort(hashes.begin(), hashes.end());
            u64 result = mix(seed ^ graph.size());
            for (u64 hash : hashes) {
                result = mix(result ^ hash);
            }
            return mix(result ^ hashes.size());
        }
        assert(0 <= root && root < (int)graph.size());
        std::vector<int> father(graph.size(), -1), order{root};
        father[root] = root;
        for (int i = 0; i < (int)order.size(); ++i) {
            int u = order[i];
            for (int v : graph[u]) {
                if (v != father[u]) {
                    assert(father[v] == -1);
                    father[v] = u;
                    order.push_back(v);
                }
            }
        }
        assert(order.size() == graph.size());
        std::vector<u64> hashes(graph.size());
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            int u = *it;
            std::vector<u64> children;
            for (int v : graph[u]) {
                if (father[v] == u && v != u) {
                    children.push_back(hashes[v]);
                }
            }
            std::sort(children.begin(), children.end());
            u64 result = mix(seed ^ children.size());
            for (u64 child : children) {
                result = mix(result ^ child);
            }
            hashes[u] = result;
        }
        return hashes[root];
    }
};
#endif
