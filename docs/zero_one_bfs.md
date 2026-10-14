# 0-1 BFS (`zero_one_bfs.hpp`)

`zero_one_bfs.hpp` 使用双端队列（`std::deque`）在严格线性时间内计算边权仅含 $0$ 和 $1$ 的有向或无向图的单源最短路 `zero_one_bfs`。

## 接口说明

```cpp
std::vector<int> zero_one_bfs(
    const std::vector<std::vector<std::pair<int, int>>>& adj,
    int source);
```

- **参数与约束**：
  - 顶点编号为 **`0-based`**，范围 $0 \le u < n$（其中 $n = \text{adj.size()}$），起点需满足 $0 \le \text{source} < n$。
  - `adj[u]` 存储从 `u` 出发的邻接边 `std::pair<int, int>{to, weight}`。**每条边的权值 `weight` 必须严格等于 `0` 或 `1`**（否则会触发内部 `assert(weight == 0 || weight == 1)`）。
- **返回值**：返回长度为 $n$ 的最短距离数组 `distance`，其中 `distance[source] == 0`，**不可达顶点的距离为 `INT_MAX`（`<climits>`）**。

## 复杂度

- 时间复杂度：$O(n + m)$，其中 $n$ 为顶点数，$m$ 为边数。
- 空间复杂度：$O(n)$。

## 使用示例

```cpp
#include "zero_one_bfs.hpp"
#include <cassert>
#include <climits>
#include <utility>
#include <vector>

int main() {
    std::vector<std::vector<std::pair<int, int>>> adj(4);
    adj[0].push_back({1, 1});
    adj[0].push_back({2, 0});
    adj[2].push_back({1, 0});

    auto dist = zero_one_bfs(adj, 0);
    assert(dist[0] == 0);
    assert(dist[2] == 0);
    assert(dist[1] == 0);
    assert(dist[3] == INT_MAX); // 顶点 3 不可达
    return 0;
}
```

## 相关文档

- [Dijkstra](dijkstra.md) —— 一般非负实数/整数边权的最短路算法
