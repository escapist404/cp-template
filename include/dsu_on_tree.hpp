#ifndef DSU_ON_TREE_HPP
#define DSU_ON_TREE_HPP

#include <algorithm>
#include <cassert>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

// 【树上启发式合并 / Sack 算法 (dsu_on_tree.hpp)】
// - 0-based 连通无向树，全非递归实现，每个节点作为轻子树成员最多被加入和移除 O(log n) 次
// - solve(root, state, add, remove, answer) 回调约定:
//   1. add(int u, State& st)         : 将顶点 u 的单点贡献加入全局状态 st
//   2. remove(int u, State& st)      : 将顶点 u 的单点贡献从 st 中移除（必须为 add 的严格逆操作）
//   3. answer(const State& st, int u): 当 st 中恰好包含整个子树 u 时调用，返回子树 u 的查询答案（注意参数顺序为 st 在前、u 在后）

class DSUOnTree {
    std::vector<std::vector<int>> graph;

public:
    DSUOnTree(int n) : graph(n) {}

    void add_edge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

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
