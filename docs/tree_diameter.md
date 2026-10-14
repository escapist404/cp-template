# Tree Diameter (`tree_diameter.hpp`)

`tree_diameter.hpp` 通过两次 DFS 实现无向树的直径与最远顶点查询，同时提供带边权版本与默认边权为 $1$ 的无权树重载。

## 接口说明

```cpp
template <typename T>
std::pair<T, int> tree_diameter_farthest(
    const std::vector<std::vector<std::pair<int, T>>>& adj,
    int start);

template <typename T>
std::pair<T, std::pair<int, int>> tree_diameter(
    const std::vector<std::vector<std::pair<int, T>>>& adj);

inline std::pair<int, std::pair<int, int>> tree_diameter(
    const std::vector<std::vector<int>>& adj);
```

- **前提条件与索引约定**：
  - 输入图 `adj` 必须是一棵**连通无向树**，顶点编号为 **`0-based`** $[0, n)$。
  - 由于使用两次 DFS（先从顶点 `0` 找到最远点 `u`，再从 `u` 找到最远点 `v`），**树中所有边权必须非负**（若存在负权边，两次 DFS 法不成立，需改用树形 DP）。
- **返回值**：
  - `tree_diameter_farthest(adj, start)`：返回 `{max_dist, farthest_vertex}`，即从 `start` 出发的最远距离及对应的端点编号；
  - `tree_diameter(adj)`：返回 `{diameter_length, {u, v}}`，其中 `first` 为直径总长度，`second` 为直径的一对端点 `{u, v}`。当输入为空图 `adj.empty()` 时返回 `{T(), {-1, -1}}`。

## 复杂度

- 时间复杂度：$O(n)$，其中 $n = \text{adj.size()}$。
- 空间复杂度：$O(n)$。

## 使用示例

```cpp
#include "tree_diameter.hpp"
#include <cassert>
#include <utility>
#include <vector>

int main() {
    // 带权树: 0 --(5)-- 1 --(3)-- 2, 1 --(6)-- 3
    std::vector<std::vector<std::pair<int, long long>>> adj(4);
    auto add_edge = [&](int u, int v, long long w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    };
    add_edge(0, 1, 5);
    add_edge(1, 2, 3);
    add_edge(1, 3, 6);

    auto [diam, endpoints] = tree_diameter(adj);
    assert(diam == 11); // 路径 0 - 1 - 3 长度为 5 + 6 = 11
    return 0;
}
```

## 相关文档

- [HLD](hld.md) —— 树链剖分与树上路径查询
- [CentroidDecomposition](centroid_decomposition.md) —— 处理一般树上路径统计的点分治
