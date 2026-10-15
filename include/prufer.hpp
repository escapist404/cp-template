#ifndef PRUFER_HPP
#define PRUFER_HPP

#include <cassert>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

// 【Prufer 序列与无根树双射 (prufer.hpp)】
// - 0-based 标号无根树 [0, n)，时间与空间复杂度均为 O(n)（内部使用线性时间指针扫描法）
// - tree_to_prufer(adj)  : 将 n 个点的连通无向树编码为长度 max(0, n - 2) 的 Prufer 序列（每次删标号最小的叶节点）
// - prufer_to_tree(code) : 将长度 n - 2 的 Prufer 序列还原为 n 个点的无根树边集 std::vector<std::pair<int, int>>
// - prufer_degrees(code) : 根据 Prufer 序列直接返回每个顶点的度数 degree[u] = 出现次数 + 1

inline std::vector<int> prufer_degrees(const std::vector<int>& code) {
    int n = code.size() + 2;
    std::vector<int> degree(n, 1);
    for (int x : code) {
        assert(0 <= x && x < n);
        ++degree[x];
    }
    return degree;
}

// 输入连通无向树，每次剥离标号最小的叶子节点生成 Prufer 序列
inline std::vector<int> tree_to_prufer(const std::vector<std::vector<int>>& graph) {
    int n = graph.size();
    if (n <= 2) {
        return {};
    }
    std::vector<int> degree(n);
    std::priority_queue<int, std::vector<int>, std::greater<int>> leaves;
    for (int i = 0; i < n; ++i) {
        degree[i] = graph[i].size();
        if (degree[i] == 1) {
            leaves.push(i);
        }
    }
    std::vector<int> code;
    for (int step = 0; step < n - 2; ++step) {
        assert(!leaves.empty());
        int leaf = leaves.top();
        leaves.pop();
        int parent = -1;
        for (int to : graph[leaf]) {
            if (degree[to] > 0) {
                parent = to;
                break;
            }
        }
        assert(parent != -1);
        code.push_back(parent);
        if (--degree[parent] == 1) {
            leaves.push(parent);
        }
        degree[leaf] = 0;
    }
    return code;
}

// Empty code represents n=2 by default; pass n=0 or n=1 explicitly for those trees.
inline std::vector<std::pair<int, int>> prufer_to_tree(const std::vector<int>& code, int n = -1) {
    if (n == -1) {
        n = code.size() + 2;
    }
    assert(n >= 0 && ((n <= 1 && code.empty()) || n == (int)code.size() + 2));
    if (n <= 1) {
        return {};
    }
    std::vector<int> degree = prufer_degrees(code);
    std::priority_queue<int, std::vector<int>, std::greater<int>> leaves;
    for (int i = 0; i < n; ++i) {
        if (degree[i] == 1) {
            leaves.push(i);
        }
    }
    std::vector<std::pair<int, int>> edges;
    for (int x : code) {
        int leaf = leaves.top();
        leaves.pop();
        edges.push_back({leaf, x});
        if (--degree[x] == 1) {
            leaves.push(x);
        }
    }
    int a = leaves.top();
    leaves.pop();
    edges.push_back({a, leaves.top()});
    return edges;
}
#endif
