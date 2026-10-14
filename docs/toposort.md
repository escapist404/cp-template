# Topological Sort (`toposort.hpp`)

`toposort.hpp` 实现基于入度队列（Kahn 算法）的有向图拓扑排序与有向环判定函数 `topological_sort`。

## 接口说明

```cpp
std::vector<int> topological_sort(const std::vector<std::vector<int>>& adj);
```

- **图表示与索引约定**：顶点编号为 **`0-based`**，范围 $0 \le u < n$（$n = \text{adj.size()}$），`adj[u]` 为从 `u` 出发的有向边终点列表。
- **返回值与判环约定**：
  - 若有向图是无环图（DAG），返回长度为 $n$ 的拓扑序顶点排列 `order`，满足对任意有向边 $u \to v$，都有 $u$ 在 `order` 中出现在 $v$ 之前；
  - **若图中存在有向环（包括自环），函数会将结果清空并返回空向量 `{}`**。因此当 $n > 0$ 时，可通过检查 **`order.empty()`** 直接判断原图是否含环。

## 复杂度

- 时间复杂度：$O(n + m)$，其中 $n$ 为顶点数，$m$ 为有向边数。
- 空间复杂度：$O(n)$。

## 使用示例

```cpp
#include "toposort.hpp"
#include <cassert>
#include <vector>

int main() {
    std::vector<std::vector<int>> dag = {{1, 2}, {3}, {3}, {}};
    auto order = topological_sort(dag);
    assert(order.size() == 4);
    assert(order.front() == 0 && order.back() == 3);

    // 含环图返回空向量
    std::vector<std::vector<int>> cyclic = {{1}, {2}, {0}};
    assert(topological_sort(cyclic).empty());
    return 0;
}
```

## 相关文档

- [SCC](scc.md) —— 有向图强连通分量缩点转 DAG
- [FunctionalGraph](functional_graph.md) —— 每个点出度恰为 $1$ 的基环内向树森林
