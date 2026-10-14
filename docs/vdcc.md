# VDCC

给定一张无向图，按割点划分出点双连通分量，并以每个分量包含的边集合返回。

Tarjan 的 `dfn/low` 遍历复用 `scc.hpp` 中的共享内部框架。

## 接口

```cpp
std::vector<std::vector<std::pair<int, int>>> get_vdcc(const std::vector<std::vector<int>> &adj);
```

> **注意**：同样**没有 `inline`**，多翻译单元包含会有 ODR 风险。

## 返回值（与 SCC、EDCC 不同，务必看清）

外层是点双列表，但**每个分量用「边的列表」表示，而不是顶点列表**：

```
vector< 分量 > ，分量 = vector< pair<端点, 端点> >
```

边的方向为**压栈时的方向**（树边为父→子，回边为后代→祖先），**朝向不统一**，应视作无序对使用。

## 约束

- **无 `assert`**
- 无向图，顶点按 $0$ 到 $n-1$ 使用
- 对每个未访问顶点各起一次 Tarjan（`f == -1`），因此支持不连通图

**复杂度**
- $O(n + m)$，源码未注明

## 使用示例

```cpp
#include "vdcc.hpp"
#include <iostream>

int main() {
    std::vector<std::vector<int>> adj = {
        {1, 2}, {0, 2}, {0, 1, 3}, {2, 4}, {3}
    };

    auto vdcc = get_vdcc(adj);

    for (const auto &component : vdcc) {
        std::cout << "component with " << component.size() << " edges: ";
        for (auto [u, v] : component) std::cout << "(" << u << "," << v << ") ";
        std::cout << '\n';
    }

    return 0;
}
```

### 从边表还原顶点集合

```cpp
std::set<int> vertices_of(const std::vector<std::pair<int,int>> &component) {
    std::set<int> result;
    for (auto [u, v] : component) {
        result.insert(u);
        result.insert(v);
    }
    return result;
}
```

### 补充说明与使用约定

- **孤点的顶点编号会丢失**。当某 DFS 根没有子树时（`f == -1 && !subtree_num`），会压入一个**空的分量 `{}`**——即孤点只体现为一个空 `vector`，**无法从返回值恢复它是哪个顶点**。若需要处理孤点，必须自行遍历 `adj` 找出度为零的点。
- **自环被完全丢弃**。`to == cur` 时 `dfn[to] < dfn[cur]` 为假，自环既不入栈也不计入任何分量（源码未说明，由代码推得）。
- 用 `skipped_parent` 布尔量**只跳过一条父边**，因此平行边被正确保留（两条平行边属于同一个点双）。
- 返回的边方向不统一（树边父→子，回边后代→祖先），做比较时要用无序对。
- Tarjan 递归，深图有爆栈风险。
- **缺少 `inline`**。

## 相关文档

- [EDCC](edcc.md) —— 边双连通分量
- [SCC](scc.md) —— 有向图的强连通分量
- [TwoSat](two_sat.md) —— 2-SAT
