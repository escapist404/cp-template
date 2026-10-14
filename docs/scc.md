# SCC

SCC、EDCC、VDCC 共用 `scc.hpp` 中的 `tarjan_traverse` Tarjan 遍历骨架；三个模块只保留各自的回边和出栈处理逻辑。

把有向图划分为互相可达的最大顶点集合，并把这些集合压缩成有向无环图。

## 接口

```cpp
inline std::vector<std::vector<int>> get_scc(const std::vector<std::vector<int>> &adj);
inline std::vector<std::vector<int>> get_dag(const std::vector<std::vector<int>> &adj,
                                             const std::vector<std::vector<int>> &components);
inline std::vector<std::vector<int>> get_dag(const std::vector<std::vector<int>> &adj);
```

全部为自由函数，带 `inline`。文件注释：`// 0 index`，顶点编号 $0$ 到 $n-1$。

## get_scc

```cpp
std::vector<std::vector<int>> get_scc(const std::vector<std::vector<int>> &adj);
```

返回强连通分量的点集合。外层每个元素是一个分量，内层保存该分量中的顶点编号；所有分量构成顶点集合的划分。

**编号规则**
- 分量编号从 0 开始，按 Tarjan 的**弹出顺序递增**
- 分量内顶点按 Tarjan 出栈顺序排列，顺序不应依赖

**约束**
- 源码中**没有任何 `assert`**
- `adj` 是邻接表；算法面向有向图，但对无向图也能跑（结果退化为连通分量）

**复杂度**
- $O(n + m)$

## get_dag

```cpp
std::vector<std::vector<int>> get_dag(
    const std::vector<std::vector<int>> &adj,
    const std::vector<std::vector<int>> &components);
std::vector<std::vector<int>> get_dag(const std::vector<std::vector<int>> &adj);
```

按给定（或自行计算的）点集合把图缩成 DAG。`get_dag(adj)` 内部使用 `get_scc(adj)`；显式传入的 `components` 应是顶点集合的划分，分量编号就是外层下标。

**返回值**
- 大小等于 `components.size()` 的邻接表
- `components` 为空时返回 `{}`

**注意**：**不做重边去重**。同一对分量之间有多条边时，输出中会出现重复的邻接项。

## 使用示例

```cpp
#include "scc.hpp"
#include <iostream>

int main() {
    // 0 → 1 → 2 → 0 (环), 2 → 3
    std::vector<std::vector<int>> adj = {
        {1}, {2}, {0, 3}, {}
    };

    auto scc = get_scc(adj);
    for (int id = 0; id < (int)scc.size(); ++id) {
        std::cout << "component " << id << ": ";
        for (int v : scc[id]) std::cout << v << ' ';
        std::cout << '\n';
    }
    // 0, 1, 2 同属一个分量；3 单独一个

    auto dag = get_dag(adj, scc);
    // dag 是缩点后的 DAG

    return 0;
}
```

### 缩点后跑 DP（最长路）

```cpp
auto scc = get_scc(adj);
auto dag = get_dag(adj, scc);

const int m = dag.size();
std::vector<int> dp(m, 0);
// 按分量编号顺序处理（编号按弹出顺序递增，需自行确认拓扑序方向，
// 必要时用拓扑排序重排）
for (int u = 0; u < m; ++u) {
    for (int v : dag[u]) {
        dp[v] = std::max(dp[v], dp[u] + 1);
    }
}
```

### 补充说明与使用约定

- **分量编号的拓扑方向未定义**。若缩点后要做 DP，不能想当然认为编号递增就是拓扑序——要么自己跑一遍拓扑排序，要么先用小数据验证方向。
- `get_dag` **不去重边**，DAG 的边数可能远大于实际不同边数，DP 时若做计数要先去重。
- `get_dag(adj, components)` **不校验点集合是否覆盖且不重复**，参数不匹配时行为未定义。
- Tarjan 用**递归 lambda**实现，深链状图上有递归爆栈风险，源码未提供迭代版本。

## 相关文档

- [EDCC](edcc.md) —— 边双连通分量（无向图）
- [VDCC](vdcc.md) —— 点双连通分量（无向图）
- [TwoSat](two_sat.md) —— 2-SAT，内部也用 SCC 思想
