# Dijkstra (`dijkstra.hpp`)

`dijkstra.hpp` 实现基于二叉堆（`std::priority_queue`）的多源非负权最短路算法 `dijkstra`，支持从单个或多个起点同时出发，计算到所有顶点的最短路径距离。

## 接口说明

```cpp
template <typename T>
std::vector<T> dijkstra(
    const std::vector<std::vector<std::pair<int, T>>>& adj,
    const std::initializer_list<int> sources);
```

- **图表示与索引约定**：
  - 顶点编号为 **`0-based`**，范围 $0 \le u < n$（其中 $n = \text{adj.size()}$）。
  - 邻接表 `adj[u]` 存储从 `u` 出发的有向边列表 `std::pair<int, T>{to, weight}`（无向图需双向加边）。
  - **所有边权必须非负**（`weight >= T()`）；若图中存在负权边，请使用 [`difference_constraints.hpp`](difference_constraints.md) 中的 SPFA / Bellman-Ford。
- **参数与返回值**：
  - `sources`：起点集合，传入列表初始化形式（单源传入 `{s}`，多源传入 `{s1, s2, ...}`），要求所有起点满足 $0 \le s < n$。
  - 返回长度为 $n$ 的最短距离数组 `distance`：所有起点初值为 `T()`（即 $0$），**不可达顶点的距离保持为 `std::numeric_limits<T>::max()`**。内部在松弛时自动检查 `current_distance > inf - weight`，避免无穷大或超大边权相加发生有符号整数溢出。

## 复杂度

- 时间复杂度：$O((n + m) \log m)$，其中 $n$ 为顶点数，$m$ 为边数。
- 空间复杂度：$O(n + m)$。

## 使用示例

```cpp
#include "dijkstra.hpp"
#include <cassert>
#include <limits>
#include <utility>
#include <vector>

int main() {
    int n = 4;
    std::vector<std::vector<std::pair<int, long long>>> adj(n);
    adj[0].push_back({1, 4});
    adj[0].push_back({2, 1});
    adj[2].push_back({1, 2});
    adj[1].push_back({3, 5});

    auto dist = dijkstra(adj, {0});
    assert(dist[0] == 0);
    assert(dist[2] == 1);
    assert(dist[1] == 3);
    assert(dist[3] == 8);
    return 0;
}
```

## 相关文档

- [ZeroOneBFS](zero_one_bfs.md) —— 边权仅为 $0$ 或 $1$ 的线性时间最短路
- [DifferenceConstraints](difference_constraints.md) —— 支持负权边与负环检测的差分约束求解
