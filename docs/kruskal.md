# Kruskal Minimum Spanning Tree (`kruskal.hpp`)

`kruskal.hpp` 实现基于边权排序与并查集的 Kruskal 最小生成树（或最小生成森林）算法 `kruskal`。依赖 [`dsu.hpp`](dsu.md)。

## 接口说明

```cpp
template <typename T>
struct KruskalEdge {
    int u, v;
    T weight;
};

template <typename T>
std::pair<T, std::vector<KruskalEdge<T>>> kruskal(
    int n, std::vector<KruskalEdge<T>> edges);
```

- **索引约定**：顶点编号为 **`0-based`**，范围 $0 \le u, v < n$。边权类型 `T` 需支持 `<` 比较、默认构造 `T()` 及 `+=`。
- **返回值**：返回 `std::pair<T, std::vector<KruskalEdge<T>>>`（可用 `auto [total_weight, mst_edges] = kruskal(n, edges);` 接收）：
  1. `first`：所选生成树（或生成森林）的边权总和；
  2. `second`：按边权升序选入生成树（或森林）的边列表。
- **连通性判定**：算法对不连通图同样适用，此时返回的是**最小生成森林**。**若题目要求判定原图是否连通（即是否存在唯一一棵覆盖全部 $n$ 个顶点的生成树），调用者必须在返回后检查 `mst_edges.size() == (size_t)std::max(0, n - 1)`**。

## 复杂度

- 时间复杂度：$O(m \log m + n)$，其中 $m = \text{edges.size()}$ 为边数，$n$ 为顶点数。
- 空间复杂度：$O(n + m)$。

## 使用示例

```cpp
#include "kruskal.hpp"
#include <cassert>
#include <vector>

int main() {
    std::vector<KruskalEdge<long long>> edges = {
        {0, 1, 4}, {0, 2, 1}, {1, 2, 2}, {1, 3, 5}, {2, 3, 8}
    };
    auto [total_weight, tree_edges] = kruskal(4, edges);

    assert(tree_edges.size() == 3); // 4 个顶点选出 3 条边，说明图连通
    assert(total_weight == 1 + 2 + 5);
    return 0;
}
```

## 相关文档

- [DSU](dsu.md) —— Kruskal 内部使用的并查集
- [MatrixTree](matrix_tree.md) —— 统计生成树数量的矩阵树定理
