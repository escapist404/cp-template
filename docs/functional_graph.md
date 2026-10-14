# Functional Graph (`functional_graph.hpp`)

`functional_graph.hpp` 实现针对函数图（每个顶点出度恰为 $1$ 的有向图，即基环内向树森林）的分解与倍增跳转结构 `FunctionalGraph`，支持 $O(\log k)$ 查询从任意点沿出边走 $k$ 步到达的顶点，以及 $O(1)$ 查询所属基环编号、入环距离和所在基环长度。

## 接口说明

```cpp
class FunctionalGraph {
public:
    FunctionalGraph(const std::vector<int>& next);
    int jump(int vertex, std::uint64_t steps);
    int component(int vertex);
    int distance_to_cycle(int vertex);
    int cycle_length(int vertex);
};
```

- **索引与参数约定**：顶点编号为 **`0-based`**，范围 $0 \le u < n$（$n = \text{next.size()}$）。
- `FunctionalGraph(const std::vector<int>& next)`：传入长度为 $n$ 的后继数组 `next`，其中 `next[u]` 表示从 `u` 出发的唯一有向边终点（**必须满足 $0 \le \text{next}[u] < n$**，否则触发 `assert`）。构造时自动通过拓扑剥叶识别所有有向环，反向 BFS 计算每个点到环的距离，并预处理 $64$ 层倍增表 `up[64][n]`。
- `jump(int vertex, std::uint64_t steps)`：返回从 `vertex`（$0 \le \text{vertex} < n$）出发沿 `next` 连续走 `steps` 步（支持高达 $2^{64}-1$ 步）后到达的顶点编号。
- `component(int vertex)`：返回 `vertex` 所属弱连通分量（对应唯一一个有向基环）的 **`0-based` 编号**。两个顶点能经过若干步走到同一个环当且仅当它们的 `component` 相同。
- `distance_to_cycle(int vertex)`：返回从 `vertex` 出发首次进入基环所需的最少步数（若 `vertex` 本身就在环上则返回 `0`）。
- `cycle_length(int vertex)`：返回 `vertex` 所在连通分量的基环长度（$\ge 1$）。

## 复杂度

- 构造：时间与空间复杂度均为 $O(n \log(2^{64})) = O(64 n)$。
- `jump(vertex, steps)`：时间复杂度 $O(\log(\text{steps})) \le O(64)$。
- `component`、`distance_to_cycle`、`cycle_length`：严格 $O(1)$。

## 使用示例

```cpp
#include "functional_graph.hpp"
#include <cassert>
#include <vector>

int main() {
    // 0 -> 1 -> 2 -> 3 -> 1 (环为 1->2->3->1，长度为 3；0 在环外距离 1)
    FunctionalGraph fg({1, 2, 3, 1});

    assert(fg.distance_to_cycle(0) == 1);
    assert(fg.distance_to_cycle(2) == 0);
    assert(fg.cycle_length(0) == 3);
    assert(fg.component(0) == fg.component(3));

    assert(fg.jump(0, 1) == 1);
    assert(fg.jump(0, 4) == 1);
    assert(fg.jump(0, 1000000000000000000ULL) == 1);
    return 0;
}
```

## 相关文档

- [BinaryLiftingLCA](binary_lifting_lca.md) —— 树上倍增祖先与 LCA 查询
- [TopologicalSort](toposort.md) —— 一般有向图的入度拓扑剥离
