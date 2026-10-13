# Kruskal

给定一张带边权的无向图，选出不成环的边，使每个连通块中的总权值最小。

## 类型

```cpp
template <typename T>
struct KruskalEdge {
    int u, v;
    T weight;
};
```

**聚合类型，无构造函数**，用花括号初始化：`{u, v, w}`。

依赖 `dsu.hpp`。

## 接口

```cpp
template <typename T>
std::pair<T, std::vector<KruskalEdge<T>>> kruskal(const int n, std::vector<KruskalEdge<T>> edges);
```

**`edges` 按值传入**：内部会 `std::sort` 这个副本，**调用方的原 vector 不被改动**。

**返回值**
- 第一个分量：最小生成森林的总权
- 第二个分量：选中的边列表

**约束**
- **无 `assert`**
- 顶点编号 $0$ 到 $n-1$
- `T` 需可默认构造（`T weight = T();`）且支持 `<` 与 `+=`

**复杂度**
- $O(m \log m)$（排序为瓶颈），源码未注明

## 使用示例

```cpp
#include "kruskal.hpp"
#include <iostream>
#include <vector>

int main() {
    int n = 4;
    std::vector<KruskalEdge<int>> edges = {
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3},
        {0, 3, 10},
        {1, 3, 4}
    };

    auto [total_weight, chosen] = kruskal(n, edges);

    std::cout << total_weight << '\n';   // 1 + 2 + 3 = 6
    for (auto e : chosen) {
        std::cout << e.u << '-' << e.v << " (" << e.weight << ")\n";
    }

    return 0;
}
```

### 判定连通性

```cpp
auto [weight, chosen] = kruskal(n, edges);
bool connected = ((int)chosen.size() == n - 1);
```

**返回结果不指示连通性**：图不连通时返回的是最小生成森林，`chosen.size() < n - 1`。源码未提供任何标志位。

### 最大生成树

把权取负后排序即可：

```cpp
for (auto &e : edges) e.weight = -e.weight;
auto [neg_weight, chosen] = kruskal(n, edges);
// 最大生成树权 = -neg_weight
```

## 前置条件与实现约定

- **必须自行判断连通性**。返回值里没有 `bool` 或连通分量数；通过 `chosen.size() == n - 1` 判断。
- **`edges` 按值传入**，大 $m$ 时有一次拷贝开销。不介意修改原数组时可以用 `std::move(edges)`。
- **自环边会被跳过**（`u == v` 时 `dsu.same` 判为同集合），不会选入结果。
- 平行边正常处理：只要不成环就都会被考虑。
- 排序用 `a.weight < b.weight`，**不稳定**（但生成树总权不受影响）。
- `T` 需支持 `+=` 以累加总权，且需容纳总和。

## 相关文档

- [DSU](dsu.md) —— 本文件的依赖
- [MatrixTree](matrix_tree.md) —— 生成树计数（不是求最小生成树）
- [Dijkstra](dijkstra.md) —— 最短路
