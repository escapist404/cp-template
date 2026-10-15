#ifndef PRUFER_HPP
#define PRUFER_HPP

#include <cassert>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

// ============================================================================
// 【Prufer 序列与带标号无根树线性双射 (prufer.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解（顶点编号均为 0-based [0, n)，全部采用线性时间指针扫描法，时空复杂度严格 O(n)）：
//   1. inline std::vector<int> prufer_degrees(const std::vector<int>& code)
//      功能：根据长度为 n - 2 的 Prufer 序列 code 直接计算还原后树中每个顶点的度数 degree[u] = 出现次数 + 1。O(n)。
//   2. inline std::vector<int> tree_to_prufer(const std::vector<std::vector<int>>& graph)
//      功能：输入 n 个点的连通无向树邻接表 graph，每次剥离标号最小的叶子并记录其唯一邻居，
//            返回长度为 max(0, n - 2) 的 Prufer 序列（n <= 2 时返回空向量 {}）。时间复杂度严格 O(n)。
//   3. inline std::vector<std::pair<int, int>> prufer_to_tree(const std::vector<int>& code, int n = -1)
//      功能：将 Prufer 序列 code 还原为无根树的 n - 1 条无向边列表 {{u, v}, ...}。
//      参数约定：当 n >= 2 时要求 code.size() == n - 2；当传入默认 n = -1 时，若 code 非空自动按 n = code.size() + 2 还原，
//                但若需还原 n = 0, 1, 2 的空/单点/双点树（此时 code 均为空），必须显式传入对应的 n。时间复杂度严格 O(n)。
// ============================================================================

// inline std::vector<int> prufer_degrees(const std::vector<int>& code) — 功能：根据长度为 n - 2 的 Prufer 序列 code 直接计算还原后树中每个顶点的度数 degree[u] = 出现次数 + 1。O(n)。
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

// 空 Prufer 序列默认对应 n = 2 的树；若需构造 n = 0 或 n = 1 的退化树请显式传入 vertex_count。
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
