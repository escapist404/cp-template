#ifndef DSU_ON_TREE_HPP
#define DSU_ON_TREE_HPP

#include <algorithm>
#include <cassert>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

// ============================================================================
// 【树上启发式合并 / Sack 算法 DSU on Tree (dsu_on_tree.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class DSUOnTree
//   静态树子树离线统计框架（全非递归显式任务栈实现，防爆栈），顶点编号为 0-based [0, n)。
// 构造函数与成员函数签名详解：
//   - DSUOnTree(int n)
//     功能：初始化包含 n 个顶点 [0, n) 的空树。O(n)。
//   - void add_edge(int u, int v)
//     功能：添加一条连接 u 与 v 的无向树边。O(1)。
//   - template <typename State, typename Add, typename Remove, typename Answer>
//     auto solve(int root, State& state, Add add, Remove remove, Answer answer) const
//     功能：以 root 为根执行树上启发式合并，返回长度为 n 的各子树答案数组 std::vector<Result>。
//     【回调函数签名约定】
//       1. void add(int u, State& state)          ：将顶点 u 的单点信息加入全局状态 state；
//       2. void remove(int u, State& state)       ：将顶点 u 的单点信息从 state 中撤销（必须为 add 的逆操作）；
//       3. Result answer(const State& state, int u)：当 state 恰好包含子树 u 的全部顶点时调用，返回子树 u 的答案（【注意参数顺序】state 在前，u 在后）。
//     复杂度：每个顶点最多被 add/remove O(log n) 次，总时间 O(n log n * T_op + n * T_ans)。
// ============================================================================

class DSUOnTree {
    std::vector<std::vector<int>> graph;

public:
    // DSUOnTree(int n) — 功能：初始化包含 n 个顶点 [0, n) 的空树。O(n)。
    DSUOnTree(int n) : graph(n) {}

    // void add_edge(int u, int v) — 功能：添加一条连接 u 与 v 的无向树边。O(1)。
    void add_edge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // template <typename State, typename Add, typename Remove, typename Answer> auto solve(int root, State& state, Add add, Remove remove, Answer answer) const — 功能：以 root 为根执行树上启发式合并，返回长度为 n 的各子树答案数组 std::vector<Result>。
    template <typename State, typename Add, typename Remove, typename Answer>
    auto solve(int root, State& state, Add add, Remove remove, Answer answer) const {
        using Result = std::decay_t<decltype(answer(state, root))>;
        int n = graph.size();
        assert(0 <= root && root < n);
        std::vector<int> parent(n, -1), order, stack{root}, size(n, 1), heavy(n, -1), tin(n);
        parent[root] = root;
        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            tin[u] = order.size();
            order.push_back(u);
            for (int v : graph[u]) {
                if (v != parent[u]) {
                    assert(parent[v] == -1);
                    parent[v] = u;
                    stack.push_back(v);
                }
            }
        }
        assert((int)order.size() == n);
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            int u = *it;
            for (int v : graph[u]) {
                if (parent[v] == u && v != u) {
                    size[u] += size[v];
                    if (heavy[u] == -1 || size[v] > size[heavy[u]]) {
                        heavy[u] = v;
                    }
                }
            }
        }
        struct Task {
            int type, vertex;
            bool keep;
        };
        std::vector<Task> tasks{{0, root, false}};
        std::vector<Result> result(n);
        while (!tasks.empty()) {
            Task task = tasks.back();
            tasks.pop_back();
            int u = task.vertex;
            if (task.type == 0) {
                if (!task.keep) {
                    tasks.push_back({4, u, false});
                }
                tasks.push_back({3, u, false});
                tasks.push_back({2, u, false});
                for (int v : graph[u]) {
                    if (parent[v] == u && v != u && v != heavy[u]) {
                        tasks.push_back({1, v, false});
                    }
                }
                if (heavy[u] != -1) {
                    tasks.push_back({0, heavy[u], true});
                }
                for (int v : graph[u]) {
                    if (parent[v] == u && v != u && v != heavy[u]) {
                        tasks.push_back({0, v, false});
                    }
                }
            } else if (task.type == 1 || task.type == 4) {
                for (int i = tin[u]; i < tin[u] + size[u]; ++i) {
                    if (task.type == 1) {
                        add(order[i], state);
                    } else {
                        remove(order[i], state);
                    }
                }
            } else if (task.type == 2) {
                add(u, state);
            } else {
                result[u] = answer(state, u);
            }
        }
        return result;
    }
};
#endif
