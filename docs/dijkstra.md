# Dijkstra

给定带非负边权的图和若干源点，求每个顶点到最近源点的最短距离。

## 接口

```cpp
template <typename T>
std::vector<T> dijkstra(
        const std::vector<std::vector<std::pair<int, T>>> &adj,
        std::initializer_list<int> sources);
```

**参数**
- `adj`：邻接表，`adj[u]` 为 `u` 的全部出边，每条边写作 `{v, w}`。节点数即 `adj.size()`，编号 $0$ 到 $n-1$
- `sources`：**源点列表**（`std::initializer_list<int>`），可传一个或多个源点

**返回值**
- 长度 $n$ 的数组 `distance`，`distance[i]` 为「任一源点到 $i$ 的最短距离」
- 不可达时为 `std::numeric_limits<T>::max()`（即保留 `inf`，不会变成 `inf + w`）

**约束**
- 边权必须**非负**（Dijkstra 的前提）
- 每个源点需满足 $0 \le s < n$，源码有 `assert`

**复杂度**
- $O((n + m)\log n)$，其中 $m$ 为边数

## 使用示例

### 单源

```cpp
#include "dijkstra.hpp"
#include <iostream>
#include <limits>
#include <vector>

int main() {
    int n = 5;
    std::vector<std::vector<std::pair<int, long long>>> adj(n);

    auto add = [&](int u, int v, long long w) {
        adj[u].push_back({v, w});
    };

    add(0, 1, 4);
    add(0, 2, 1);
    add(2, 1, 2);
    add(1, 3, 1);
    add(2, 3, 5);
    add(3, 4, 3);

    auto dist = dijkstra(adj, {0});

    for (int i = 0; i < n; ++i) {
        if (dist[i] == std::numeric_limits<long long>::max()) {
            std::cout << i << ": unreachable\n";
        } else {
            std::cout << i << ": " << dist[i] << '\n';
        }
    }
    // 0: 0, 1: 3, 2: 1, 3: 4, 4: 7

    return 0;
}
```

### 多源

`sources` 是初始化列表，多源无需建超级源点，直接把源点都写进去：

```cpp
auto dist = dijkstra(adj, {0, 3, 7});  // 三个源点同时入队
```

### 建图提示

本接口只接收**邻接表**，不接收边列表或三元组；边是**有向**的，无向边需要双向各加一条：

```cpp
adj[u].push_back({v, w});
adj[v].push_back({u, w});  // 无向
```

## 前置条件与实现约定

- **参数顺序与常见写法不同**：邻接表在前，源点列表在后。
- 入参是 `std::initializer_list<int>`，因此**必须写花括号**，例如 `dijkstra(adj, {s})`；直接传一个 `std::vector<int>` 不会隐式转换为 `initializer_list`，需要写成 `dijkstra(adj, {v.begin(), v.end()})` 这类形式或改调用方式。
- 源码对**溢出**做了保护：当 `weight > 0` 且 `current_distance > inf - weight` 时跳过松弛，因此 `inf` 不会被加上权重而回绕成小数。`weight == 0` 的边不受该判断影响。
- 该保护仅覆盖「加到 `inf` 上」的情形，若图本身的真实距离接近 `T` 的上限，仍应选用更宽的 `T`。
- 没有返回前驱数组的版本，**路径还原需要自行复制实现**（或在松弛时记录 `prev`）。
- 没有 `max_flow` 式的提前终止参数，会一直算完全部可达点。

## 相关算法

- [Kruskal](kruskal.md) —— 最小生成树
- [SCC](scc.md)、[EDCC](edcc.md)、[VDCC](vdcc.md) —— 连通性
- 负权边请改用 Bellman-Ford / SPFA（本库未提供）
